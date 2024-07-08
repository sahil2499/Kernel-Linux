#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/mm.h>
#include <linux/gfp.h>

static int __init my_module_init(void) {
    struct page *page;
    void *page_addr;

    // Allocate a single page
    page = alloc_pages(GFP_KERNEL, 0);
    if (!page) {
        printk(KERN_ERR "Page allocation failed\n");
        return -ENOMEM;
    }

    // Get the virtual address of the allocated page
    page_addr = page_address(page);
    printk(KERN_INFO "Allocated page at virtual address: %p\n", page_addr);

    // Use the page (write data to it)
    memset(page_addr, 0, PAGE_SIZE);

    // Free the allocated page
    __free_pages(page, 0);
    printk(KERN_INFO "Page freed\n");

    return 0;
}

static void __exit my_module_exit(void) {
    printk(KERN_INFO "Module exit\n");
}

module_init(my_module_init);
module_exit(my_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sahil");
MODULE_DESCRIPTION("Page allocation example");
