#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <unistd.h>

#define SEM_MUTEX_NAME "/sem-mutex"
#define SEM_BUFFER_COUNT_NAME "/sem-buffer-count"
#define SEM_SPOOL_SIGNAL_NAME "/sem-spool-signal"

// Buffer data structures
#define MAX_BUFFERS 10
char buf [MAX_BUFFERS] [100];
int buffer_index;
int buffer_print_index;

sem_t *mutex_sem, *buffer_count_sem, *spool_signal_sem;

void *producer (void *arg);
void *spooler (void *arg);

int main (int argc, char **argv)
{
    pthread_t tid_producer [10], tid_spooler;
    int i, r;

    // initialization
    buffer_index = buffer_print_index = 0;

    //  mutual exclusion semaphore, mutex_sem with an initial value 1.
    if ((mutex_sem = sem_open (SEM_MUTEX_NAME, O_CREAT, 0660, 1)) == SEM_FAILED) {
        perror ("sem_open"); exit (1);
    }
    // counting semaphore, indicating the number of available buffers. Initial value = MAX_BUFFERS
    if ((buffer_count_sem = sem_open (SEM_BUFFER_COUNT_NAME, O_CREAT, 0660, MAX_BUFFERS)) == SEM_FAILED) {
        perror ("sem_open"); exit (1);
    }
 // counting semaphore, indicating the number of strings to be printed. Initial value = 0
    if ((spool_signal_sem = sem_open (SEM_SPOOL_SIGNAL_NAME, O_CREAT, 0660, 0)) == SEM_FAILED) {
        perror ("sem_open"); exit (1);
    }

    // Create spooler
    if ((r = pthread_create (&tid_spooler, NULL, spooler, NULL)) != 0) {
        fprintf (stderr, "Error = %d (%s)\n", r, strerror (r)); exit (1);
    }
