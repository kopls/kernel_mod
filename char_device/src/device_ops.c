#include "device_ops.h"

static char device_buffer[BUFFER_SIZE];
static size_t data_size;

static unsigned long open_count;
static unsigned long read_count;
static unsigned long write_count;

void my_device_get_stats(struct my_device_stats *stats)
{
    stats->data_size = data_size;
    stats->open_count = open_count;
    stats->read_count = read_count;
    stats->write_count = write_count;
}

static ssize_t my_write(struct file *file, const char __user *user_buffer, size_t count, loff_t *offset)
{
    write_count++;

    size_t bytes_to_copy;
    unsigned long not_copied;

    bytes_to_copy = min(count, (size_t)BUFFER_SIZE);

    not_copied = copy_from_user(device_buffer, user_buffer, bytes_to_copy);

    data_size = bytes_to_copy - not_copied;

    if (data_size == 0 && bytes_to_copy != 0)
        return -EFAULT;

    pr_info("written %zu bytes\n", data_size);

    return data_size;
}

static ssize_t my_read(struct file *file, char __user *user_buffer, size_t count, loff_t *offset)
{
    read_count++;

    size_t bytes_left;
    size_t bytes_to_copy;
    unsigned long not_copied;

    if (*offset >= data_size)
        return 0;

    bytes_left = data_size - *offset;
    bytes_to_copy = min(count, bytes_left);

    not_copied = copy_to_user(user_buffer, device_buffer + *offset, bytes_to_copy);

    bytes_to_copy -= not_copied;

    if (bytes_to_copy == 0)
        return -EFAULT;

    *offset += bytes_to_copy;

    pr_info("read %zu bytes\n", bytes_to_copy);

    return bytes_to_copy;
}

static int my_open(struct inode *inode, struct file *file)
{
    open_count++;

    pr_info("opened\n");
    return 0;
}

static int my_release(struct inode *inode, struct file *file)
{
    pr_info("closed\n");
    return 0;
}

static long my_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    unsigned long size;

    switch (cmd)
    {
        case MY_IOCTL_CLEAR:
            memset(device_buffer, 0, BUFFER_SIZE);
            data_size = 0;

            pr_info("buffer cleared\n");

            return 0;

        case MY_IOCTL_GET_SIZE:
            size = data_size;

            if (copy_to_user((unsigned long __user *)arg, &size, sizeof(size)))
                return -EFAULT;

            pr_info("data size requested: %lu\n", size);

            return 0;

        default:
            return -ENOTTY;
    }
}

const struct file_operations my_fops = {
    .owner = THIS_MODULE,
    .open = my_open,
    .release = my_release,
    .read = my_read,
    .write = my_write,
    .unlocked_ioctl = my_ioctl,
};
