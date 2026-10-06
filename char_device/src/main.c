#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/cdev.h>

#define DEVICE_NAME "my_device"
#define CLASS_NAME "my_device_class"

static dev_t dev_num;
static struct cdev my_cdev;
static struct class *my_class;
static struct device *my_device;

static const struct file_operations my_fops = {
    .owner = THIS_MODULE,
};

static int __init my_device_init(void)
{
    int rc = 0;

    rc = alloc_chrdev_region(&dev_num, 0, 1, DEVICE_NAME);
    if (rc < 0)
    {
        pr_err("failed to allocate device number\n");
        return rc;
    }

    pr_info("major=%d minor=%d\n", MAJOR(dev_num), MINOR(dev_num));

    cdev_init(&my_cdev, &my_fops);
    my_cdev.owner = THIS_MODULE;

    rc = cdev_add(&my_cdev, dev_num, 1);
    if (rc < 0)
    {
        pr_err("failed to add cdev\n");
        goto unregister_region;
    }

    my_class = class_create(CLASS_NAME);
    if (IS_ERR(my_class))
    {
        rc = PTR_ERR(my_class);
        pr_err("failed to create class\n");
        goto delete_cdev;
    }

    my_device = device_create(
        my_class,
        NULL,
        dev_num,
        NULL,
        DEVICE_NAME
    );

    if (IS_ERR(my_device))
    {
        rc = PTR_ERR(my_device);
        pr_err("failed to create device\n");
        goto destroy_class;
    }

    pr_info("loaded\n");

    return 0;

destroy_class:
    class_destroy(my_class);


delete_cdev:
    cdev_del(&my_cdev);

unregister_region:
    unregister_chrdev_region(dev_num, 1);

    return rc;
}

static void __exit my_device_exit(void)
{
    device_destroy(my_class, dev_num);
    class_destroy(my_class);

    cdev_del(&my_cdev);
    unregister_chrdev_region(dev_num, 1);

    pr_info("unloaded\n");
}

module_init(my_device_init);
module_exit(my_device_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("mmm");
MODULE_DESCRIPTION("Symbolic device");
MODULE_VERSION("1.0");
