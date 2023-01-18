#include <stdatomic.h>

void init_mutex(atomic_int *mutex) {
    atomic_init(mutex, 0);
}

void lock(atomic_int *mutex) {
    int expected = 0;
    while(!atomic_compare_exchange_strong(mutex, &expected, 1)) {
        expected = 0;
    }
}

void unlock(atomic_int *mutex) {
    atomic_store(mutex, 0);
}
