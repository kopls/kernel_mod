// SPDX-License-Identifier: GPL-2.0
/*
 * mymath_module.c — основной модуль ядра.
 *
 * KUnit-тесты (mymath_test.c) прилинкованы в тот же .ko и выполняются
 * через механизм kunit_test_suite() до тела mymath_init().
 *
 * Порядок событий при modprobe kunit_builtin_example:
 *   1. kunit_run_tests()  ← автоматически, до mymath_init()
 *   2. mymath_init()      ← наш pr_info()
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/printk.h>
#include "mymath.h"

static int __init mymath_init(void)
{
	pr_info("mymath: module loaded, 2+2=%d\n", mymath_add(2, 2));
	return 0;
}

static void __exit mymath_exit(void)
{
	pr_info("mymath: module unloaded\n");
}

module_init(mymath_init);
module_exit(mymath_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("KUnit Lecture Example");
MODULE_DESCRIPTION("Модуль с встроенными KUnit-тестами (сценарий 1)");
