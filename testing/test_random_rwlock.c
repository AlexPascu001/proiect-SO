#include <stdio.h>
#include <stdatomic.h>
#include <pthread.h>
#include <liblocks.h>
#include <errno.h>
#include <time.h>
 
atomic_int rwlock;
atomic_int readers;
 
void *reader_thread(void *arg) {
    int id = *(int *)arg;
    printf("Reader %d: Trying to acquire read lock\n", id);
    rwlock_rdlock(&rwlock, &readers);
    printf("Reader %d: Read lock acquired\n", id);
    sleep(1);
    printf("Reader %d: Releasing read lock\n", id);
    rwlock_unlock(&rwlock, &readers);
    return NULL;
}
 
void *writer_thread(void *arg) {
    int id = *(int *)arg;
    printf("Writer %d: Trying to acquire write lock\n", id);
    rwlock_wrlock(&rwlock, &readers);
    printf("Writer %d: Write lock acquired\n", id);
    sleep(1);
    printf("Writer %d: Releasing write lock\n", id);
    rwlock_unlock(&rwlock, &readers);
    return NULL;
}
 
int main(int argc, char *argv[]) {
    init_rwlock(&rwlock, &readers);
    if (argc != 3) {
        printf("Numar invalid de arg.\n");
        return errno;
    }
 
    int cnt_readers = strtol(argv[1], NULL, 10);
    int cnt_writers = strtol(argv[2], NULL, 10);
 
    pthread_t readers[cnt_readers], writers[cnt_writers];
    int reader_ids[cnt_readers], writer_ids[cnt_writers];
    for (int i = 0; i < cnt_readers; i++) {
        reader_ids[i] = i;
    }
    for (int i = 0; i < cnt_writers; i++) {
        writer_ids[i] = i;
    }
 
    int SZ = cnt_readers + cnt_writers;
    int perm[SZ];
    for (int i = 0; i < SZ; ++i) {
        perm[i] = i;
    }
    srand(time(NULL));
    // Random permutation the order
    for (int i = 0; i < SZ; i++) {
        int j, t;
        j = rand() % (SZ-i) + i;
        t = perm[j]; 
        perm[j] = perm[i]; 
        perm[i] = t; // Swap i and j
    }
 
    for (int i = 0;i < SZ;++i) {
        int val = perm[i];
        if (val < cnt_readers) {
            pthread_create(&readers[val], NULL, reader_thread, &reader_ids[val]);
        }
        else {
            pthread_create(&writers[val - cnt_readers], NULL, writer_thread, &writer_ids[val - cnt_readers]);
        }
    }
 
    for (int i = 0; i < cnt_readers; i++) {
        pthread_join(readers[i], NULL);
    }
    for (int i = 0; i < cnt_writers; i++) {
        pthread_join(writers[i], NULL);
    }
    
    return 0;
}
