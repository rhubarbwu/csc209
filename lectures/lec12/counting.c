#include "pthread.h"
#include "stdio.h"
#include "stdlib.h"

#ifdef MUTEX
pthread_mutex_t mtx;
#endif

long S = 0;
int T, N;

void *increment_by_part(void *_) {
    long partial = 0;
    for (int j = 0; j < N; j++)
        partial += 1;
#ifdef MUTEX
    pthread_mutex_lock(&mtx);
#endif
    S += partial;
#ifdef MUTEX
    pthread_mutex_unlock(&mtx);
#endif
}

void *increment_by_one(void *_) {
    for (int j = 0; j < N; j++) {
#ifdef MUTEX
        pthread_mutex_lock(&mtx);
#endif
        long partial = S;
        partial++;
        S = partial;
#ifdef MUTEX
        pthread_mutex_unlock(&mtx);
#endif
    }
}

int main(int argc, char **argv) {
#ifdef MUTEX
    int status = pthread_mutex_init(&mtx, NULL);
    if (status != 0) {
        perror("pthread_mutex_init");
        printf("failed to create mutex :C\n");
    } else
        printf("running with mutex :)\n");
#else
    printf("running without mutex :(\n");
#endif

    T = atoi(argv[1]);
    N = atoi(argv[2]);

    pthread_t TIDs[T];

    int P = atoi(argv[3]);

    for (int t = 0; t < T; t++)
        pthread_create(&TIDs[t], NULL, P ? increment_by_part : increment_by_one, NULL);

    for (int t = 0; t < T; t++)
        pthread_join(TIDs[t], NULL);

    printf("threads summed to %d\n", S);
    return 0;
}
