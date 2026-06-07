/* SPDX-License-Identifier: GPL-2.0 */
/*
 * mymath.h — простая библиотека арифметических функций.
 * Используется как «объект под тестом» в примере kunit_builtin.
 */
#ifndef _MYMATH_H
#define _MYMATH_H

/**
 * mymath_add() - складывает два целых числа
 * @a: первое слагаемое
 * @b: второе слагаемое
 *
 * Return: сумма @a и @b
 */
static inline int mymath_add(int a, int b)
{
	return a + b;
}

/**
 * mymath_sub() - вычитает @b из @a
 * @a: уменьшаемое
 * @b: вычитаемое
 *
 * Return: разность @a - @b
 */
static inline int mymath_sub(int a, int b)
{
	return a - b;
}

/**
 * mymath_mul() - умножает два целых числа
 * @a: первый множитель
 * @b: второй множитель
 *
 * Return: произведение @a * @b
 */
static inline int mymath_mul(int a, int b)
{
	return a * b;
}

/**
 * mymath_clamp() - ограничивает значение в диапазоне [lo, hi]
 * @val: исходное значение
 * @lo:  нижняя граница
 * @hi:  верхняя граница
 *
 * Return: val, если lo <= val <= hi; иначе lo или hi
 */
static inline int mymath_clamp(int val, int lo, int hi)
{
	if (val < lo)
		return lo;
	if (val > hi)
		return hi;
	return val;
}

#endif /* _MYMATH_H */
