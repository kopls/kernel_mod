#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#define OK 0

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

static int int_val;


static int get_int_val(char *val, const struct kernel_param *kp)
{
    return sprintf(val, "%d\n", int_val);
}

static int set_int_val(const char *val, const struct kernel_param *kp)
{
    int rc;
    rc = kstrtoint(val, 10, &int_val);
    if (rc)
    {
        pr_err("Err strtoint\n");
        return -EINVAL;
    }

    pr_info("New param = %d\n", int_val);

    return OK;
}

static const struct kernel_param_ops int_val_params =
{
    .get = get_int_val,
    .set = set_int_val
};

module_param_cb(int_val, &int_val_params, &int_val, 0644);
MODULE_PARM_DESC(int_value, "Param to change from sysfs");

static int __init hello_init(void)
{
    pr_info("Hello world from kernel\n");
    pr_info("Param = %d\n", int_val);
    return 0;
}

static void __exit hello_exit(void)
{
    pr_info("Goodbye kernel\n");
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Kopls");
MODULE_DESCRIPTION("Module hw");
MODULE_VERSION("1.0");
