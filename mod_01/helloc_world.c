#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt
#define OK 0

#include <linux/module.h>
#include <linux/kernel.h>

static int __init hello_init(void)
{
    pr_info("Hello, World from the kernel!\n");
    return OK;
}

static int __exit hello_exit(void)
{
    pr_info("Goodbye, World from the kernel!\n");
    return OK;
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("ESHKERE");
MODULE_AUTHOR("Kopls");
MODULE_DESCRIOTION("Hello world module first");
