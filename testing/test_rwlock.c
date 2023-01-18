#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <liblocks.h>

int shared_variable = 0;

const int iterations = 5;

atomic_int rds;
atomic_int rwlock;

void *reader_thread(void *v) {
    int id = *(int *)v;
    for (int i = 0; i < iterations; i++) {
        rwlock_rdlock(&rwlock, &rds);
        int value = shared_variable;
        printf("Reader %d: shared_variable = %d\n", id, value);
        rwlock_unlock(&rwlock, &rds);
    }
    return NULL;
}

void *writer_thread(void *v) {
    int id = *(int *)v;
    for (int i = 0; i < iterations; i++) {
        rwlock_wrlock(&rwlock, &rds);
        shared_variable++;
        printf("Writer %d: shared_variable = %d\n", id, shared_variable);
        rwlock_unlock(&rwlock, &rds);
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Invalid number of arguments.\n");
        return 1;
    }
    int nr_readers = strtol(argv[1], NULL, 10);
    int nr_writers = strtol(argv[2], NULL, 10);

    init_rwlock(&rwlock, &rds);

    pthread_t *readers = (pthread_t *)malloc(nr_readers * sizeof(pthread_t));
    for (int i = 0; i < nr_readers; i++) {
        int *id = malloc(sizeof(int));
        *id = i;
        if (pthread_create(&readers[i], NULL, reader_thread, id)) {
            perror("Error creating reader thread.\n");
            return 1;
        }
    }

    pthread_t *writers = (pthread_t *)malloc(nr_writers * sizeof(pthread_t));
    for (int i = 0; i < nr_writers; i++) {
        int *id = malloc(sizeof(int));
        *id = i;
        if (pthread_create(&writers[i], NULL, writer_thread, id)) {
            perror("Error creating writer thread.\n");
            return 1;
        }
    }

    for (int i = 0; i < nr_readers; i++) {
        if (pthread_join(readers[i], NULL)) {
            perror("Error joining reader thread.\n");
            return 1;
        }
    }

    for (int i = 0; i < nr_writers; i++) {
        if (pthread_join(writers[i], NULL)) {
            perror("Error joining writer thread.\n");
            return 1;
        }
    }

    free(readers);
    free(writers);
    return 0;
}

