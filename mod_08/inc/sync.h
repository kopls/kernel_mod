#ifndef SYNC_H
#define SYNC_H

#include <linux/spinlock.h>
#include <linux/mutex.h>
#include <linux/semaphore.h>

#include <linux/ktime.h>

#include <linux/sched.h>
#include <linux/atomic.h>
#include <linux/wait.h>

#define SPINLOCK 0
#define MUTEX 1
#define SEMAPHORE 2

struct sync_ctx
{
    unsigned int num_threads;
    unsigned int iterations;
    unsigned int lock_type;

    long long shared_counter;

    spinlock_t slock;
    struct mutex mlock;
    struct semaphore sem;

    s64 total_wait_time_ns;
    unsigned int contention_count;

    struct task_struct **threads;
    atomic_t threads_done;
    int last_run_result;

    wait_queue_head_t wait_queue;
};

struct worker_args
{
    struct sync_ctx *ctx;
    unsigned int thread_id;
    s64 wait_time_ns;
};

int worker_fn(void *data);
int run_test(struct sync_ctx *ctx);

void sync_lock(struct sync_ctx *ctx, s64 *wait_ns);
void sync_unlock(struct sync_ctx *ctx);

#endif
