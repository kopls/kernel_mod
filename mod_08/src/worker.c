#include "worker.h"

#define WAIT_THRESHOLD_NS 250 

int worker_fn(void *data)
{
    struct worker_args *args = data;
    struct sync_ctx *ctx = args->ctx;

    s64 wait_ns;
    args->wait_time_ns = 0;

    for (int i = 0; i < ctx->iterations; ++i)
    {
        sync_lock(ctx, &wait_ns);
        if (wait_ns > WAIT_THRESHOLD_NS)
        {
            args->wait_time_ns += wait_ns;
            ctx->contention_count++;
        }
        ctx->shared_counter++;
        sync_unlock(ctx);

        sync_lock(ctx, &wait_ns);
        if (wait_ns > WAIT_THRESHOLD_NS)
        {
            args->wait_time_ns += wait_ns;
            ctx->contention_count++;
        }
        ctx->shared_counter--;
        sync_unlock(ctx);
    }

    atomic_inc(&ctx->threads_done);
    wake_up(&ctx->wait_queue);

    return 0;
}

int run_test(struct sync_ctx *ctx)
{
    if (!ctx)
        return -EFAULT;

    int rc = 0;
    ctx->shared_counter = 0;
    ctx->total_wait_time_ns = 0;
    ctx->contention_count = 0;
    ctx->last_run_result = 0;

    init_waitqueue_head(&ctx->wait_queue);

    ctx->threads = NULL;
    atomic_set(&ctx->threads_done, 0);

    struct worker_args *args = NULL;

    switch (ctx->lock_type)   
    {
        case SPINLOCK:
            spin_lock_init(&ctx->slock);
            break;

        case MUTEX:
            mutex_init(&ctx->mlock);
            break;

        case SEMAPHORE:
            sema_init(&ctx->sem, 1);
            break;

        default:
            pr_err("Invalid lock_type!\n");
            rc = -EINVAL;
            goto free_mem;
    }

    ctx->threads = kcalloc(ctx->num_threads, sizeof(*ctx->threads), GFP_KERNEL);
    if (!ctx->threads)
    {
        rc = -ENOMEM;
        pr_err("Error allocate threads\n");
    }
    else
    {
        args = kcalloc(ctx->num_threads, sizeof(struct worker_args), GFP_KERNEL);
        if (!args)
        {
            rc = -ENOMEM;
            pr_err("Error allocate args\n");
        }
        else
        {
            for (int i = 0; i < ctx->num_threads; ++i)
            {
                args[i].ctx = ctx;
                args[i].thread_id = i;
                args[i].wait_time_ns = 0;
            }

            for (int i = 0; i < ctx->num_threads; ++i)
            {
                ctx->threads[i] = kthread_create(worker_fn, &args[i], "sync_worker_%d", i);

                if (IS_ERR(ctx->threads[i]))
                {
                    rc = PTR_ERR(ctx->threads[i]);
                    pr_err("Error in create kthread %d\n", i);
                    ctx->threads[i] = NULL;
                    goto stop_created;
                }
            }

            for (int i = 0; i < ctx->num_threads; ++i)
                wake_up_process(ctx->threads[i]);

            wait_event(ctx->wait_queue, atomic_read(&ctx->threads_done) == ctx->num_threads);          

            for (int i = 0; i < ctx->num_threads; ++i)
                ctx->total_wait_time_ns += args[i].wait_time_ns;

            goto free_mem;
        }
    }

    goto free_mem;

stop_created:
    if (ctx->threads)
        for (int i = 0; i < ctx->num_threads; ++i)
            if (ctx->threads[i])
            {
                kthread_stop(ctx->threads[i]);
                ctx->threads[i] = NULL;
            }
        
free_mem:
    kfree(args);
    args = NULL;
    kfree(ctx->threads);
    ctx->threads = NULL;

    return rc;
}
