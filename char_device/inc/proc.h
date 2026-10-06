#ifndef MY_PROC_H
#define MY_PROC_H

#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>

#include "device_ops.h"

int my_proc_init(void);
void my_proc_exit(void);

#endif
