/*
 * libshim_omx: Android 4.2 ABI for the stock T399 Exynos OMX components.
 * 4.2's GraphicBufferMapper::lock() took "int usage"; 8.1 takes uint32_t,
 * which changes the mangled name. Forward the old symbol.
 */
#include <ui/GraphicBufferMapper.h>
#include <ui/Rect.h>

using android::GraphicBufferMapper;
using android::Rect;
using android::status_t;

extern "C" status_t _ZN7android19GraphicBufferMapper4lockEPK13native_handleiRKNS_4RectEPPv(
        GraphicBufferMapper *thiz, buffer_handle_t handle, int usage,
        const Rect &bounds, void **vaddr) {
    return thiz->lock(handle, static_cast<uint32_t>(usage), bounds, vaddr);
}
