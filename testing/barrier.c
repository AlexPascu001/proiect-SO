#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>
#include <string.h>
#include <liblocks.h>
#include <unistd.h>

int cnt = 0;

int nr_threads;
atomic_int mutex;
atomic_int sem;
atomic_int sem_maxval;


void barrier_point() {
    lock(&mutex);
    cnt++;
    
    if (cnt == nr_threads) {
        unlock(&mutex);
        signal_semaphore(&sem, &sem_maxval);
    }
    else {
        unlock(&mutex);
        wait_semaphore(&sem);
        signal_semaphore(&sem, &sem_maxval);
    }
}

void *tfun(void *v) {
    int *tid = (int *)v;
    
    printf("%d reached the barrier\n", *tid);
    barrier_point();
    printf("%d passed the barrier\n", *tid);
    
    free(tid);
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Numar invalid de arg.\n");
        return errno;
    }
    
    nr_threads = strtol(argv[1], NULL, 10);
  	printf("NTHRS=%d\n", nr_threads);	
  	
  	init_mutex(&mutex);
  	
  	init_semaphore(&sem, &sem_maxval, nr_threads);
  	
  	
    pthread_t *thr = (pthread_t*)malloc(nr_threads * sizeof(pthread_t));
    for (int i = 0;i < nr_threads;++i) {
        int *id = malloc(sizeof(int));
        *id = i;
        if (pthread_create(&thr[i], NULL, tfun, id)) {
            perror("Eroare creare thread.\n");
            return errno;
        }
    }
    void *result;
    for (int i = 0;i < nr_threads;++i) {
        if (pthread_join(thr[i], &result)) {
            perror("Eroare join.\n");
            return errno;
        }
    }
    free(thr);
    return 0;
}



