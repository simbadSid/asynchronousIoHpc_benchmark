#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "../sharedFile/shared.h"




/*===================================================
 * Plane manager:
 * 1 runway, NBR_PLANES planes.
 * Synchronization method: mutex lock.
 * Parameters (non mandatory): <number of planes>
 ====================================================*/


pthread_mutex_t		lock;							// Synchronization lock between the threads
runwayHistory		*history	= NULL;				// List of the planes which have landed





void *land(void *arg)								// Function executed by each thread (plane)
{
	int				planeID	= (long) arg;
	runwayHistory	*newCell= (runwayHistory*)malloc(sizeof(runwayHistory));

	newCell->id = planeID;
	pthread_mutex_lock(&lock);
	newCell->next	= history;
	history			= newCell;
	pthread_mutex_unlock(&lock);

	return NULL;
}

int main(int argc, char **argv)
{
	int nbrPlanes;

	if (argc > 1)									// Initialize the number of planes
	{
		nbrPlanes = atoi(argv[1]);
		if (nbrPlanes <= 0) nbrPlanes = NBR_PLANES;
	}
	else nbrPlanes = NBR_PLANES;
	long i;

	pthread_t tids[nbrPlanes];						// Initialize the thread id struct
	pthread_mutex_init(&lock, NULL);				// Initialize the lock
	for (i=0; i<nbrPlanes; i++)						// Run the concurrent threads
		pthread_create (&tids[i], NULL, land, (void*)i);
	for (i=0; i<nbrPlanes; i++)						// Wait for all the threads to finish
		pthread_join(tids[i], NULL);

	printf("%d planes have tried to land:\n", nbrPlanes);
	printAndRemoveHistory(history);					// Check the concurrent execution's
	pthread_mutex_destroy(&lock);
	return 0;
}
