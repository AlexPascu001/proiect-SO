#include <stdio.h>
#include <pthread.h>
#include <liblocks.h>

int shared_variable = 0;
atomic_int mutex;

void * increment_variable(void *arg) {
    int i;
    for (i = 0; i < 1000000; i++) {
        lock(&mutex);
        shared_variable++;
        unlock(&mutex);
    }
    return NULL;
}

int main() {
    pthread_t thread1, thread2;
    init_mutex(&mutex);

    pthread_create(&thread1, NULL, increment_variable, NULL);
    pthread_create(&thread2, NULL, increment_variable, NULL);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    printf("Final value of shared variable: %d\n", shared_variable);
    return 0;
}
