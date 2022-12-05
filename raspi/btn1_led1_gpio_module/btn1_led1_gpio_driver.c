#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/gpio.h>
#include <linux/uaccess.h> // copy_to/from_user
#include "btn1_led1_gpio_driver.h"

static int led_red = 17;
static int btn_red = 13;

// call when inserting the GPIO module
static int __init gpio_init(void);

// call when removing the GPIO module
static void __exit gpio_exit(void);

// call this when calling "open" for the GPIO driver file
static int gpio_open(struct inode *inode, struct file *filp);

// call when calling "close" for the GPIO driver file
static int gpio_release(struct inode *inode, struct file *filp);

// call when calling "read" for the GPIO driver file
static ssize_t gpio_read(struct file *filp, 
                         char *buf, size_t len, loff_t *off);

// call when calling "write" the GPIO driver file
static ssize_t gpio_write(struct file *filp,
                          const char *buf, size_t len, loff_t *off);

// connect file operations of the GPIO driver to the functions above
static struct file_operations gpio_fops = {
    .owner      = THIS_MODULE,
    .open       = gpio_open,
    .release    = gpio_release,
    .read       = gpio_read,
    .write      = gpio_write,
};

static int gpio_open(struct inode *inode, struct file *filp)
{
    if (gpio_request(led_red, "led_red") != 0) {
        printk(KERN_ERR "ERROR: led_red gpio_request\n");
        return -1;
    } else {
        printk(KERN_INFO "led_red OPEN COMPLETE\n");

        // led_red(LED) is an output, and set 0(=off)
        gpio_direction_output(led_red, 0);
    }

    if (gpio_request(btn_red, "btn_red") != 0) {
        printk(KERN_ERR "ERROR: btn_red gpio_request\n");
        return -1;
    } else {
        printk(KERN_INFO "btn_red OPEN COMPLETE\n");

        // btn_red(BUTTON) is an input
        gpio_direction_input(btn_red);
    }

    return 0;
}

static int gpio_release(struct inode *inode, struct file *filp)
{
    // free the used memory
    gpio_free(led_red);
    gpio_free(btn_red);
    return 0;
}

static ssize_t gpio_read(struct file *filp,
                         char *buf, size_t len, loff_t *off)
{
    // get the value of the gpio
    char input = gpio_get_value(btn_red);

    if (input != 0 && input != 1) {
        printk(KERN_ERR "ERROR: unknown user input\n");
        return -1;
    }

    // copy input from the user into the kernel
    if (copy_to_user(buf, &input, len) != 0) {
        printk(KERN_ERR "ERROR: copy_to_user\n");
        return -1;
    }
    //printk(KERN_INFO "READ GPIO\n");
    return len;
}

static ssize_t gpio_write(struct file *filp,
                          const char *buf, size_t len, loff_t *off)
{
    // the writing value of the user
    char output;

    // copy the output value to write from the user into the kernel
    if (copy_from_user(&output, buf, len) > 0) {
        printk(KERN_ERR "ERROR: copy_from_user\n");
        return -1;
    }

    if (output != 1 && output != 0) {
        printk(KERN_ERR "ERROR: unknown user input\n");
        return -1;
    }

    // deliver the writing value
    gpio_set_value(led_red, output);
    //printk(KERN_INFO "WRITE GPIO\n");
    return len;
}

static int __init gpio_init(void)
{
    // register the device driver
    if (register_chrdev(DEV_GPIO_MAJOR, DEV_GPIO_NAME, &gpio_fops) < 0) {
        printk(KERN_ERR "ERROR: cannot allocate %s(Major:%d)\n",
               DEV_GPIO_NAME, DEV_GPIO_MAJOR);
        return -EBUSY;
    }
    printk(KERN_INFO "Insert GPIO driver %s(Major:%d);\n",
        DEV_GPIO_NAME, DEV_GPIO_MAJOR);
    printk(KERN_INFO "[RED]: led_red:%d; btn_red:%d\n", led_red, btn_red);
    return 0;
}

static void __exit gpio_exit(void)
{
    unregister_chrdev(DEV_GPIO_MAJOR, DEV_GPIO_NAME);
    printk(KERN_INFO "Remove GPIO driver ... Done!\n");
}

module_param(led_red, int, 0);
module_param(btn_red, int, 0);
module_init(gpio_init);
module_exit(gpio_exit);
MODULE_LICENSE("GPL");
