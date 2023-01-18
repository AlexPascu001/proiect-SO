#include <stdio.h>
#include <liblocks.h>

atomic_int mutex;

int main() {
    init_mutex(&mutex);
    printf("Trying to acquire the lock...\n");
    lock(&mutex);
    printf("Lock acquired!\n");
    printf("Doing some critical work...\n");
    printf("Releasing the lock...\n");
    unlock(&mutex);
    printf("Lock released!\n");
    return 0;
}
