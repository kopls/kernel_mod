// замена макроса на строку, которая позднее склеится, вызывается в pr_info()
#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/kernel.h> // логирование, строки
#include <linux/module.h> // инициализация, информация о модуле, параметры модуля

#include "allocator.h"

static int __init my_module_init(void)
{
    int rc = ERR_OK;
    rc = allocator_init();
    if (rc)
    {
        pr_err("Error in allocator init\n");
    }
    else
    {
        pr_info("Module install allocator\n");
    }

    return rc;
}

static void __exit my_module_exit(void)
{
    allocator_destroy();
    pr_info("Module uninstall allocator\n");
}

module_init(my_module_init);
module_exit(my_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("mmm");
MODULE_DESCRIPTION("kernel allocator module");
MODULE_VERSION("1.0");
