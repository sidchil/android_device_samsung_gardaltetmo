#include <stdint.h>
extern "C" {
#define EMPTY_SLOT(sym) void sym(void) {}
EMPTY_SLOT(_ZN7android10VectorImpl19reservedVectorImpl1Ev)
EMPTY_SLOT(_ZN7android10VectorImpl19reservedVectorImpl2Ev)
EMPTY_SLOT(_ZN7android10VectorImpl19reservedVectorImpl3Ev)
EMPTY_SLOT(_ZN7android10VectorImpl19reservedVectorImpl4Ev)
EMPTY_SLOT(_ZN7android10VectorImpl19reservedVectorImpl5Ev)
EMPTY_SLOT(_ZN7android10VectorImpl19reservedVectorImpl6Ev)
EMPTY_SLOT(_ZN7android10VectorImpl19reservedVectorImpl7Ev)
EMPTY_SLOT(_ZN7android10VectorImpl19reservedVectorImpl8Ev)
EMPTY_SLOT(_ZN7android16SortedVectorImpl25reservedSortedVectorImpl1Ev)
EMPTY_SLOT(_ZN7android16SortedVectorImpl25reservedSortedVectorImpl2Ev)
EMPTY_SLOT(_ZN7android16SortedVectorImpl25reservedSortedVectorImpl3Ev)
EMPTY_SLOT(_ZN7android16SortedVectorImpl25reservedSortedVectorImpl4Ev)
EMPTY_SLOT(_ZN7android16SortedVectorImpl25reservedSortedVectorImpl5Ev)
EMPTY_SLOT(_ZN7android16SortedVectorImpl25reservedSortedVectorImpl6Ev)
EMPTY_SLOT(_ZN7android16SortedVectorImpl25reservedSortedVectorImpl7Ev)
EMPTY_SLOT(_ZN7android16SortedVectorImpl25reservedSortedVectorImpl8Ev)
int32_t android_atomic_release_cas(int32_t oldv, int32_t newv, volatile int32_t *p)
{ return !__sync_bool_compare_and_swap(p, oldv, newv); }
void android_atomic_release_store(int32_t v, volatile int32_t *p)
{ __atomic_store_n(p, v, __ATOMIC_RELEASE); }
int32_t android_atomic_inc(volatile int32_t *p) { return __sync_fetch_and_add(p, 1); }
int32_t android_atomic_dec(volatile int32_t *p) { return __sync_fetch_and_sub(p, 1); }
int32_t android_atomic_and(int32_t v, volatile int32_t *p) { return __sync_fetch_and_and(p, v); }
int32_t android_atomic_or(int32_t v, volatile int32_t *p) { return __sync_fetch_and_or(p, v); }
int _ZN7android6Parcel10writeInt32Ei(void *thiz, int v);
int _ZN7android6Parcel11writeIntPtrEi(void *thiz, int v)
{ return _ZN7android6Parcel10writeInt32Ei(thiz, v); }
}
