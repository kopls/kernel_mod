#include "sync.h"

void sync_lock(struct sync_ctx *ctx, s64 *wait_ns)
{
    ktime_t before, after;
    switch (ctx->lock_type)   
    {
        case SPINLOCK:
            before = ktime_get();
            spin_lock(&ctx->slock);
            after = ktime_get();
            break;

        case MUTEX:
            before = ktime_get();
            mutex_lock(&ctx->mlock);
            after = ktime_get();
            break;

        case SEMAPHORE:
            before = ktime_get();
            down(&ctx->sem);
            after = ktime_get();
            break;

        default:
            pr_err("Invalid lock_type!\n");
            break;
    }

    *wait_ns = ktime_to_ns(ktime_sub(after, before));
}

void sync_unlock(struct sync_ctx *ctx)
{
    switch (ctx->lock_type)   
    {
        case SPINLOCK:
            spin_unlock(&ctx->slock);
            break;

        case MUTEX:
            mutex_unlock(&ctx->mlock);
            break;

        case SEMAPHORE:
            up(&ctx->sem);
            break;

        default:
            pr_err("Invalid lock_type!\n");
            break;
    }
}
