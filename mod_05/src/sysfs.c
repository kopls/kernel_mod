#include <linux/kobject.h>
#include <linux/sysfs.h>
#include <linux/errno.h>
#include <linux/kernel.h>

#include "kernel_stack.h"

static struct kernel_stack *global_ks;

static ssize_t push_store(struct kobject *kobj, 
                   struct kobj_attribute *attr, 
                   const char *buf, size_t count)
{
    int rc = 0;

    if (!kobj || !attr || !buf)
    {
        pr_err("Err inval args\n");
        rc = -EINVAL;
    }
    else
    {
        int num;

        rc = kstrtoint(buf, 10, &num);
        if (rc)
        {
            pr_err("Err str to int in push\n");
        }
        else
        {
            rc = stack_push(&global_ks->stack, num);
            if (rc)
            {
                pr_err("Error stack push\n");
            }
        }
    }

    if (rc)
        return rc;
    return count;
}

static ssize_t pop_show(struct kobject *kobj, 
                   struct kobj_attribute *attr, 
                   char *buf)
{
    int rc = 0;

    ssize_t count;

    if (!kobj || !attr || !buf)
    {
        pr_err("Err inval args\n");
        rc = -EINVAL;
    }
    else
    {
        int num;

        rc = stack_pop(&global_ks->stack, &num);
        if (rc)
        {
            pr_err("Nothing to pop\n");
        }
        else
        {
            count = sysfs_emit(buf, "%d\n", num);       
        }
    }

    if (rc)
        return rc;
    return count;
}

static ssize_t peek_show(struct kobject *kobj,
                         struct kobj_attribute *attr,
                         char *buf)
{
    int rc = 0;
    int num;

    if (!kobj || !attr || !buf)
    {
        rc = -EINVAL;
        pr_err("Err inval args\n");
    }
    else
    {
        rc = stack_peek(&global_ks->stack, &num);
        if (rc)
        {
            pr_err("Nothing in stack\n");
        }
        else
        {
            return sysfs_emit(buf, "%d\n", num);
        }
    }

    return rc;
}

static ssize_t size_show(struct kobject *kobj,
                         struct kobj_attribute *attr,
                         char *buf)
{
    int rc = 0;
    size_t size;

    if (!kobj || !attr || !buf)
    {
        rc = -EINVAL;
        pr_err("Err inval args\n");
    }
    else
    {
        rc = stack_size(&global_ks->stack, &size);
        if (rc)
        {
            pr_err("Err in show size\n");
        }
        else
        {
            return sysfs_emit(buf, "%lu\n", size);
        }
    }

    return rc;
}

static ssize_t is_empty_show(struct kobject *kobj,
                         struct kobj_attribute *attr,
                         char *buf)
{
    int rc = 0;

    if (!kobj || !attr || !buf)
    {
        rc = -EINVAL;
        pr_err("Err inval args\n");
    }
    else
    {
        if (stack_is_empty(&global_ks->stack))
        {
            return sysfs_emit(buf, "%d\n", 1);
        }
        else
        {
            return sysfs_emit(buf, "%d\n", 0);
        }
    }

    return rc;
}

static ssize_t clear_store(struct kobject *kobj, 
                   struct kobj_attribute *attr, 
                   const char *buf, size_t count)
{
    int rc = 0;

    if (!kobj || !attr || !buf)
    {
        pr_err("Err inval args\n");
        rc = -EINVAL;
    }
    else
    {
        int num;

        rc = kstrtoint(buf, 10, &num);
        if (rc)
        {
            pr_err("Err str to int in clear\n");
        }
        else
        {
            if (num == 1)
            {
                rc = stack_clear(&global_ks->stack);
                if (rc)
                {
                    pr_err("Error stack clear\n");
                }
            }
            else
            {
                rc = -EINVAL;
            }
        }
    }

    if (rc)
        return rc;
    return count;
}

static struct kobj_attribute push_attr =
              __ATTR(push, 0220, NULL, push_store);

static struct kobj_attribute pop_attr =
              __ATTR(pop, 0444, pop_show, NULL);

static struct kobj_attribute peek_attr =
              __ATTR(peek, 0444, peek_show, NULL);

static struct kobj_attribute size_attr =
              __ATTR(size, 0444, size_show, NULL);

static struct kobj_attribute is_empty_attr =
              __ATTR(is_empty, 0444, is_empty_show, NULL);

static struct kobj_attribute clear_attr =
              __ATTR(clear, 0220, NULL, clear_store);




static struct attribute *stack_attrs[] = {
    &push_attr.attr,
    &pop_attr.attr,
    &peek_attr.attr,
    &size_attr.attr,
    &is_empty_attr.attr,
    &clear_attr.attr,
    NULL
};

static const struct attribute_group stack_group = {
    .attrs = stack_attrs,
};

int init_sysfs(struct kernel_stack *ks)
{
    int rc = 0;

    if (!ks)
    {
        rc = -EINVAL;
        pr_err("Err inval in init_sysfs\n");
    }
    else
    {
        ks->kobj = kobject_create_and_add("kernel_stack", kernel_kobj);
        if (!ks->kobj)
        {
            rc = -ENOMEM;
            pr_err("No mem for kobj\n");
        }
        else
        {
            global_ks = ks;
            rc = sysfs_create_group(ks->kobj, &stack_group);
            if (rc)
            {
                pr_err("Err create group\n");
                kobject_put(ks->kobj);
                ks->kobj = NULL;
                global_ks = NULL;
            }
        }
    }
    return rc;
}

int exit_sysfs(struct kernel_stack *ks)
{
    int rc = 0;

    if (!ks || !ks->kobj)
    {
        rc = -EINVAL;
    }
    else
    {
        sysfs_remove_group(ks->kobj, &stack_group);
        kobject_put(ks->kobj);
        ks->kobj = NULL;
        global_ks = NULL;
    }

    return rc;
}
