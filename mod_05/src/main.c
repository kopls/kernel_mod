#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/kernel.h>
#include <linux/module.h>

#include "kernel_stack.h"

static struct kernel_stack ks;

static int __init my_module_init() 
{
    int rc = 0;

    rc = stack_init(&ks.stack);
    if (rc)
    {
        pr_err("Error stack init\n");
    }
    else
    {
        rc = sysfs_init(&ks);
        if (!rc)
        {
            pr_info("Stack module was install!\n");
        }
    }

    return rc;
}

static void __exit my_module_exit() 
{
    int rc = 0;

    rc = exit_sysfs(&ks);
    if (rc)
    {
        pr_err("err in exit sysfs\n");
    }
    else
    {
        rc = stack_clear(&ks.stack);
        if (rc)
            pr_err("Err clear stack\n");
        else
            pr_info("Stack module was uninstall!\n");
    }
}

module_init(my_module_init);
module_exit(my_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Me");
MODULE_DESC("Kernel stack module");
MODULE_VERSION("1337.0");
