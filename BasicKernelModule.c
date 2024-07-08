#include <linux/init.h>
#include <linux/module.h>

static int __init hello_init(void) {
    printk(KERN_INFO "Hello, kernel world!\n");
    return 0; // Non-zero return values are errors
}

static void __exit hello_exit(void) {
    printk(KERN_INFO "Goodbye, kernel world!\n");
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sahil");
MODULE_DESCRIPTION("Basic Module");
MODULE_VERSION("0.1");
