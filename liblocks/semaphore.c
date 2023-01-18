#include <stdatomic.h>

void init_semaphore(atomic_int *semaphore_value, atomic_int *semaphore_max_value, int max_value) {
    atomic_init(semaphore_value, 0);
    atomic_init(semaphore_max_value, max_value);
}

void wait_semaphore(atomic_int *semaphore_value) {
    int expected = 0;
    while(1) {
        expected = atomic_load(semaphore_value);
        if (expected > 0) {
            atomic_fetch_sub(semaphore_value, 1);
            break;
        }
    }
}

void signal_semaphore(atomic_int *semaphore_value, atomic_int *semaphore_max_value) {
    int expected = 0;
    while(1) {
        expected = atomic_load(semaphore_value);
        if (expected < atomic_load(semaphore_max_value)) {
        	atomic_fetch_add(semaphore_value, 1);
            break;
        }
    }
}

