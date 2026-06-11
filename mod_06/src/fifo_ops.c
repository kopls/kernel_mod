#include <linux/slab.h>
#include <linux/errno.h>

#include "kernel_fifo.h"

int fifo_init(struct kernel_fifo *queue, size_t size)
{
    int rc = 0;

    if (!queue)
    {
        rc = -EINVAL;
    }
    else
    {
        if (size <= 0)
        {
            rc = -EINVAL;
        }
        else
        {
            /* чета лень проверку на ->queue уже делать, пускай падает. Все из-за struct kfifo*  */
            rc = kfifo_alloc(&queue->queue, size, GFP_KERNEL);
            queue->max_size = size;
        }
    }

    return rc;
}

int fifo_enqueue(struct kernel_fifo *queue, int value)
{
    int rc = 0;

    if (!queue)
    {
        rc = -EINVAL;
    }
    else
    {
        struct fifo_entry cell;
        cell.data = value;

        rc = kfifo_put(&queue->queue, cell);
        if (rc == 1)
            rc = 0;
        else
            rc = -ENOSPC;
    }

    return rc;
}

int fifo_is_empty(const struct kernel_fifo *queue)
{
    return kfifo_is_empty(&queue->queue);
}

int fifo_is_full(const struct kernel_fifo *queue)
{
    return kfifo_is_full(&queue->queue);
}

size_t fifo_available(const struct kernel_fifo *queue)
{
    return kfifo_avail(&queue->queue);
}

int fifo_dequeue(struct kernel_fifo *queue, int *value)
{
    int rc = 0;

    if (!queue || !value)
    {
        rc = -EINVAL;
    }
    else
    {
        if (fifo_is_empty(queue))
        {
            rc = -ENOENT;
        }
        else
        {
            struct fifo_entry cell;
            rc = kfifo_get(&queue->queue, &cell);
            *value = cell.data;

            if (rc == 1)
                rc = 0;
        }
    }

    return rc;
}

int fifo_peek(struct kernel_fifo *queue, int *value)
{
    int rc = 0;

    if (!queue || !value)
    {
        rc = -EINVAL;
    }
    else
    {
        if (fifo_is_empty(queue))
        {
            rc = -ENOENT;
        }
        else
        {
            struct fifo_entry cell;
            rc = kfifo_peek(&queue->queue, &cell);
            *value = cell.data;

            if (rc == 1)
                rc = 0;
        }
    }

    return rc;
}

int fifo_size(struct kernel_fifo *queue, size_t *value)
{
    int rc = 0;

    if (!queue || !value)
    {
        rc = -EINVAL;
    }
    else
    {
        *value = kfifo_size(&queue->queue);
    }

    return rc;
}

int fifo_clear(struct kernel_fifo *queue)
{
    int rc = 0;

    if (!queue)
    {
        rc = -EINVAL;
    }
    else
    {
        kfifo_reset(&queue->queue);
    }

    return rc;
}
