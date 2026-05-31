#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#define OK 0

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/stat.h>
#include <linux/errno.h>

int param;

static int param_set(const char *val, const struct kernel_param *kp)
{
    int res;
    int _val;

    res = kstrtoint(val, 10, &_val);
    if (res)
    {
        pr_err("Err strto int val\n");
        return -EINVAL;
    }

    if (_val < 0 || _val > 100)
    {
        pr_err("Err fence\n");
        return -EINVAL;
    }

    param = _val;

    pr_info("New param - %d\n", param);
    return OK;
}

static const struct kernel_param_ops param_ops = {
    .set = param_set,
    .get = param_get_int,
};

module_param_cb(param, &param_ops, &param, 0644);
MODULE_PARM_DESC(param, "param with callback (0 - 100)\n");

static int __init mod_init(void)
{
    pr_info("Модуль загружен, param = %d\n", param);
    return 0;
}

static void __exit mod_exit(void)
{
    pr_info("Модуль выгружен, param = %d\n", param);
}

module_init(mod_init);
module_exit(mod_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Kopls");
MODULE_DESCRIPTION("Param callback module kernel");
