#define LOG_TAG "sensors_garda"
#include <dlfcn.h>
#include <errno.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

#include <hardware/hardware.h>
#include <hardware/sensors.h>

#ifdef __ANDROID__
#include <android/log.h>
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#else
#define LOGE(...) ((void)0)
#endif

#ifndef STOCK_LIB_1
#define STOCK_LIB_1 "/system/lib/hw/sensors.universal_garda.so"
#endif
#ifndef STOCK_LIB_2
#define STOCK_LIB_2 "/system/vendor/lib/hw/sensors.universal_garda.so"
#endif

static struct sensors_module_t *stock_module;
static pthread_mutex_t load_lock = PTHREAD_MUTEX_INITIALIZER;

static struct sensors_module_t *load_stock(void)
{
    pthread_mutex_lock(&load_lock);
    if (!stock_module) {
        const char *paths[2] = { STOCK_LIB_1, STOCK_LIB_2 };
        int i;
        for (i = 0; i < 2 && !stock_module; i++) {
            void *h = dlopen(paths[i], RTLD_NOW);
            if (!h) {
                LOGE("dlopen %s failed: %s", paths[i], dlerror());
                continue;
            }
            stock_module = (struct sensors_module_t *)dlsym(h, HAL_MODULE_INFO_SYM_AS_STR);
            if (!stock_module) {
                LOGE("no %s in %s", HAL_MODULE_INFO_SYM_AS_STR, paths[i]);
                dlclose(h);
            }
        }
    }
    pthread_mutex_unlock(&load_lock);
    return stock_module;
}

struct wrap_dev {
    struct sensors_poll_device_1 dev;
    struct sensors_poll_device_t *real;
};

#define REAL(d) (((struct wrap_dev *)(d))->real)

static int w_close(struct hw_device_t *d)
{
    struct wrap_dev *w = (struct wrap_dev *)d;
    int r = w->real->common.close(&w->real->common);
    free(w);
    return r;
}

static int w_activate(struct sensors_poll_device_t *d, int handle, int enabled)
{
    return REAL(d)->activate(REAL(d), handle, enabled);
}

static int w_setDelay(struct sensors_poll_device_t *d, int handle, int64_t ns)
{
    return REAL(d)->setDelay(REAL(d), handle, ns);
}

static int w_poll(struct sensors_poll_device_t *d, sensors_event_t *data, int count)
{
    return REAL(d)->poll(REAL(d), data, count);
}

static int w_batch(struct sensors_poll_device_1 *d, int handle, int flags,
                   int64_t period_ns, int64_t timeout)
{
    (void)flags; (void)timeout;
    return REAL(d)->setDelay(REAL(d), handle, period_ns);
}

static int w_flush(struct sensors_poll_device_1 *d, int handle)
{
    (void)d; (void)handle;
    return -EINVAL;
}

static int w_open(const struct hw_module_t *module, const char *id,
                  struct hw_device_t **device)
{
    struct sensors_module_t *m = load_stock();
    struct hw_device_t *rd = NULL;
    struct wrap_dev *w;
    int r;

    if (!m || !m->common.methods || !m->common.methods->open)
        return -ENODEV;
    r = m->common.methods->open(&m->common, id, &rd);
    if (r)
        return r;

    w = (struct wrap_dev *)calloc(1, sizeof(*w));
    if (!w) {
        rd->close(rd);
        return -ENOMEM;
    }
    w->real = (struct sensors_poll_device_t *)rd;
    w->dev.common.tag = HARDWARE_DEVICE_TAG;
    w->dev.common.version = SENSORS_DEVICE_API_VERSION_1_3;
    w->dev.common.module = (struct hw_module_t *)module;
    w->dev.common.close = w_close;
    w->dev.activate = w_activate;
    w->dev.setDelay = w_setDelay;
    w->dev.poll = w_poll;
    w->dev.batch = w_batch;
    w->dev.flush = w_flush;
    *device = &w->dev.common;
    return 0;
}

static int w_get_sensors_list(struct sensors_module_t *module, const struct sensor_t **list)
{
    struct sensors_module_t *m = load_stock();
    (void)module;
    if (!m)
        return 0;
    return m->get_sensors_list(m, list);
}

static int w_set_operation_mode(unsigned int mode)
{
    (void)mode;
    return -EINVAL;
}

static struct hw_module_methods_t w_methods = { .open = w_open };

struct sensors_module_t HAL_MODULE_INFO_SYM = {
    .common = {
        .tag = HARDWARE_MODULE_TAG,
        .module_api_version = SENSORS_MODULE_API_VERSION_0_1,
        .hal_api_version = HARDWARE_HAL_API_VERSION,
        .id = SENSORS_HARDWARE_MODULE_ID,
        .name = "T399 sensors wrapper",
        .author = "garda",
        .methods = &w_methods,
    },
    .get_sensors_list = w_get_sensors_list,
    .set_operation_mode = w_set_operation_mode,
};
