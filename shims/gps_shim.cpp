/*
 * libshim_gps: Android 4.2 SensorManager ABI for the stock Broadcom gpsd.
 * 4.2 SensorManager was a Singleton with a public default constructor; 8.1
 * uses SensorManager::getInstanceForPackage() and moved it to libsensor.
 */
#define LOG_TAG "libshim_gps"
#include <log/log.h>
#include <new>
#include <sensor/Sensor.h>
#include <sensor/SensorEventQueue.h>
#include <sensor/SensorManager.h>
#include <utils/Mutex.h>
#include <utils/String16.h>
#include <utils/String8.h>

using namespace android;

extern "C" {
/* Storage for gpsd's inlined Singleton<SensorManager>::getInstance(). */
Mutex _ZN7android9SingletonINS_13SensorManagerEE5sLockE;
SensorManager *_ZN7android9SingletonINS_13SensorManagerEE9sInstanceE = nullptr;

/* Never reached: sInstance is set before gpsd's main() runs. */
void _ZN7android13SensorManagerC1Ev(void *thiz) {
    ALOGE("4.2 SensorManager() constructor called (%p); unsupported", thiz);
}

/* sp<SensorEventQueue> SensorManager::createEventQueue()
 * (sp<> returned through the hidden return slot, passed before this) */
void _ZN7android13SensorManager16createEventQueueEv(sp<SensorEventQueue> *ret,
                                                     SensorManager *thiz) {
    new (ret) sp<SensorEventQueue>(thiz->createEventQueue(String8(""), 0));
}

/* ssize_t SensorManager::getSensorList(Sensor const* const**) const */
ssize_t _ZNK7android13SensorManager13getSensorListEPPKPKNS_6SensorE(
        SensorManager *thiz, Sensor const *const **list) {
    return thiz->getSensorList(list);
}
}

__attribute__((constructor)) static void shim_gps_init() {
    _ZN7android9SingletonINS_13SensorManagerEE9sInstanceE =
            &SensorManager::getInstanceForPackage(String16("gpsd"));
}

/* ---- OpenSSL (4.2) -> BoringSSL (8.1) for gpsd's SUPL client ---------- */
#include <openssl/mem.h>
#include <openssl/ssl.h>

extern "C" {
/* void *CRYPTO_malloc(int num, const char *file, int line) */
#undef CRYPTO_malloc
void *CRYPTO_malloc(int num, const char *file, int line) {
    (void) file;
    (void) line;
    return num > 0 ? OPENSSL_malloc((size_t) num) : nullptr;
}

/* long SSL_CTX_ctrl(SSL_CTX *ctx, int cmd, long larg, void *parg);
 * OpenSSL 1.0 command numbers used by the SSL_CTX_* macros. */
long SSL_CTX_ctrl(SSL_CTX *ctx, int cmd, long larg, void *parg) {
    switch (cmd) {
    case 14: /* SSL_CTRL_EXTRA_CHAIN_CERT */
        return SSL_CTX_add_extra_chain_cert(ctx, (X509 *) parg);
    case 32: /* SSL_CTRL_OPTIONS */
        return (long) SSL_CTX_set_options(ctx, (uint32_t) larg);
    case 33: /* SSL_CTRL_MODE */
        return (long) SSL_CTX_set_mode(ctx, (uint32_t) larg);
    case 44: /* SSL_CTRL_SET_SESS_CACHE_MODE */
        return (long) SSL_CTX_set_session_cache_mode(ctx, (int) larg);
    case 77: /* SSL_CTRL_CLEAR_OPTIONS */
        return (long) SSL_CTX_clear_options(ctx, (uint32_t) larg);
    case 78: /* SSL_CTRL_CLEAR_MODE */
        return (long) SSL_CTX_clear_mode(ctx, (uint32_t) larg);
    default:
        ALOGW("SSL_CTX_ctrl: unsupported cmd %d (larg %ld)", cmd, larg);
        return 0;
    }
}
}
