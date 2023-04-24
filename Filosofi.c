#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define N 5
#define PENSA 0
#define ATTESA 1
#define MANGIA 2
#define DESTRA (x+1)%N
#define SINISTRA (x-1+N)%N

pthread_mutex_t mutex, mutex_f[N]; //semafori per RC e forchette
int stato[N];// variabile condivisa con lo stato dei filosofi

void pensa(int i){
	printf("\nFILOSOFO %d: sto pensando...",i);
	sleep(rand()%N); //pensa per un tempo casuale tra 0 e N
}
void mangia(int i){
	printf("\nFILOSOFO %d: sto mangiando...",i);
	sleep(rand()%N); // mangia per un tempo casuale tra 0 e N
}
void posa(int i){
	pthread_mutex_lock(&mutex);
	stato[i]=PENSA;
	pthread_mutex_unlock(&mutex_f[i]);
	pthread_mutex_unlock(&mutex);
	
}
void prendi(int i){
	pthread_mutex_unlock(&mutex);
	printf("\nFILOSOFO %d: ho fame e aspetto le forchette...",i);
	stato[i]= ATTESA;
	controlloPosate(i);
	printf("\nFILOSOFO %d: ... ora prendo le forchette e mangio",i);
	pthread_mutex_lock(&mutex_f[i]);
	pthread_mutex_unlock(&mutex);
}
void controlloPosate(int x){
int dx = DESTRA;
int sx = SINISTRA;
pthread_mutex_lock(&mutex);

while(stato[dx] == MANGIA || stato[sx] == MANGIA){
    stato[x] = ATTESA;
    pthread_mutex_unlock(&mutex);
    pthread_mutex_lock(&mutex_f[x]);
    pthread_mutex_lock(&mutex);
}
stato[x] = MANGIA;
pthread_mutex_unlock(&mutex);
}
void *filosofo(void *x){
	int k = (int)x;
	while(1){
		pensa(k);
		prendi(k);
		mangia(k);
		posa(k);
	}
}
int main(int argc, char *argv[]) {
	int x;
	srand((int)time(NULL));
	pthread_t filo[N];//definizione dei thread
	pthread_mutex_init(&mutex,NULL);//inizializzazione semafori
	for(x=0;x<N;x++)
	pthread_mutex_init(&mutex_f[x],NULL);
	for(x=0;x<N;x++){
		pthread_create(&filo[x],NULL,(void *)filosofo,(void *)x);//crea i thread
		sleep(1);
	}
	pthread_exit(0);
}
