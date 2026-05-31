#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#define OK 0

#include <linux/kernel.h>
#include <linux/module.h>

static int __init hello_init(void) {
  pr_info("Hello, World from the kernel!\n");
  return OK;
}

static void __exit hello_exit(void) {
  pr_info("Goodbye, World from the kernel!\n");
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Kopls");
MODULE_DESCRIPTION("Hello world module first");
