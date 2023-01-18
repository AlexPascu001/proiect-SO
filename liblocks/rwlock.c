#include <stdatomic.h>

void init_rwlock(atomic_int *rwlock, atomic_int *readers) {
    atomic_init(rwlock, 0);
    atomic_init(readers, 0);
}

void rwlock_rdlock(atomic_int *rwlock, atomic_int *readers) {
    // daca lasam acest if in loc de cel din while, in momentul in care
    // elibereaza un scriitor resursa si erau cititori in coada de asteptare
    // acestia acceseaza resursa unul cate unul, in loc sa o citeasca deodata
    /*
    	if (atomic_load(rwlock) == -1) {
    		atomic_fetch_add(readers, 1);
			return;
    	}
    */
    int expected = 0;
    while(!atomic_compare_exchange_strong(rwlock, &expected, -1)) {
        expected = 0;
        // in acest caz, cand un cititor acceseaza resursa, ii "anunta" pe ceilalti sa o acceseze si ei
        if (atomic_load(rwlock) == -1) {
			break;
    	}
    }
    atomic_fetch_add(readers, 1);
}

void rwlock_wrlock(atomic_int *rwlock, atomic_int *readers) {
    int expected = 0;
    while(!atomic_compare_exchange_strong(rwlock, &expected, 1)) {
        expected = 0;
    }
    while (atomic_load(readers) != 0) {}
}

void rwlock_unlock(atomic_int *rwlock, atomic_int *readers) {
    if (atomic_load(rwlock) == -1) {
        if (atomic_fetch_sub(readers, 1) == 1) { 
            atomic_store(rwlock, 0);
        }
    }
    else {
        atomic_store(rwlock, 0);
    }
}

