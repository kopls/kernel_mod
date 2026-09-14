#include "params.h"

#define MIN_ITERATIONS 1
#define MAX_ITERATIONS 1000000
#define MAX_NUM_LOCK_TYPE 2
#define MIN_NUM_THREADS 1
#define MAX_NUM_THREADS 32
#define MAX_LEN_STR 256

extern struct sync_ctx *ctx;

static unsigned num_threads = 4;
static unsigned iterations = 1000;
static unsigned lock_type = 0;

static DEFINE_MUTEX(run_mutex);

static int num_threads_set(const char *val, const struct kernel_param *kp)
{
    unsigned int tmp_num_threads;
    int rc = 0;
    if (!val || !kp)
    {
        rc = -EFAULT;
        pr_err("Error in set num threads! !val || !kp\n");
    }
    else
    {
        rc = kstrtouint(val, 10, &tmp_num_threads);
        if (rc)
        {
            pr_err("Error in kstrtouint num threads\n");
        }
        else
        {
            if (tmp_num_threads < MIN_NUM_THREADS || tmp_num_threads > MAX_NUM_THREADS)
            {
                pr_err("Error range num_thread!\n");
                rc = -ERANGE;
            }
            else
            {
                num_threads = tmp_num_threads;
            }
        }
    }

    return rc;
}

static int num_threads_get(char *buffer, const struct kernel_param *kp)
{
    return scnprintf(buffer, PAGE_SIZE, "%u\n", num_threads);
}

static int iterations_set(const char *val, const struct kernel_param *kp)
{
    unsigned int tmp_iterations;
    int rc = 0;
    if (!val || !kp)
    {
        rc = -EFAULT;
        pr_err("Error in set iterations !val || !kp\n");
    }
    else
    {
        rc = kstrtouint(val, 10, &tmp_iterations);
        if (rc)
        {
            pr_err("Error in kstrtouint, iterations\n");
        }
        else
        {
            if (tmp_iterations < MIN_ITERATIONS || tmp_iterations > MAX_ITERATIONS)
            {
                pr_err("Error range iterations!\n");
                rc = -ERANGE;
            }
            else
            {
                iterations = tmp_iterations;
            }
        }
    }

    return rc;
}

static int iterations_get(char *buffer, const struct kernel_param *kp)
{
    return scnprintf(buffer, PAGE_SIZE, "%u\n", iterations);
}

static int lock_type_set(const char *val, const struct kernel_param *kp)
{
    unsigned int tmp_lock_type;
    int rc = 0;
    if (!val || !kp)
    {
        rc = -EFAULT;
        pr_err("Error in set lock_type !val || !kp\n");
    }
    else
    {
        rc = kstrtouint(val, 10, &tmp_lock_type);
        if (rc)
        {
            pr_err("Error in kstrtouint, lock_type\n");
        }
        else
        {
            if (tmp_lock_type > MAX_NUM_LOCK_TYPE)
            {
                pr_err("Error range lock_type!\n");
                rc = -ERANGE;
            }
            else
            {
                lock_type = tmp_lock_type;
            }
        }
    }

    return rc;
}

static int lock_type_get(char *buffer, const struct kernel_param *kp)
{
    return scnprintf(buffer, PAGE_SIZE, "%u\n", lock_type);
}

static int run_set(const char *val, const struct kernel_param *kp)
{
    unsigned run;
    int rc = 0;
    if (!val || !kp || !ctx)
    {
        rc = -EFAULT;
        pr_err("Error in set run !val || !kp\n");
    }
    else
    {
        rc = kstrtouint(val, 10, &run);
        if (rc)
        {
            pr_err("Error in kstrtouint, run\n");
        }
        else
        {
            if (run == 1) 
            {
                if (!mutex_trylock(&run_mutex))
                {
                    pr_err("Test already running\n");
                    rc = -EBUSY;
                }
                else
                {
                    ctx->num_threads = num_threads;
                    ctx->iterations = iterations;
                    ctx->lock_type = lock_type;

                    pr_info("Tests are running\n");
                    rc = run_test(ctx);

                    mutex_unlock(&run_mutex);
                }
            }
            else
            {
                pr_err("Run is 1\n");
                rc = -EINVAL;
            }
        }
    }

    return rc;
}

static int result_get(char *buffer, const struct kernel_param *kp)
{
    int rc = 0;

    if (!buffer || !kp)
    {
        rc = -EINVAL;
    }
    else
    {
        const char *lock_type_c;
        switch (lock_type)   
        {
            case SPINLOCK:
                lock_type_c = "spinlock";
                break;

            case MUTEX:
                lock_type_c = "mutex";
                break;

            case SEMAPHORE:
                lock_type_c = "semaphore";
                break;

            default:
                pr_err("Invalid lock_type!\n");
                rc = -EINVAL;
                break;
        }

        if (!rc)
        {
            rc = scnprintf(buffer, PAGE_SIZE,
                           "counter=%lld threads=%u iterations=%u lock=%s %s\n",
                           ctx->shared_counter,
                           num_threads,
                           iterations,
                           lock_type_c,
                           ctx->last_run_result == 0 ? "ok" : "error");
        }
    }

    return rc;
}

static int stats_get(char *buffer, const struct kernel_param *kp)
{
    int rc = 0;
    s64 avg_wait_ns = 0;

    if (!buffer || !kp)
    {
        rc = -EINVAL;
    }
    else
    {
        if (ctx->contention_count != 0)
            avg_wait_ns = ctx->total_wait_time_ns / ctx->contention_count;

        rc = scnprintf(buffer, PAGE_SIZE,
                      "contention=%u, total_wait_time_ns=%lld, avg_wait_ns=%lld\n",
                      ctx->contention_count,
                      ctx->total_wait_time_ns,
                      avg_wait_ns);
    }

    return rc;
}

static int reset_set(const char *val, const struct kernel_param *kp)
{
    unsigned reset;
    int rc = 0;
    if (!val || !kp)
    {
        rc = -EFAULT;
        pr_err("Error in set reset !val || !kp\n");
    }
    else
    {
        rc = kstrtouint(val, 10, &reset);
        if (rc)
        {
            pr_err("Error in kstrtouint, reset\n");
        }
        else
        {
            if (reset) 
            {
                if (!mutex_trylock(&run_mutex))
                {
                    pr_err("Test already running\n");
                    rc = -EBUSY;
                }
                else
                {
                    ctx->shared_counter = 0;
                    ctx->total_wait_time_ns = 0;
                    ctx->contention_count = 0;
                    ctx->last_run_result = 0;

                    mutex_unlock(&run_mutex);
                }
            }
            else
            {
                pr_info("Nothing");
            }
        }
    }

    return rc;
}

static const struct kernel_param_ops num_threads_ops = {
    .set = num_threads_set,
    .get = num_threads_get
};

static const struct kernel_param_ops iterations_ops = {
    .set = iterations_set,
    .get = iterations_get
};

static const struct kernel_param_ops lock_type_ops = {
    .set = lock_type_set,
    .get = lock_type_get
};

static const struct kernel_param_ops run_ops = {
    .set = run_set,
};

static const struct kernel_param_ops result_ops = {
    .get = result_get
};

static const struct kernel_param_ops stats_ops = {
    .get = stats_get
};

static const struct kernel_param_ops reset_ops = {
    .set = reset_set,
};

module_param_cb(num_threads, &num_threads_ops, NULL, 0600);
module_param_cb(iterations, &iterations_ops, NULL, 0600);
module_param_cb(lock_type, &lock_type_ops, NULL, 0600);
module_param_cb(run, &run_ops, NULL, 0200);
module_param_cb(result, &result_ops, NULL, 0400);
module_param_cb(stats, &stats_ops, NULL, 0400);
module_param_cb(reset, &reset_ops, NULL, 0200);
