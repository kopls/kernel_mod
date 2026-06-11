#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/kernel.h>
#include <linux/module.h>

#include "kernel_fifo.h"

#define FIFO_SIZE 32

static struct kernel_fifo queue;

static int __init my_module_init(void) 
{
    int rc = 0;

    rc = fifo_init(&queue, FIFO_SIZE);
    if (rc)
    {
        pr_err("Error stack init\n");
    }
    else
    {
        rc = init_sysfs(&queue);
        if (!rc)
        {
            pr_info("Stack module was install\n");
        }
    }

    return rc;
}

static void __exit my_module_exit(void) 
{
    int rc = 0;

    rc = exit_sysfs(&queue);
    if (rc)
    {
        pr_err("err in exit sysfs\n");
    }
    else
    {
        kfifo_free(&queue.queue);
        pr_info("Stack module was uninstall!\n");
    }
}

module_init(my_module_init);
module_exit(my_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Me");
MODULE_DESCRIPTION("Kernel fifo module");
MODULE_VERSION("1337.0");
