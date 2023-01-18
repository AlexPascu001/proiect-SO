#include <stdatomic.h>

#if __STDC_NO_ATOMICS__
    #error "The compiler does not support C11 atomics. Please use another implementation or a different library"
#else
    void init_semaphore(atomic_int *semaphore_value, atomic_int *semaphore_max_value, int max_value);
	void wait_semaphore(atomic_int *semaphore_value);
	void signal_semaphore(atomic_int *semaphore_value, atomic_int *semaphore_max_value);
#endif
