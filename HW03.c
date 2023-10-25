#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>

#define H 10 // capacity of the pot
#define N 5  // number of bees

sem_t emptyPot;
sem_t fullPot;
int portions = 0;

void *bee(void *arg) {
  while (1) {
    sem_wait(&emptyPot);
    portions++;
    printf("Bee %d gathered honey. Portions: %d\n", *((int *)arg), portions);
    if (portions == H) {
      sem_post(&fullPot);
      printf("Bee %d filled the pot and awakened the bear.\n", *((int *)arg));
    } else {
      sem_post(&emptyPot);
    }
  }
}

void *bear(void *arg) {
  while (1) {
    sem_wait(&fullPot);
    printf("Bear is eating honey.\n");
    for (int i = H; i > 0; i--) {
      portions--;
      printf("Bear ate a portion. Remaining portions: %d\n", portions);
    }
    printf("Bear has finished eating and is now going to sleep.\n");
    sem_post(&emptyPot);
  }
}

int main() {
  pthread_t bees[N], bearThread;
  int ids[N];
  sem_init(&emptyPot, 0, 1);
  sem_init(&fullPot, 0, 0);

  pthread_create(&bearThread, NULL, bear, NULL);

  for (int i = 0; i < N; i++) {
    ids[i] = i + 1;
    pthread_create(&bees[i], NULL, bee, (void *)&ids[i]);
  }

  pthread_join(bearThread, NULL);

  for (int i = 0; i < N; i++) {
    pthread_join(bees[i], NULL);
  }

  return 0;
}
