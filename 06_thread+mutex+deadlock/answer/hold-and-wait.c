#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t L1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t L2 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t G1 = PTHREAD_MUTEX_INITIALIZER;

void something(int limit) { for(int i = 0; i < limit; ++i) ; }

void *thread1(void *arg) {
    printf("t1: begin\n");
    something(3000); // working without shared resources.
    printf("t1: try to acquire G1\n");
    pthread_mutex_lock(&G1);
    printf("t1: G1 acquired\n");
    printf("t1: try to acquire L1\n");
    pthread_mutex_lock(&L1);
    printf("t1: L1 acquired\n");
    something(9000); // working with L2
    printf("t1: try to acquire L2\n");
    pthread_mutex_lock(&L2);
    printf("t1: L2 acquired\n");
    something(6000); // working with L2
    pthread_mutex_unlock(&L2);
    pthread_mutex_unlock(&L1);
    pthread_mutex_unlock(&G1);
    something(5000); // working without shared resources.
    printf("t1: end\n");
    return NULL;
}

void *thread2(void *arg) {
    printf("                           t2: begin\n");
    something(5000); // working without shared resources.
    printf("                           t2: try to acquire G1...\n");
    pthread_mutex_lock(&G1);
    printf("                           t2: G1 acquired\n");
    printf("                           t2: try to acquire L2...\n");
    pthread_mutex_lock(&L2);
    printf("                           t2: L2 acquired\n");
    something(8000); // working with L2
    printf("                           t2: try to acquire L1...\n");
    pthread_mutex_lock(&L1);
    printf("                           t2: L1 acquired\n");
    something(4000); // working with L1, L2
    pthread_mutex_unlock(&L1);
    pthread_mutex_unlock(&L2);
    pthread_mutex_unlock(&G1);
    something(6000); // working without shared resources.
    printf("                           t2: end\n");
    return NULL;
}

int main(int argc, char *argv[]) {                    
    pthread_t p1, p2;
    printf("main: begin\n");
    pthread_create(&p1, NULL, thread1, NULL); 
    pthread_create(&p2, NULL, thread2, NULL);
    pthread_join(p1, NULL); 
    pthread_join(p2, NULL); 
    printf("main: end\n");
    return 0;
}

