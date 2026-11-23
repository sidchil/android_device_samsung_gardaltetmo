/*
 * libshim_camera: Samsung CameraParameters additions used by the stock
 * camera.exynos3.so (Android 4.2) that AOSP 8.1 libcamera_client lacks.
 */
#include <stdint.h>
#include <stdlib.h>
#include <camera/CameraParameters.h>

using android::CameraParameters;

extern "C" {

#define SHIM_STR(sym, val) \
    extern const char sym[] __attribute__((visibility("default"))); \
    const char sym[] = val;

SHIM_STR(_ZN7android16CameraParameters12KEY_METERINGE, "metering")
SHIM_STR(_ZN7android16CameraParameters13METERING_SPOTE, "spot")
SHIM_STR(_ZN7android16CameraParameters15METERING_CENTERE, "center")
SHIM_STR(_ZN7android16CameraParameters15METERING_MATRIXE, "matrix")
SHIM_STR(_ZN7android16CameraParameters11KEY_WEATHERE, "weather")
SHIM_STR(_ZN7android16CameraParameters10KEY_CITYIDE, "contextualtag-cityid")

int64_t _ZNK7android16CameraParameters8getInt64EPKc(
        const CameraParameters *thiz, const char *key) {
    const char *v = thiz->get(key);
    if (v == NULL)
        return -1;
    return strtoll(v, NULL, 10);
}

}
