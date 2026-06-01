#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#define OK 0
#define LEN_ARR 14 + 1 // под фразу + \n

#define MIN_IDX 0
#define MAX_IDX 13

#define MAX_CHAR_VALUE 127
#define MIN_CHAR_VALUE 32

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/moduleparam.h>

static int idx;
static char ch_val;
static unsigned char my_arr[LEN_ARR]; 

static int idx_set(const char *val, const struct kernel_param *kp)
{
    int rc = OK;
    int new_idx;

    if (!val || !kp)
    {
        pr_err("Error in param!\n");
        rc = -EINVAL;
    }
    else
    {
        rc = kstrtoint(val, 10, &new_idx);
        if (rc)
        {
            pr_err("Error in set idx!\n");
        }
        else if (new_idx < MIN_IDX || new_idx > MAX_IDX)
        {
            pr_err("Error in range idx!\n");
            rc = -EINVAL;
        }
        else
        {
            idx = new_idx;
            pr_info("New idx: %d\n", idx);
        }
    }

    return rc;
}

static int idx_get(char *buffer, const struct kernel_param *kp)
{
    int rc = OK;

    if (!buffer || !kp)
    {
        pr_err("Error in param idx_get!\n");
        rc = -EINVAL;
    }
    else
    {
        rc =  param_get_int(buffer, kp);   
    }

    return rc;
}

static int ch_val_set(const char *val, const struct kernel_param *kp)
{
    int rc = OK;
    int new_ch_val;

    if (!val || !kp)
    {
        pr_err("Error in param!\n");
        rc = -EINVAL;
    }
    else 
    {
        rc = kstrtoint(val, 10, &new_ch_val);
        if (rc)
        {
            pr_err("Error in set char!\n");  
        }
        else if (new_ch_val < MIN_CHAR_VALUE || new_ch_val > MAX_CHAR_VALUE)
        {
            pr_err("Error in set char value!\n");
            rc = -EINVAL;
        }
        else
        {
            ch_val = new_ch_val;
            pr_info("New ch_val: %c\n", new_ch_val);

            my_arr[idx] = ch_val;
            pr_info("New str: %s\n", my_arr);
        }
    }

    return rc;
}

static int my_arr_get(char *buffer, const struct kernel_param *kp)
{
    int rc = OK;

    if (!buffer || !kp)
    {
        pr_err("Error in param my_arr_get!\n");
        rc = -EINVAL;
    }
    else
    {
        int len = strnlen(my_arr, ARRAY_SIZE(my_arr));

        if (!memcpy(buffer, my_arr, len))
        {
            pr_err("Error in memcpy!\n");
            rc = -EFAULT;
        }
        else
        {
            buffer[len] = '\n';
            rc = len + 1;
        }
    }

    return rc;
}

static int my_arr_set(const char *val, const struct kernel_param *kp)
{
    int rc = OK;

    if (!val || !kp)
        rc = -EINVAL;

    return rc;
}

static int ch_val_get(char *buffer, const struct kernel_param *kp)
{
    int rc = OK;

    if (!buffer || !kp)
    {
        rc = -EINVAL;
    }
    else
    {
        rc = param_get_byte(buffer, kp);   
    }

    return rc;
}

static const struct kernel_param_ops idx_ops = {
    .set = idx_set,
    .get = idx_get
};

static const struct kernel_param_ops ch_val_ops = {
    .set = ch_val_set,
    .get = ch_val_get
};

static const struct kernel_param_ops my_arr_ops = {
    .set = my_arr_set,
    .get = my_arr_get
};

module_param_cb(idx, &idx_ops, &idx, 0644); 
MODULE_PARM_DESC(idx, "Idx for enter to arr");

module_param_cb(ch_val, &ch_val_ops, &ch_val, 0644); 
MODULE_PARM_DESC(ch_val, "Value for enter to arr");

module_param_cb(my_arr, &my_arr_ops, &my_arr, S_IRUSR | S_IRGRP | S_IROTH);
MODULE_PARM_DESC(my_arr, "Arr for message");

static int __init my_module_init(void)
{
    pr_info("Module install: Hello, world! From kernel\n");
    pr_info("Idx: %d\n", idx);
    pr_info("ch_val: %d\n", ch_val);
    pr_info("Str: %s\n", my_arr);
    return OK;
}

static void __exit my_module_exit(void)
{
    pr_info("Module uninstall: Goodbye, world! From kernel\n");
}

module_init(my_module_init);
module_exit(my_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Artyom");
MODULE_DESCRIPTION("Kernel module with param");
MODULE_VERSION("1.0");
