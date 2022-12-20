#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

static int std_num = 1; // 202150003
static char *class_id = "aa";

module_param(std_num, int, 0);
module_param(class_id, charp, 0);

int __init hello_module_init(void)
{
    printk("The ID of the OS class which I'm taking is %s, and my student number: %d!\n", 
		class_id, std_num);
    return 0;
}

void __exit hello_module_exit(void)
{
    printk("Bye, module (practice)!\n");
}

module_init(hello_module_init);
module_exit(hello_module_exit);
MODULE_LICENSE("GPL");
