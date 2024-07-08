#include <linux/module.h>
#include <linux/fs.h>
#include <linux/pagemap.h>
#include <linux/buffer_head.h>

void example_page_cache_usage(struct address_space *mapping, loff_t offset) {
    struct page *page;
    pgoff_t index = offset >> PAGE_SHIFT;

    
    page = read_cache_page(mapping, index, NULL, NULL);
    if (IS_ERR(page)) {
        printk(KERN_ERR "Failed to read page from cache\n");
        return;
    }

    lock_page(page);

    void *page_data = kmap(page);
    if (!page_data) {
        printk(KERN_ERR "Failed to map page\n");
        unlock_page(page);
        put_page(page);
        return;
    }

    printk(KERN_INFO "Data at offset %lld: %s\n", offset, (char *)page_data);

    kunmap(page);

    unlock_page(page);
    put_page(page);
}

static int __init my_module_init(void) {
    struct address_space *mapping = NULL;  // Replace with a valid address space mapping
    loff_t offset = 4096;  // Example: Replace with a valid offset

    printk(KERN_INFO "Page cache example module loaded\n");
    // Call the example function
    example_page_cache_usage(mapping, offset);
    return 0;
}

static void __exit my_module_exit(void) {
    printk(KERN_INFO "Page cache example module unloaded\n");
}

module_init(my_module_init);
module_exit(my_module_exit);

