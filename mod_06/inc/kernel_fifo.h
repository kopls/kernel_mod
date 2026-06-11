#ifndef KERNEL_FIFO_H
#define KERNEL_FIFO_H

#include <linux/kobject.h>
#include <linux/kfifo.h>

struct fifo_entry
{
    int data;
};

struct kernel_fifo
{

    DECLARE_KFIFO_PTR(queue, struct fifo_entry);
    size_t max_size;
    struct kobject *kobj;
};

int fifo_init(struct kernel_fifo *queue, size_t size);
int fifo_enqueue(struct kernel_fifo *queue, int value);
int fifo_is_empty(const struct kernel_fifo *queue);
int fifo_is_full(const struct kernel_fifo *queue);
size_t fifo_available(const struct kernel_fifo *queue);
int fifo_dequeue(struct kernel_fifo *queue, int *value);
int fifo_peek(struct kernel_fifo *queue, int *value);
int fifo_size(struct kernel_fifo *queue, size_t *value);
int fifo_clear(struct kernel_fifo *queue);

int init_sysfs(struct kernel_fifo *queue);
int exit_sysfs(struct kernel_fifo *queue);

#endif

