// SPDX-License-Identifier: GPL-2.0
/*
 * mymath_test.c — KUnit-тесты для mymath.h
 *
 * Сценарий: тесты ВСТРОЕНЫ в модуль и запускаются автоматически
 * в хуке module_init() ДО основной логики модуля.
 *
 * Сборка с CONFIG_KUNIT=y (или =m) и CONFIG_MYMATH_TEST=y.
 * Запуск: modprobe kunit_builtin_example
 *         dmesg | grep -E "kunit|PASS|FAIL"
 */

#include <kunit/test.h>
#include "mymath.h"

/* ------------------------------------------------------------------ */
/*  Тест-кейсы                                                          */
/* ------------------------------------------------------------------ */

static void mymath_test_add_basic(struct kunit *test)
{
	KUNIT_EXPECT_EQ(test, mymath_add(2, 3), 5);
	KUNIT_EXPECT_EQ(test, mymath_add(0, 0), 0);
	KUNIT_EXPECT_EQ(test, mymath_add(-1, 1), 0);
}

static void mymath_test_add_overflow_boundary(struct kunit *test)
{
	/* Проверяем поведение вблизи INT_MAX */
	KUNIT_EXPECT_EQ(test, mymath_add(INT_MAX, 0), INT_MAX);
}

static void mymath_test_sub_basic(struct kunit *test)
{
	KUNIT_EXPECT_EQ(test, mymath_sub(10, 4), 6);
	KUNIT_EXPECT_EQ(test, mymath_sub(0, 0), 0);
	KUNIT_EXPECT_EQ(test, mymath_sub(-3, -3), 0);
}

static void mymath_test_mul_basic(struct kunit *test)
{
	KUNIT_EXPECT_EQ(test, mymath_mul(3, 4), 12);
	KUNIT_EXPECT_EQ(test, mymath_mul(0, 999), 0);
	KUNIT_EXPECT_EQ(test, mymath_mul(-2, 5), -10);
}

static void mymath_test_clamp_inside(struct kunit *test)
{
	KUNIT_EXPECT_EQ(test, mymath_clamp(5, 1, 10), 5);
}

static void mymath_test_clamp_below(struct kunit *test)
{
	KUNIT_EXPECT_EQ(test, mymath_clamp(-5, 0, 10), 0);
}

static void mymath_test_clamp_above(struct kunit *test)
{
	KUNIT_EXPECT_EQ(test, mymath_clamp(20, 0, 10), 10);
}

/* ------------------------------------------------------------------ */
/*  Тест-сьют                                                           */
/* ------------------------------------------------------------------ */

static struct kunit_case mymath_test_cases[] = {
	KUNIT_CASE(mymath_test_add_basic),
	KUNIT_CASE(mymath_test_add_overflow_boundary),
	KUNIT_CASE(mymath_test_sub_basic),
	KUNIT_CASE(mymath_test_mul_basic),
	KUNIT_CASE(mymath_test_clamp_inside),
	KUNIT_CASE(mymath_test_clamp_below),
	KUNIT_CASE(mymath_test_clamp_above),
	{} /* sentinel */
};

static struct kunit_suite mymath_test_suite = {
	.name  = "mymath",
	.test_cases = mymath_test_cases,
};

/* ------------------------------------------------------------------ */
/*  Регистрация — макрос kunit_test_suite() генерирует __init/__exit   */
/* ------------------------------------------------------------------ */
kunit_test_suite(mymath_test_suite);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("KUnit Lecture Example");
MODULE_DESCRIPTION("Встроенные KUnit-тесты для mymath (сценарий 1)");
