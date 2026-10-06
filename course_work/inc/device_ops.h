#ifndef DEVICE_OPS_H
#define DEVICE_OPS_H

#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/string.h>

#include "my_ioctl.h"

#define BUFFER_SIZE 1024

struct my_device_stats
{
    size_t data_size;
    unsigned long open_count;
    unsigned long read_count;
    unsigned long write_count;
};

extern const struct file_operations my_fops;

void my_device_get_stats(struct my_device_stats *stats);

#endif
