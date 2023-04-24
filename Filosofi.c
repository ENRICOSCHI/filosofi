// inclusione delle librerie necessarie al programma
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define N 5 // numero di filosofi
#define PENSA 0 // costante che identifica lo stato di pensiero
#define ATTESA 1 // costante che identifica lo stato di attesa per le posate
#define MANGIA 2 // costante che identifica lo stato di mangiare
#define DESTRA (x+1)%N // costante che rappresenta il filosofo a destra
#define SINISTRA (x-1+N)%N // costante che rappresenta il filosofo a sinistra

/*" RC " sta per RISORSA CONDIVISA
  "mutex" mutua esclusione*/


pthread_mutex_t mutex, mutex_f[N]; // dichiarazione di semafori per RC e per forchette
int stato[N]; // dichiarazione di una variabile condivisa che contiene lo stato dei filosofi

// funzione che simula il pensiero del filosofo
void pensa(int i){
printf("\nFILOSOFO %d: sto pensando...",i);
sleep(rand()%N); // il filosofo pensa per un tempo casuale tra 0 e N
}
// funzione che simula il momento in cui il filosofo mangia
void mangia(int i){
printf("\nFILOSOFO %d: sto mangiando...",i);
sleep(rand()%N); // il filosofo mangia per un tempo casuale tra 0 e N
}
// funzione che simula il momento in cui il filosofo posa le posate
void posa(int i){
pthread_mutex_lock(&mutex); // acquisizione del semaforo
stato[i]=PENSA; // lo stato del filosofo diventa PENSA
pthread_mutex_unlock(&mutex_f[i]); // rilascio del semaforo delle forchette
pthread_mutex_unlock(&mutex); // rilascio del semaforo
}
// funzione che simula il momento in cui il filosofo prende le posate
void prendi(int i){
pthread_mutex_unlock(&mutex); // rilascio del semaforo
printf("\nFILOSOFO %d: ho fame e aspetto le forchette...",i);
stato[i]= ATTESA; // lo stato del filosofo diventa ATTESA
controlloPosate(i); // controllo se le posate sono disponibili
printf("\nFILOSOFO %d: ...ora prendo le forchette e mangio",i);
pthread_mutex_lock(&mutex_f[i]); // acquisizione del semaforo della forchetta
pthread_mutex_unlock(&mutex); // rilascio del semaforo
}
// funzione che controlla se le posate a destra o a sinistra del filosofo sono occupate
void controlloPosate(int x){
int dx = DESTRA; // filosofo a destra
int sx = SINISTRA; // filosofo a sinistra
pthread_mutex_lock(&mutex); // acquisizione del semaforo
while(stato[dx] == MANGIA || stato[sx] == MANGIA){ // finché le posate sono occupate da altri filosofi
stato[x] = ATTESA; // lo stato del filosofo diventa ATTESA
pthread_mutex_unlock(&mutex); // rilascio del semaforo
pthread_mutex_lock(&mutex_f[x]); // acquisizione del semaforo della forchetta
pthread_mutex_lock(&mutex); // acquisizione del semaforo
}
stato[x] = MANGIA; // lo stato del filosofo diventa MANGIA
pthread_mutex_unlock(&mutex); // rilascio del semaforo
}
// funzione che rappresenta il codice di ogni filosofo
void *filosofo(void *x){
int k = (int)x;
while(1){
pensa(k); // il filosofo pensa
prendi(k); // il filosofo prende le posate
mangia(k); // il filosofo mangia
posa(k); // il filosofo posa le posate
}
}
// funzione principale
int main(int argc, char *argv[]) {
int x;
srand((int)time(NULL));
pthread_t filo[N]; // definizione dei thread
pthread_mutex_init(&mutex,NULL); // inizializzazione del semaforo RC
for(x=0;x<N;x++)
pthread_mutex_init(&mutex_f[x],NULL); // inizializzazione del semaforo della forchetta
for(x=0;x<N;x++){
pthread_create(&filo[x],NULL,(void *)filosofo,(void *)x); // crea i thread
sleep(1); // si mette in attesa per un secondo
}
pthread_exit(0);
}
