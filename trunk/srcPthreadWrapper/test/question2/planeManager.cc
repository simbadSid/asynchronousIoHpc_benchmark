#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/types.h>
#include <unistd.h>

#include "../sharedFile/shared.h"





/*===================================================
 * Plane manager:
 * N runway, P planes.
 * Synchronization method: semaphore (+ mutex lock for synchronization inside the previous cs).
 * Parameters (non mandatory): <number of planes > 0> <number of runways > 0>
 ====================================================*/



runwayHistory		**runwayList;					// List of the histories of the runways
char				*busyRunway;					// Indicates whether a runway is busy or not
sem_t				semaphore;						// Allows at most N threads to access the runway history struct
pthread_mutex_t		lock;							// Allows at most 1 thread to access the runway state
int					nbrRunway;






void *land(void *arg)								// Function executed by each thread (plane)
{
	int	planeID	= (long) arg;
	runwayHistory *newCell= (runwayHistory*)malloc(sizeof(runwayHistory));
	newCell->id = planeID;
	int freeRunway;

	sem_wait(&semaphore);							// Enter the critical section

	pthread_mutex_lock(&lock);						// Look for a free runway (in a critical section)
	freeRunway = getFreeRunway(busyRunway, nbrRunway);
	busyRunway[freeRunway] = 1;
	pthread_mutex_unlock(&lock);

	newCell->next			= runwayList[freeRunway];
	runwayList[freeRunway]	= newCell;
usleep(400);

	pthread_mutex_lock(&lock);						// Free the runway (in a critical section)
	busyRunway[freeRunway] = 0;
	pthread_mutex_unlock(&lock);

	sem_post(&semaphore);
	return NULL;
}


int main(int argc, char **argv)
{
	int nbrPlanes;
	long i;

	if (argc > 1)									// Initialize the number of planes
	{
		nbrPlanes = atoi(argv[1]);
		if (nbrPlanes <= 0) nbrPlanes = NBR_PLANES;
	}
	else nbrPlanes = NBR_PLANES;
	if (argc > 2)									// Initialize the number of runways
	{
		nbrRunway = atoi(argv[2]);
		if (nbrRunway <= 0) nbrRunway = NBR_RUNWAY;
	}
	else nbrRunway = NBR_RUNWAY;
	runwayList = (runwayHistory**)malloc(nbrRunway * sizeof(runwayHistory));
	busyRunway = (char*)malloc(nbrRunway * sizeof(char));
	for (i=0; i<nbrRunway; i++)	busyRunway[i] = 0;

	pthread_t tids[nbrPlanes];						// Initialize the thread id struct
	pthread_mutex_init(&lock, NULL);				// Initialize the lock
	sem_init(&semaphore, 0, nbrRunway);				// Initialize the semaphore
	for (i=0; i<nbrPlanes; i++)						// Run the concurrent threads
		pthread_create (&tids[i], NULL, land, (void*)i);
	for (i=0; i<nbrPlanes; i++)						// Wait for all the threads to finish
		pthread_join(tids[i], NULL);

	printf("%d planes have tried to land:\n", nbrPlanes);
	printAndRemoveRunwayList(nbrRunway, runwayList);// Check the concurrent execution's
	free(busyRunway);
	pthread_mutex_destroy(&lock);
	sem_destroy(&semaphore);
	return 0;
}
