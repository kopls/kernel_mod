// SPDX-License-Identifier: GPL-2.0
/*
 * strutil_module.c — основной модуль ядра.
 *
 * Этот файл компилируется в strutil_module.ko.
 * Тесты находятся в ОТДЕЛЬНОМ модуле strutil_test.ko —
 * они не включены в этот файл вообще.
 *
 * Порядок работы:
 *   sudo insmod strutil_module.ko   ← основная логика
 *   sudo insmod strutil_test.ko     ← тесты запускаются при загрузке
 *   sudo rmmod strutil_test
 *   sudo rmmod strutil_module
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/printk.h>
#include "strutil.h"

static int __init strutil_init(void)
{
	pr_info("strutil: module loaded, 'racecar' palindrome=%d\n",
		strutil_is_palindrome("racecar", 7));
	return 0;
}

static void __exit strutil_exit(void)
{
	pr_info("strutil: module unloaded\n");
}

module_init(strutil_init);
module_exit(strutil_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("KUnit Lecture Example");
MODULE_DESCRIPTION("Основной модуль strutil (без тестов — сценарий 2)");
