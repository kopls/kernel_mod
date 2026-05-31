#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#define OK 0

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/stat.h>

short int myshort = 2;
int myint = 52;
long mylong = 1338;
char *mystr = "qweqwe";
int myintarr[2] = {1, 2};
int arr_argc;

module_param(myshort, short, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
MODULE_PARM_DESC(myshort, "A short int");

module_param(myint, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
MODULE_PARM_DESC(myint, "A int");

module_param(mylong, long, S_IRUSR);
MODULE_PARM_DESC(mylong, "A long");

module_param(mystr, charp, 0000);
MODULE_PARM_DESC(mystr, "A str");

module_param_array(myintarr, int, &arr_argc, 0000);
MODULE_PARM_DESC(myintarr, "A arr int");

static int __init hello_param_init(void)
{
    pr_info("Hello world!");

    pr_info("my short: %hd!\n", myshort);
    pr_info("my int: %d!\n", myint);
    pr_info("my long: %ld!\n", mylong);
    pr_info("my str: %s!\n", mystr);

    pr_info("My arr int : ");
    for (int i = 0; i < ARRAY_SIZE(myintarr); ++i)
        pr_info("%d ", myintarr[i]);
    pr_info("end of arr");

    pr_info("arr szie: %ld\n", ARRAY_SIZE(myintarr));

    return OK;
}

static void __exit hello_param_exit(void)
{
    pr_info("Goodby, kernel world\n");
}

module_init(hello_param_init);
module_exit(hello_param_exit);

MODULE_LICENSE("GAYPL");
MODULE_AUTHOR("Kopls");
MODULE_DESCRIPTION("Module with param");
