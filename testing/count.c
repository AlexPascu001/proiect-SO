#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <liblocks.h>
#define MAX_RESOURCES 5

int available_resources = MAX_RESOURCES;
int x;
atomic_int mutex;

int decrease_count(int count) {
	if (count > available_resources)
		return -1;
	else {
		available_resources -= count;
		return 0;
	}

}

int increase_count(int count) {
	available_resources += count;
	return 0;
}

void *thread_func(void *argc) {
	int *cnt = (int*)argc;
    lock(&mutex);
    if (decrease_count(*cnt)) {
    	perror("Nu sunt destule resurse disponibil1e pentru a aloca alte resurse!\n");
    	unlock(&mutex);
    	return NULL;
    }
    else {
    	printf("Got %d resources, %d remaining.\n", *cnt, available_resources);
    }
    increase_count(*cnt);
    printf("Released %d resources, %d remaining.\n", *cnt, available_resources);
    unlock(&mutex);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Numar invalid de arg.\n");
        return errno;
    }

    init_mutex(&mutex);
    
    srand(time(NULL));
    int nr_threads = strtol(argv[1], NULL, 10);
    pthread_t *thr = (pthread_t*)malloc(nr_threads * sizeof(pthread_t));
    int *thr_ids = (int*)malloc(nr_threads * sizeof(int));
    for (int i = 0;i < nr_threads;++i) {
    	thr_ids[i] = rand() % MAX_RESOURCES + 1;
        if (pthread_create(thr + i, NULL, thread_func, thr_ids+i)) {
            perror("Eroare creare thread.\n");
            return errno;
        }
    }
    void *result;
    for (int i = 0; i < nr_threads;++i) {
        if (pthread_join(thr[i], &result)) {
            perror("Eroare join.\n");
            return errno;
        }
    }
    free(thr);
    return 0;
}




