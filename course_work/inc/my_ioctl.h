#ifndef MY_IOCTL_H
#define MY_IOCTL_H

#include <linux/ioctl.h>

#define MY_IOCTL_MAGIC 'M'

#define MY_IOCTL_CLEAR _IO(MY_IOCTL_MAGIC, 0)
#define MY_IOCTL_GET_SIZE _IOR(MY_IOCTL_MAGIC, 1, unsigned long)

#endif
