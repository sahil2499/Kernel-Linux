#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/syscalls.h>

// Define the system call handler
asmlinkage long my_syscall_handler(int a, int b) {
    printk(KERN_INFO "my_syscall_handler: a=%d, b=%d\n", a, b);
    return a + b;
}

// Function to modify the syscall table
static int modify_syscall_table(void) {
    unsigned long **syscall_table;
    unsigned long original_syscall;
    
    // Access the syscall table address (not recommended for production use)
    syscall_table = (unsigned long **)kallsyms_lookup_name("sys_call_table");
    if (!syscall_table) {
        printk(KERN_ERR "Failed to find syscall table address\n");
        return -EFAULT;
    }

    // Save original syscall
    original_syscall = (unsigned long)syscall_table[__NR_my_syscall];

    // Replace syscall with custom handler
    write_cr0(read_cr0() & (~0x10000)); // Disable write protection
    syscall_table[__NR_my_syscall] = (unsigned long *)my_syscall_handler;
    write_cr0(read_cr0() | 0x10000); // Enable write protection

    return 0;
}

// Module initialization function
static int __init my_syscall_init(void) {
    printk(KERN_INFO "my_syscall: Installing system call\n");

    if (modify_syscall_table() != 0) {
        printk(KERN_ERR "my_syscall: Failed to modify syscall table\n");
        return -EFAULT; // Return an error code
    }

    return 0; // Success
}

// Module exit function
static void __exit my_syscall_exit(void) {
    unsigned long **syscall_table;
    
    printk(KERN_INFO "my_syscall: Removing system call\n");

    // Access the syscall table address (not recommended for production use)
    syscall_table = (unsigned long **)kallsyms_lookup_name("sys_call_table");
    if (!syscall_table) {
        printk(KERN_ERR "Failed to find syscall table address\n");
        return;
    }

    // Restore original syscall
    write_cr0(read_cr0() & (~0x10000)); // Disable write protection
    syscall_table[__NR_my_syscall] = (unsigned long *)original_syscall;
    write_cr0(read_cr0() | 0x10000); // Enable write protection
}

module_init(my_syscall_init);
module_exit(my_syscall_exit);

