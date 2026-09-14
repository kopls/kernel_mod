#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/kernel.h>
#include <linux/module.h>

#include "sync.h"

struct sync_ctx *ctx;

static int __init my_module_init(void)
{
    int rc = 0;
    ctx = kcalloc(1, sizeof(struct sync_ctx), GFP_KERNEL);
    if (!ctx)
        return -ENOMEM;

    pr_info("Module install counter\n");
    return rc;
}

static void __exit my_module_exit(void)
{
    kfree(ctx);
    pr_info("Module uninstall counter\n");
}

module_init(my_module_init);
module_exit(my_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("mmm");
MODULE_DESCRIPTION("kernel ");
MODULE_VERSION("1.0");
