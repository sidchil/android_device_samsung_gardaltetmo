/*
 * libshim_mali: thread shim for the Mali r3p2 (Android 4.2) libMali.so.
 *
 * libMali joins worker threads that are no longer valid (already joined or
 * inherited across fork()). Android 4.2's bionic returned ESRCH for that;
 * Android 8.1's bionic aborts with "invalid pthread_t passed to libc".
 *
 * libMali.so has its imports renamed (same length) to t399_th_create and
 * t399_th_join, so only libMali uses these wrappers. They track the threads
 * libMali creates and refuse to join anything not in the list.
 */
#define LOG_TAG "libshim_mali"

#include <errno.h>
#include <pthread.h>
#include <stdbool.h>
#include <log/log.h>

#define MAX_THREADS 64

static pthread_mutex_t g_lock = PTHREAD_MUTEX_INITIALIZER;
static pthread_t g_threads[MAX_THREADS];
static int g_count;
static pthread_once_t g_once = PTHREAD_ONCE_INIT;

static void atfork_child(void)
{
    /* Threads don't survive fork(); none of the handles are valid anymore. */
    g_count = 0;
    pthread_mutex_init(&g_lock, NULL);
}

static void init_once(void)
{
    pthread_atfork(NULL, NULL, atfork_child);
}

static bool take(pthread_t t)
{
    bool found = false;
    pthread_mutex_lock(&g_lock);
    for (int i = 0; i < g_count; i++) {
        if (g_threads[i] == t) {
            g_threads[i] = g_threads[--g_count];
            found = true;
            break;
        }
    }
    pthread_mutex_unlock(&g_lock);
    return found;
}

int t399_th_create(pthread_t *thread, const pthread_attr_t *attr,
                   void *(*start)(void *), void *arg)
{
    pthread_once(&g_once, init_once);
    int ret = pthread_create(thread, attr, start, arg);
    if (ret == 0) {
        pthread_mutex_lock(&g_lock);
        if (g_count < MAX_THREADS)
            g_threads[g_count++] = *thread;
        else
            ALOGW("thread table full, %p not tracked", (void *)*thread);
        pthread_mutex_unlock(&g_lock);
    }
    return ret;
}

int t399_th_join(pthread_t thread, void **retval)
{
    if (!take(thread)) {
        ALOGW("ignoring join of unknown/stale thread %p", (void *)thread);
        return ESRCH;
    }
    return pthread_join(thread, retval);
}
