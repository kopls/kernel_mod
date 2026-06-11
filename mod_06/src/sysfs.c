#include <linux/kobject.h>
#include <linux/sysfs.h>
#include <linux/errno.h>
#include <linux/kernel.h>

#include "kernel_fifo.h"

static struct kernel_fifo *global_queue;

static ssize_t enqueue_store(struct kobject *kobj, 
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
            rc = fifo_enqueue(global_queue, num);
            if (rc == -ENOSPC)
            {
                pr_err("Error fifo enqueue, queue is full\n");
            }
            else 
            {
                if (rc == -EINVAL)
                    pr_err("Error bad args in enqueue\n");
            }
        }
    }

    if (rc)
        return rc;
    return count;
}

static ssize_t dequeue_show(struct kobject *kobj, 
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

        rc = fifo_dequeue(global_queue, &num);
        if (rc == -ENOENT)
        {
            pr_err("Nothing to pop\n");
        }
        else
        {
            if (rc == -EINVAL)
                pr_err("Err inval in dequeue\n");
            else
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
        rc = fifo_peek(global_queue, &num);
        if (rc == -ENOENT)
        {
            pr_err("Nothing in fifo\n");
        }
        else
        {
            if (rc == -EINVAL)
                pr_err("Error inval peek\n");
            else
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
        rc = fifo_size(global_queue, &size);
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
        if (fifo_is_empty(global_queue))
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

static ssize_t is_full_show(struct kobject *kobj,
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
        if (fifo_is_full(global_queue))
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

static ssize_t available_show(struct kobject *kobj,
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
        return sysfs_emit(buf, "%lu\n", fifo_available(global_queue));
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
                rc = fifo_clear(global_queue);
                if (rc)
                {
                    pr_err("Error fifo clear\n");
                }
            }
        }
    }

    if (rc)
        return rc;
    return count;
}

static struct kobj_attribute enqueue_attr =
              __ATTR(enqueue, 0220, NULL, enqueue_store);

static struct kobj_attribute dequeue_attr =
              __ATTR(dequeue, 0444, dequeue_show, NULL);

static struct kobj_attribute peek_attr =
              __ATTR(peek, 0444, peek_show, NULL);

static struct kobj_attribute size_attr =
              __ATTR(size, 0444, size_show, NULL);

static struct kobj_attribute is_empty_attr =
              __ATTR(is_empty, 0444, is_empty_show, NULL);

static struct kobj_attribute is_full_attr =
              __ATTR(is_full, 0444, is_full_show, NULL);

static struct kobj_attribute available_attr =
              __ATTR(available, 0444, available_show, NULL);

static struct kobj_attribute clear_attr =
              __ATTR(clear, 0220, NULL, clear_store);

static struct attribute *queue_attrs[] = {
    &enqueue_attr.attr,
    &dequeue_attr.attr,
    &peek_attr.attr,
    &size_attr.attr,
    &is_empty_attr.attr,
    &is_full_attr.attr,
    &available_attr.attr,
    &clear_attr.attr,
    NULL
};

static const struct attribute_group queue_group = {
    .attrs = queue_attrs,
};

int init_sysfs(struct kernel_fifo *queue)
{
    int rc = 0;

    if (!queue)
    {
        rc = -EINVAL;
        pr_err("Err inval in init_sysfs\n");
    }
    else
    {
        queue->kobj = kobject_create_and_add("kernel_fifo", kernel_kobj);
        if (!queue->kobj)
        {
            rc = -ENOMEM;
            pr_err("No mem for kobj\n");
        }
        else
        {
            global_queue = queue;
            rc = sysfs_create_group(queue->kobj, &queue_group);
            if (rc)
            {
                pr_err("Err create group\n");
                kobject_put(queue->kobj);
                queue->kobj = NULL;
                global_queue = NULL;
            }
        }
    }
    return rc;
}

int exit_sysfs(struct kernel_fifo *queue)
{
    int rc = 0;

    if (!queue || !queue->kobj)
    {
        rc = -EINVAL;
    }
    else
    {
        sysfs_remove_group(queue->kobj, &queue_group);
        kobject_put(queue->kobj);
        queue->kobj = NULL;
        global_queue = NULL;
    }

    return rc;
}
