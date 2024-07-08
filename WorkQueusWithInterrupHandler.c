#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/workqueue.h>

#define IRQ_NUMBER 1  // Example IRQ number (change as per your hardware)

static struct workqueue_struct *my_workqueue;
static struct work_struct my_work;
static int irq_count = 0;

/* Simulated interrupt handler */
static irqreturn_t my_interrupt_handler(int irq, void *dev_id) {
    irq_count++;
    /* Schedule work to be done in workqueue */
    schedule_work(&my_work);
    return IRQ_HANDLED;
}

/* Workqueue handler function */
static void my_work_handler(struct work_struct *work) {
    printk(KERN_INFO "Workqueue task executed. Total interrupts: %d\n", irq_count);
}

static int __init interrupt_workqueue_init(void) {
    /* Initialize workqueue */
    my_workqueue = create_workqueue("my_workqueue");
    if (!my_workqueue) {
        printk(KERN_ERR "Failed to create workqueue\n");
        return -ENOMEM;
    }

    /* Initialize work item */
    INIT_WORK(&my_work, my_work_handler);

    /* Register interrupt handler */
    if (request_irq(IRQ_NUMBER, my_interrupt_handler, IRQF_SHARED, "my_device", &my_work)) {
        printk(KERN_ERR "Failed to register IRQ handler\n");
        destroy_workqueue(my_workqueue);
        return -EBUSY;
    }

    printk(KERN_INFO "Interrupt handler and workqueue initialized successfully\n");
    return 0;
}

static void __exit interrupt_workqueue_exit(void) {
    /* Unregister interrupt handler */
    free_irq(IRQ_NUMBER, &my_work);

    /* Flush and destroy workqueue */
    flush_workqueue(my_workqueue);
    destroy_workqueue(my_workqueue);

    printk(KERN_INFO "Interrupt handler and workqueue cleaned up\n");
}

module_init(interrupt_workqueue_init);
module_exit(interrupt_workqueue_exit);

