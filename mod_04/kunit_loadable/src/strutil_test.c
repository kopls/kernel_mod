// SPDX-License-Identifier: GPL-2.0
/*
 * strutil_test.c — KUnit-тесты для strutil.h
 *
 * Сценарий: тесты оформлены как ОТДЕЛЬНЫЙ загружаемый модуль.
 * Основной модуль (strutil_module.ko) и тестовый (strutil_test.ko)
 * собираются и загружаются независимо друг от друга.
 *
 * Сборка:
 *   make
 *
 * Запуск тестов:
 *   sudo modprobe kunit          # убедитесь, что KUnit загружен
 *   sudo insmod strutil_test.ko
 *   sudo dmesg | grep -E "kunit|PASS|FAIL|strutil"
 *   sudo rmmod strutil_test
 */

#include <linux/module.h>
#include <kunit/test.h>
#include "strutil.h"

/* ------------------------------------------------------------------ */
/*  Фикстуры (suite init / exit)                                        */
/* ------------------------------------------------------------------ */

static int strutil_suite_init(struct kunit_suite *suite)
{
	/* Здесь можно аллоцировать ресурсы, общие для всех тест-кейсов */
	pr_info("strutil suite: init\n");
	return 0;
}

static void strutil_suite_exit(struct kunit_suite *suite)
{
	pr_info("strutil suite: exit\n");
}

/* ------------------------------------------------------------------ */
/*  Per-test init/exit (демонстрация kunit->priv)                       */
/* ------------------------------------------------------------------ */

struct strutil_test_ctx {
	char buf[64];
};

static int strutil_test_init(struct kunit *test)
{
	struct strutil_test_ctx *ctx;

	ctx = kunit_kzalloc(test, sizeof(*ctx), GFP_KERNEL);
	KUNIT_ASSERT_NOT_ERR_OR_NULL(test, ctx);

	strscpy(ctx->buf, "racecar", sizeof(ctx->buf));
	test->priv = ctx;
	return 0;
}

/* ------------------------------------------------------------------ */
/*  Тест-кейсы                                                          */
/* ------------------------------------------------------------------ */

static void strutil_test_count_char_basic(struct kunit *test)
{
	KUNIT_EXPECT_EQ(test, strutil_count_char("hello world", 'l'), 3);
	KUNIT_EXPECT_EQ(test, strutil_count_char("", 'x'), 0);
	KUNIT_EXPECT_EQ(test, strutil_count_char("aaa", 'a'), 3);
}

static void strutil_test_count_char_no_match(struct kunit *test)
{
	KUNIT_EXPECT_EQ(test, strutil_count_char("linux kernel", 'z'), 0);
}

static void strutil_test_palindrome_true(struct kunit *test)
{
	struct strutil_test_ctx *ctx = test->priv;

	/* ctx->buf содержит "racecar", подготовленный в init */
	KUNIT_EXPECT_EQ(test, strutil_is_palindrome(ctx->buf,
						     strlen(ctx->buf)), 1);
	KUNIT_EXPECT_EQ(test, strutil_is_palindrome("abba", 4), 1);
	KUNIT_EXPECT_EQ(test, strutil_is_palindrome("a", 1), 1);
	/* Пустая строка — тоже палиндром */
	KUNIT_EXPECT_EQ(test, strutil_is_palindrome("", 0), 1);
}

static void strutil_test_palindrome_false(struct kunit *test)
{
	KUNIT_EXPECT_EQ(test, strutil_is_palindrome("linux", 5), 0);
	KUNIT_EXPECT_EQ(test, strutil_is_palindrome("kernel", 6), 0);
}

static void strutil_test_to_upper(struct kunit *test)
{
	KUNIT_EXPECT_EQ(test, strutil_to_upper_char('a'), 'A');
	KUNIT_EXPECT_EQ(test, strutil_to_upper_char('Z'), 'Z');
	KUNIT_EXPECT_EQ(test, strutil_to_upper_char('0'), '0');
}

/* ---------------------------------------------------------------- */
/* Пример теста с ASSERT (прерывает выполнение при провале)          */
/* ---------------------------------------------------------------- */

static void strutil_test_assert_demo(struct kunit *test)
{
	const char *str = "kunit";

	/* ASSERT: если NULL — тест прерывается немедленно */
	KUNIT_ASSERT_NOT_NULL(test, str);

	/* До сюда дойдём только если str != NULL */
	KUNIT_EXPECT_EQ(test, strutil_count_char(str, 'k'), 1);
}

/* ------------------------------------------------------------------ */
/*  Параметризованный тест (демонстрация KUNIT_CASE_PARAM)             */
/* ------------------------------------------------------------------ */

struct palindrome_param {
	const char *str;
	int expected;
};

static const struct palindrome_param palindrome_params[] = {
	{ "madam",  1 },
	{ "noon",   1 },
	{ "hello",  0 },
	{ "world",  0 },
	{ "level",  1 },
};

KUNIT_ARRAY_PARAM_DESC(palindrome, palindrome_params, str);

static void strutil_test_palindrome_param(struct kunit *test)
{
	const struct palindrome_param *p = test->param_value;

	KUNIT_EXPECT_EQ(test, strutil_is_palindrome(p->str, strlen(p->str)),
			p->expected);
}

/* ------------------------------------------------------------------ */
/*  Тест-сьют                                                           */
/* ------------------------------------------------------------------ */

static struct kunit_case strutil_test_cases[] = {
	KUNIT_CASE(strutil_test_count_char_basic),
	KUNIT_CASE(strutil_test_count_char_no_match),
	KUNIT_CASE_PARAM(strutil_test_palindrome_param, palindrome_gen_params),
	KUNIT_CASE(strutil_test_palindrome_true),
	KUNIT_CASE(strutil_test_palindrome_false),
	KUNIT_CASE(strutil_test_to_upper),
	KUNIT_CASE(strutil_test_assert_demo),
	{} /* sentinel */
};

static struct kunit_suite strutil_test_suite = {
	.name       = "strutil",
	.suite_init = strutil_suite_init,
	.suite_exit = strutil_suite_exit,
	.init       = strutil_test_init,
	.test_cases = strutil_test_cases,
};

kunit_test_suite(strutil_test_suite);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("KUnit Lecture Example");
MODULE_DESCRIPTION("Отдельный KUnit-модуль для strutil (сценарий 2)");
