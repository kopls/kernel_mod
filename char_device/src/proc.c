#include "proc.h"

#define PROC_NAME "my_device"

static struct proc_dir_entry *proc_entry;

static ssize_t proc_read(struct file *file, char __user *user_buffer, size_t count, loff_t *offset)
{
    char buffer[256];
    int len;

    struct my_device_stats stats;

    my_device_get_stats(&stats);

    len = snprintf(
        buffer,
        sizeof(buffer),
        "buffer_capacity: %d\n"
        "data_size: %zu\n"
        "open_count: %lu\n"
        "read_count: %lu\n"
        "write_count: %lu\n",
        BUFFER_SIZE,
        stats.data_size,
        stats.open_count,
        stats.read_count,
        stats.write_count);

    return simple_read_from_buffer(user_buffer, count, offset, buffer, len);
}

static const struct proc_ops proc_fops = {
    .proc_read = proc_read,
};

int my_proc_init(void)
{
    proc_entry = proc_create(PROC_NAME, 0444, NULL, &proc_fops);

    if (proc_entry == NULL)
        return -ENOMEM;

    pr_info("created /proc/%s\n", PROC_NAME);

    return 0;
}

void my_proc_exit(void)
{
    proc_remove(proc_entry);

    pr_info("removed /proc/%s\n", PROC_NAME);
}
