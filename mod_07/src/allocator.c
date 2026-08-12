#define TOTAL_MEMORY 10 * 1024 * 1024 * 8
#define BLOCK_SIZE (4UL * 1024UL * 8UL)

#include "allocator.h"

struct memory_allocator
{
    unsigned char *bitmap;
    void *memory_pool;
    size_t *allocations;
    size_t total_blocks;
    size_t block_size;
};

static struct memory_allocator allocator;

int allocator_init(void)
{
    int rc = ERR_OK;

    allocator.total_blocks = TOTAL_MEMORY / BLOCK_SIZE;
    allocator.block_size = BLOCK_SIZE;
    size_t bitmap_bytes = DIV_ROUND_UP(allocator.total_blocks, 8);

    allocator.memory_pool = vzalloc(TOTAL_MEMORY);
    if (!allocator.memory_pool)
    {
        pr_err("Error allocate pool\n");
        rc = ERR_ALLOCATE_ALLOCATOR;
    }
    else
    {
        allocator.bitmap = create_bitmap(bitmap_bytes);
        if (!allocator.bitmap)
        {
            pr_err("Error allocate bitmap\n");
            vfree(allocator.memory_pool);
            rc = ERR_ALLOCATE_BITMAP;
        }
        else
        {
            allocator.allocations = vzalloc(allocator.total_blocks * sizeof(size_t));
            if (!allocator.allocations)
            {
                pr_err("Error allocate allocations\n");
                vfree(allocator.memory_pool);
                bitmap_destroy(allocator.bitmap);
                rc = ERR_ALLOCATE_META;
            }
        }
    }

    return rc;
}

void allocator_destroy(void)
{
    bitmap_destroy(allocator.bitmap);
    vfree(allocator.allocations);
    vfree(allocator.memory_pool);
}

void *allocator_alloc(size_t bytes)
{
    if (bytes == 0)
        return NULL;

    size_t need_blocks = DIV_ROUND_UP(bytes, allocator.block_size);
    size_t counter = 0;
    size_t start = 0;
    for (int i = 0; i < allocator.total_blocks; ++i)
    {
        if (bitmap_is_free(allocator.bitmap, i))
        {
            counter++;

            if (counter == need_blocks)
            {
                for (int j = 0; j < need_blocks; ++j)
                    my_bitmap_set(allocator.bitmap, start + j);

                allocator.allocations[start] = need_blocks;
                return (char *)allocator.memory_pool + allocator.block_size * start;
            }
        }
        else
        {
            counter = 0;
            start = i + 1;
        }
    }
    
    return NULL;
}

void allocator_free(void *ptr)
{
    if (!ptr)
        return;

    if (ptr < allocator.memory_pool || (char *)ptr >= (char *)allocator.memory_pool + allocator.total_blocks * allocator.block_size)
        return;

    size_t offset = (char *)ptr - (char *)allocator.memory_pool;

    if (offset % allocator.block_size != 0)
        return;

    size_t block = offset / allocator.block_size;
    size_t num_blocks = allocator.allocations[block];
    if (num_blocks)
    {
        for (int i = 0; i < num_blocks; ++i)
            my_bitmap_clear(allocator.bitmap, block + i);

        allocator.allocations[block] = 0;
    }
    else
    {
        return;
    }
}

struct stats_info allocator_get_stats(void)
{
    struct stats_info si;

    si.total_blocks = allocator.total_blocks;
    si.total_memory = TOTAL_MEMORY;

    int res = 0;
    for (int i = 0; i < allocator.total_blocks; ++i)
        res += allocator.allocations[i];

    si.free_blocks = allocator.total_blocks - res;
    si.allocated_blocks = res;
    si.free_memory = si.free_blocks * allocator.block_size; 
    si.allocated_memory = res * allocator.block_size;

    size_t free_run = 0;
    size_t max_free_run = 0;
    for (int i = 0; i < allocator.total_blocks; ++i)
    {
        if (allocator.allocations[i])
        {
            if (free_run > max_free_run)
                max_free_run = free_run;

            i += allocator.allocations[i] - 1;
            free_run = 0;
        }
        else
        {
            free_run++;
        }
    }

    if (free_run > max_free_run)
        max_free_run = free_run;

    if (si.free_blocks == 0)
    {
        si.fragmentation_percent = 0;
    }
    else
    {
        si.fragmentation_percent = (si.free_blocks - max_free_run) * 100 / si.free_blocks;
    }

    return si;
}
