#include <stdatomic.h>

#if __STDC_NO_ATOMICS__
    #error "The compiler does not support C11 atomics. Please use another implementation or a different library"
#else
    void init_mutex(atomic_int *mutex);
    void lock(atomic_int *mutex);
    void unlock(atomic_int *mutex);
#endif
