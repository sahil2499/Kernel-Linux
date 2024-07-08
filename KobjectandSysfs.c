#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kobject.h>
#include <linux/sysfs.h>

static struct kobject *my_kobject; // Declare the kobject pointer

// Attribute getter function
static ssize_t my_attribute_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf) {
    return sprintf(buf, "Hello from my_attribute!\n");
}

// Attribute structure definition
static struct kobj_attribute my_attribute = {
    .attr = {
        .name = "my_attribute",
        .mode = S_IRUGO,  // Read-only permissions for this attribute
    },
    .show = my_attribute_show,  // Set the show function
};

// Module initialization function
static int __init my_module_init(void) {
    int result;

    // Create the kobject
    my_kobject = kobject_create_and_add("my_kobject", NULL);
    if (!my_kobject) {
        printk(KERN_ERR "Failed to create my_kobject\n");
        return -ENOMEM;
    }

    // Register the sysfs attribute with the kobject
    result = sysfs_create_file(my_kobject, &my_attribute.attr);
    if (result) {
        printk(KERN_ERR "Failed to create sysfs file\n");
        kobject_put(my_kobject);
        return result;
    }

    printk(KERN_INFO "my_module: Module loaded\n");
    return 0;
}

// Module exit function
static void __exit my_module_exit(void) {
    // Remove the sysfs attribute
    sysfs_remove_file(my_kobject, &my_attribute.attr);
    
    // Delete the kobject and free associated resources
    kobject_put(my_kobject);

    printk(KERN_INFO "my_module: Module unloaded\n");
}

// Define module initialization and exit macros
module_init(my_module_init);
module_exit(my_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sahil");
MODULE_DESCRIPTION("kobject and sysfs attribute");
MODULE_VERSION("1.0");
