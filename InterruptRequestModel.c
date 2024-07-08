#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/errno.h>

struct my_device_data {
    int device_id;
};

static int irq_number = 1;
static struct my_device_data device_info = {
    .device_id = 123
};

static irqreturn_t my_interrupt_handler(int irq, void *dev_id) {
    struct my_device_data *data = (struct my_device_data *)dev_id;
    printk(KERN_INFO "Handling interrupt for device ID: %d\n", data->device_id);
    return IRQ_HANDLED;
}

static int __init my_interrupt_module_init(void) {
    int result;
    result = request_irq(irq_number, my_interrupt_handler, IRQF_SHARED, "my_device", &device_info);
    if (result) {
        printk(KERN_ERR "Failed to register IRQ handler\n");
        return result;
    }
    printk(KERN_INFO "Interrupt handler registered successfully\n");
    return 0;
}

static void __exit my_interrupt_module_exit(void) {
    free_irq(irq_number, &device_info);
    printk(KERN_INFO "Interrupt handler unregistered\n");
}

module_init(my_interrupt_module_init);
module_exit(my_interrupt_module_exit);

