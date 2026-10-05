#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/kernel.h>
#include <linux/module.h>

static int __init my_module_init(void)
{
    int rc = 0;
    pr_info("Device install\n");
    return rc;
}

static void __exit my_module_exit(void)
{
    pr_info("Device uninstall\n");
}

module_init(my_module_init);
module_exit(my_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("mmm");
MODULE_DESCRIPTION("Symbolic device");
MODULE_VERSION("1.0");
