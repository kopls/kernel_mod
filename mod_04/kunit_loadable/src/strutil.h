/* SPDX-License-Identifier: GPL-2.0 */
/*
 * strutil.h — минимальные строковые утилиты для ядра.
 * «Объект под тестом» в примере kunit_loadable.
 */
#ifndef _STRUTIL_H
#define _STRUTIL_H

#include <linux/string.h>
#include <linux/ctype.h>

/**
 * strutil_count_char() - считает вхождения символа @c в строку @s
 * @s: указатель на строку (не NULL)
 * @c: искомый символ
 *
 * Return: количество вхождений @c в @s
 */
static inline int strutil_count_char(const char *s, char c)
{
	int count = 0;

	while (*s) {
		if (*s == c)
			count++;
		s++;
	}
	return count;
}

/**
 * strutil_is_palindrome() - проверяет, является ли строка палиндромом
 * @s:  указатель на строку (не NULL)
 * @len: длина строки
 *
 * Return: 1 — палиндром, 0 — нет
 */
static inline int strutil_is_palindrome(const char *s, size_t len)
{
	size_t i;

	if (len == 0)
		return 1;
	for (i = 0; i < len / 2; i++) {
		if (s[i] != s[len - 1 - i])
			return 0;
	}
	return 1;
}

/**
 * strutil_to_upper_char() - конвертирует символ в верхний регистр
 * @c: исходный символ
 *
 * Return: символ в верхнем регистре
 */
static inline char strutil_to_upper_char(char c)
{
	return (char)toupper((unsigned char)c);
}

#endif /* _STRUTIL_H */
