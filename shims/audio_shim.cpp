/*
 * libshim_audio: Android 4.2 ABI for the stock audio.primary.universal_garda.so.
 * In 4.2 these AudioParameter getters were non-const; 8.1 made them const,
 * which changes the mangled names. Forward the old symbols to the new ones.
 */
#include <media/AudioParameter.h>
#include <utils/String8.h>
#include <new>

using android::AudioParameter;
using android::String8;
using android::status_t;

extern "C" {

status_t _ZN7android14AudioParameter3getERKNS_7String8ERS1_(
        AudioParameter *thiz, const String8 &key, String8 &value) {
    return thiz->get(key, value);
}

status_t _ZN7android14AudioParameter6getIntERKNS_7String8ERi(
        AudioParameter *thiz, const String8 &key, int &value) {
    return thiz->getInt(key, value);
}

status_t _ZN7android14AudioParameter8getFloatERKNS_7String8ERf(
        AudioParameter *thiz, const String8 &key, float &value) {
    return thiz->getFloat(key, value);
}

/*
 * Returns String8 by value: per the ARM C++ ABI the hidden return slot is
 * passed first (r0) and this second (r1), so take both explicitly.
 */
void _ZN7android14AudioParameter8toStringEv(String8 *ret, AudioParameter *thiz) {
    new (ret) String8(thiz->toString());
}

}
