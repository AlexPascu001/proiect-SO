#include <stdatomic.h>
#include "mutex.h"

void init_rwlock(atomic_int *rwlock, atomic_int *readers);
void rwlock_wrlock(atomic_int *rwlock, atomic_int *readers);
void rwlock_rdlock(atomic_int *rwlock, atomic_int *readers);
void rwlock_unlock(atomic_int *rwlock, atomic_int *readers);
