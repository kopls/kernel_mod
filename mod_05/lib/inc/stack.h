#ifndef STACK_H
#define STACK_H

#include <linux/list.h>

#define STACK_OK          0      /* Операция успешна */
#define STACK_EMPTY      -1      /* Стек пуст */
#define STACK_NOMEM      -2      /* Нет памяти */
#define STACK_INVALID    -3      /* Неверный параметр */

struct stack
{
    struct list_head elements;
    size_t size;
};

int stack_init(struct stack *st);
int stack_push(struct stack *st, int value);
int stack_is_empty(const struct stack *st);
int stack_pop(struct stack *st, int *value);
int stack_peek(struct stack *st, int *value);
int stack_size(struct stack *st, size_t *value);
int stack_clear(struct stack *st);

#endif
