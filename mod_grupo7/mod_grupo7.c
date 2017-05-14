#include "module.h"
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <semaphore.h>

pthread_t thread1, thread2, thread3, thread4, thread5, thread6, thread7, thread8, threadProd; 
int cancelled;
int setup = 0;

#define N 8
rock_t buffer[N];
int proxima_insercao = 0;
int proxima_remocao = 0;

sem_t exclusao_mutua;
sem_t espera_vaga;
sem_t espera_dado;


pthread_mutex_t mutex_read;
pthread_mutex_t mutex_deliver;

static void *producer(void* ignored){
    while(cancelled == 0){
	sem_wait(&espera_vaga);
	
	buffer[proxima_insercao] = pd_read(); 
	proxima_insercao = (proxima_insercao + 1) % N;

	
	sem_post(&espera_dado);
    }
    pthread_exit(NULL);
    return NULL;
}


static void* worker(void* ignored) {
    while (cancelled == 0) {
	sem_wait(&espera_dado);
	sem_wait(&exclusao_mutua);
	
	proxima_remocao = (proxima_remocao + 1) % N;	
	
	
	sem_post(&exclusao_mutua);
	sem_post(&espera_vaga);

	oil_t oil = pd_process(buffer[proxima_remocao]);

	pthread_mutex_lock(&mutex_deliver);
   	pd_deliver(oil);
	pthread_mutex_unlock(&mutex_deliver);
    }

    pthread_exit(NULL);
    return NULL;
}

void mod_setup() {
    assert(setup == 0);
    setup = 1;
    cancelled = 0;

	
    pthread_mutex_init(&mutex_read, NULL);
    pthread_mutex_init(&mutex_deliver, NULL);

    sem_init(&exclusao_mutua, 0, 1);
    sem_init(&espera_vaga, 0, N);
    sem_init(&espera_dado, 0, 0);
	
    pthread_create(&threadProd, NULL, &producer, NULL);
    pthread_create(&thread1, NULL, &worker, NULL);
    pthread_create(&thread2, NULL, &worker, NULL);
    pthread_create(&thread3, NULL, &worker, NULL);
    pthread_create(&thread4, NULL, &worker, NULL);
    pthread_create(&thread5, NULL, &worker, NULL);
    pthread_create(&thread6, NULL, &worker, NULL);
    pthread_create(&thread7, NULL, &worker, NULL);
    pthread_create(&thread8, NULL, &worker, NULL);
   



}

char* mod_name() {
    return "grupo7";
}

void mod_shutdown() {
    assert(setup == 1);
    cancelled = 1;
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_join(thread3, NULL);
    pthread_join(thread4, NULL);
    pthread_join(thread5, NULL);
    pthread_join(thread6, NULL);
    pthread_join(thread7, NULL);
    pthread_join(thread8, NULL);
    pthread_join(threadProd, NULL);

    sem_destroy(&exclusao_mutua);
    sem_destroy(&espera_vaga);
    sem_destroy(&espera_dado);
}
