#ifndef KERNEL_STACK_H
#define KERNEL_STACK_H

#include <linux/mutex.h>
#include <linux/kobject.h>

#include "stack.h"

struct kernel_stack
{
    struct stack stack;
    struct kobject *kobj;
    struct mutex lock;
};

int init_sysfs(struct kernel_stack *ks);
int exit_sysfs(struct kernel_stack *ks);

#endif
