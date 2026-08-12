#include "params.h"

static int free_set(const char *val, const struct kernel_param *kp)
{
    unsigned long addr;
    int rc = ERR_OK;
    if (!val || !kp)
    {
        pr_err("Error in args free_set\n");
        rc = -EINVAL;
    }
    else
    {
        rc = kstrtoul(val, 16, &addr);
        if (rc)
        {
            pr_err("Error in kstrtoul in free_set\n");
        }
        else
        {
            allocator_free((void *)addr);
            pr_info("Memory free at %p\n", (void*)addr);
        }
    }
    
    return rc;
}

static int alloc_set(const char *val, const struct kernel_param *kp)
{
    unsigned long bytes;
    void *ptr = NULL;
    int rc = ERR_OK;
    if (!val || !kp)
    {
        pr_err("Error in args alloc_set\n");
        rc = -EINVAL;
    }
    else
    {
        rc = kstrtoul(val, 10, &bytes);
        if (rc)
        {
            pr_err("Error in kstroul in alloc_set\n");
        }
        else
        {
            ptr = allocator_alloc(bytes);
            if (!ptr)
            {
                pr_err("Error in allocation memory allocator_alloc\n");
                rc = -EINVAL;
            }
            else
            {
                pr_info("Allocated %zu bytes. Addr = %p\n", bytes, ptr);
            }
        }
    }

    return rc;
}

static int stats_get(char *buffer, const struct kernel_param *kp)
{
    struct stats_info inf = allocator_get_stats();
    pr_info("========== ALLOCATOR INFO ==============\n"
        "total blocks:         %zu\n"
        "free blocks:          %zu\n"
        "allocated blocks:     %zu\n"
        "total memory:         %zu\n"
        "free memory:          %zu\n"
        "allocated memory:     %zu\n"
        "fragmentation percent: %zu\n",
        inf.total_blocks,
        inf.free_blocks,
        inf.allocated_blocks,
        inf.total_memory,
        inf.free_memory,
        inf.allocated_memory,
        inf.fragmentation_percent);
    // ой надо же в буфер строку класть, ай да ладно

    return 0;
}

static const struct kernel_param_ops free_ops = 
{
    .set = free_set,
};

static const struct kernel_param_ops alloc_ops = 
{
    .set = alloc_set,
};

static const struct kernel_param_ops stats_ops = 
{
    .get = stats_get
};

module_param_cb(free, &free_ops, NULL, 0200);
module_param_cb(alloc, &alloc_ops, NULL, 0200);
module_param_cb(stats, &stats_ops, NULL, 0400);
