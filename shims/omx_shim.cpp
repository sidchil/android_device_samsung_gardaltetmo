/*
 * libshim_omx: Android 4.2 ABI for the stock T399 Exynos OMX components.
 * 4.2's GraphicBufferMapper::lock() took "int usage"; 8.1 takes uint32_t,
 * which changes the mangled name. Forward the old symbol.
 */
#include <ui/GraphicBufferMapper.h>
#include <ui/Rect.h>
#define LOG_TAG "libshim_omx"
#include <log/log.h>

using android::GraphicBufferMapper;
using android::Rect;
using android::status_t;

extern "C" status_t _ZN7android19GraphicBufferMapper4lockEPK13native_handleiRKNS_4RectEPPv(
        GraphicBufferMapper *thiz, buffer_handle_t handle, int usage,
        const Rect &bounds, void **vaddr) {
    status_t ret = thiz->lock(handle, static_cast<uint32_t>(usage), bounds, vaddr);
    if (ret != android::NO_ERROR || vaddr == nullptr || *vaddr == nullptr) {
        ALOGE("lock(%p, usage 0x%x, %dx%d) = %d vaddr %p", handle, usage,
              bounds.width(), bounds.height(), ret, vaddr ? *vaddr : nullptr);
    }
    return ret;
}
