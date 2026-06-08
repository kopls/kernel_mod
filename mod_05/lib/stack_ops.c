#include <linux/slab.h>
#include <linux/list.h>
#include <linux/errno.h>

#include "stack.h"

struct stack_entry
{
    struct list_head list;
    int data;
};

int stack_init(struct stack *st)
{
    int rc = 0;

    if (!st)
    {
        rc = -EINVAL;
    }
    else
    {
        INIT_LIST_HEAD(&st->elements);
        st->size = 0;
    }

    return rc;
}

int stack_push(struct stack *st, int value)
{
    int rc = 0;

    if (!st)
    {
        rc = -EINVAL;
    }
    else
    {
        struct stack_entry *node = kmalloc(sizeof(struct stack_entry), GFP_KERNEL);
        if (!node)
        {
            rc = STACK_NOMEM;
        }
        else
        {
            node->data = value;
            list_add(&node->list, &st->elements);
            st->size++;
        }
    }

    return rc;
}

int stack_is_empty(const struct stack *st)
{
    return list_empty(&st->elements);
}

int stack_pop(struct stack *st, int *value)
{
    int rc = 0;

    if (!st || !value)
    {
        rc = -EINVAL;
    }
    else
    {
        if (stack_is_empty(st))
        {
            rc = STACK_EMPTY;
        }
        else
        {
            struct stack_entry *p = container_of(st->elements.next, struct stack_entry, list);
            int data = p->data;
            list_del(st->elements.next);
            *value = data;
            st->size--;
            kfree(p);
        }
    }

    return rc;
}

int stack_peek(struct stack *st, int *value)
{
    int rc = 0;

    if (!st || !value)
    {
        rc = -EINVAL;
    }
    else
    {
        if (stack_is_empty(st))
        {
            rc = STACK_EMPTY;
        }
        else
        {
            struct stack_entry *p = container_of(st->elements.next, struct stack_entry, list);
            *value = p->data;
        }
    }

    return rc;
}

int stack_size(struct stack *st, size_t *value)
{
    int rc = 0;

    if (!st || !value)
    {
        rc = -EINVAL;
    }
    else
    {
        *value = st->size;
    }

    return rc;
}

int stack_clear(struct stack *st)
{
    int rc = 0;

    if (!st)
    {
        rc = -EINVAL;
    }
    else
    {
        int data;
        while (!stack_is_empty(st))
        {
            rc = stack_pop(st, &data);
            if (rc)
            {
                break;
            }
        }
    }

    return rc;
}
