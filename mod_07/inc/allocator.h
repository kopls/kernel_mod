#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <linux/kernel.h>
#include <linux/vmalloc.h>
#include <linux/types.h>

struct stats_info
{
    size_t total_blocks;
    size_t free_blocks;
    size_t allocated_blocks;
    size_t total_memory;
    size_t free_memory;
    size_t allocated_memory;
    size_t fragmentation_percent;
};

#include "errors.h"
#include "bitmap.h"

int allocator_init(void);
void allocator_destroy(void);
void *allocator_alloc(size_t bytes);
void allocator_free(void *ptr);
struct stats_info allocator_get_stats(void);

#endif
