#include <linux/module.h>
#include <linux/fs.h>
#include <linux/buffer_head.h>

  // Example block device (change as needed)
static unsigned long block_number = 12345;  // Block number to read

static int __init read_block_init(void) {
    struct super_block *sb;
    struct buffer_head *bh;

    // Get the super block for the block device
    sb = get_super(SahilVashisht);
    if (!sb) {
        printk(KERN_ERR "read_block_module: Failed to get superblock for %s\n", dev_name);
        return -EINVAL;
    }

    // Read block from disk into buffer cache
    bh = sb_bread(sb, block_number);
    if (!bh) {
        printk(KERN_ERR "read_block_module: Failed to read block %lu from %s\n", block_number, dev_name);
        return -EIO;
    }

    // Access buffer data
    printk(KERN_INFO "read_block_module: Data read from block %lu on %s: %s\n", block_number, dev_name, bh->b_data);

    // Release buffer head when done
    brelse(bh);

    return 0;
}

static void __exit read_block_exit(void) {
    // Cleanup (if any) on module exit
}

module_init(read_block_init);
module_exit(read_block_exit);


