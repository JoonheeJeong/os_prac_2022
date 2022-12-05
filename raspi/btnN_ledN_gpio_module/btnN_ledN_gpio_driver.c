#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/gpio.h>
#include <linux/uaccess.h> // copy_to/from_user
#include "btnN_ledN_gpio_driver.h"

// LEDs
static int led_red      = 22;
static int led_green    = 27; // blue
static int led_white    = 17;
static int led_yellow   = 24; // result

// buttons
static int btn_red      = 26;
static int btn_blue     = 19;
static int btn_white    = 13;
static int btn_x        = 21; // submit

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
    int err;

    if ((err = gpio_request(led_red, "led_red")) != 0) {
        printk(KERN_ERR "[ERROR:%d] led_red gpio_request\n", err);
        return -1;
    } 
    if ((err = gpio_request(led_green, "led_green")) != 0) {
        printk(KERN_ERR "[ERROR:%d] led_green gpio_request\n", err);
        return -1;
    } 
    if ((err = gpio_request(led_white, "led_white")) != 0) {
        printk(KERN_ERR "[ERROR:%d] led_white gpio_request\n", err);
        return -1;
    } 
    if ((err = gpio_request(led_yellow, "led_yellow") != 0)) {
        printk(KERN_ERR "[ERROR:%d] led_yellow gpio_request\n", err);
        return -1;
    } 

    // each LED is an output, and set 0(=off)
    gpio_direction_output(led_red, 0);
    gpio_direction_output(led_green, 0);
    gpio_direction_output(led_white, 0);
    gpio_direction_output(led_yellow, 0);

    if ((err = gpio_request(btn_red, "btn_red") != 0)) {
        printk(KERN_ERR "[ERROR:%d] btn_red gpio_request\n", err);
        return -1;
    }
    if ((err = gpio_request(btn_blue, "btn_blue") != 0)) {
        printk(KERN_ERR "[ERROR:%d] btn_blue gpio_request\n", err);
        return -1;
    }
    if ((err = gpio_request(btn_white, "btn_white") != 0)) {
        printk(KERN_ERR "[ERROR:%d] btn_white gpio_request\n", err);
        return -1;
    }
    if ((err = gpio_request(btn_x, "btn_x") != 0)) {
        printk(KERN_ERR "[ERROR:%d] btn_x gpio_request\n", err);
        return -1;
    }

    // each button is an input
    gpio_direction_input(btn_red);
    gpio_direction_input(btn_blue);
    gpio_direction_input(btn_white);
    gpio_direction_input(btn_x);

    return 0;
}

static int gpio_release(struct inode *inode, struct file *filp)
{
    // free the used memory
    gpio_free(led_red);
    gpio_free(led_green);
    gpio_free(led_white);
    gpio_free(led_yellow);
    gpio_free(btn_red);
    gpio_free(btn_blue);
    gpio_free(btn_white);
    gpio_free(btn_x);
    return 0;
}

static ssize_t gpio_read(struct file *filp,
                         char *buf, size_t len, loff_t *off)
{
    // get the values of the gpio
    char btn_red_input   = gpio_get_value(btn_red);
    char btn_blue_input  = gpio_get_value(btn_blue);
    char btn_white_input = gpio_get_value(btn_white);
    char btn_x_input     = gpio_get_value(btn_x);

    char input = btn_red_input 
                | (btn_blue_input << 1)
                | (btn_white_input << 2)
                | (btn_x_input << 3);

    if (input < 0 || input >= 16) {
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
    // the writing values of the user
    char output, led_red_output, led_green_output, 
         led_white_output, led_yellow_output;

    // copy the output value to write from the user into the kernel
    if (copy_from_user(&output, buf, len) > 0) {
        printk(KERN_ERR "ERROR: copy_from_user\n");
        return -1;
    }

    if (output >= 16 || output < 0) {
        printk(KERN_ERR "ERROR: unknown user input\n");
        return -1;
    }

    led_red_output = output & 1;
    led_green_output = (output >> 1) & 1;
    led_white_output = (output >> 2) & 1;
    led_yellow_output = (output >> 3) & 1;

    // deliver the writing value
    gpio_set_value(led_red, led_red_output);
    gpio_set_value(led_green, led_green_output);
    gpio_set_value(led_white, led_white_output);
    gpio_set_value(led_yellow, led_yellow_output);
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
    printk(KERN_INFO "[LED/BTN]: led_red:%d; btn_red:%d;\n",
        led_red, btn_red);
    printk(KERN_INFO "[LED/BTN]: led_green:%d; btn_blue:%d;\n",
        led_green, btn_blue);
    printk(KERN_INFO "[LED/BTN]: led_white:%d; btn_white:%d;\n",
        led_white, btn_white);
    printk(KERN_INFO "[LED/BTN]: led_yellow:%d; btn_x:%d.\n",
        led_yellow, btn_x);
    return 0;
}

static void __exit gpio_exit(void)
{
    unregister_chrdev(DEV_GPIO_MAJOR, DEV_GPIO_NAME);

    // free the used memory
    gpio_free(led_red);
    gpio_free(led_green);
    gpio_free(led_white);
    gpio_free(led_yellow);
    gpio_free(btn_red);
    gpio_free(btn_blue);
    gpio_free(btn_white);
    gpio_free(btn_x);

    printk(KERN_INFO "Remove GPIO driver ... Done!\n");
}

module_param(led_red, int, 0);
module_param(led_green, int, 0);
module_param(led_white, int, 0);
module_param(led_yellow, int, 0);
module_param(btn_red, int, 0);
module_param(btn_blue, int, 0);
module_param(btn_white, int, 0);
module_param(btn_x, int, 0);
module_init(gpio_init);
module_exit(gpio_exit);
MODULE_LICENSE("GPL");
