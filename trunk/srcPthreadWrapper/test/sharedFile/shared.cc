#include "shared.h"






// -------------------------------------
// Question 1, 2
// -------------------------------------
void printAndRemoveHistory(runwayHistory *history)	// Print the landing history (in the real order) and remove the list
{
	if (history == NULL) return;

	printAndRemoveHistory(history->next);
	printf("\t\t- Plane ID: %d\n", history->id);
	free(history);
}
void printAndRemoveRunwayList(int nbrRunway, runwayHistory **runwayList)
{
	int i;
	for (i=0; i<nbrRunway; i++)
	{
		printf("\t* Runway ID: %d\n", i);
		if (runwayList[i] == NULL)	printf("\t\t- Not used\n");
		else						printAndRemoveHistory(runwayList[i]);
	}
	free(runwayList);
}
int getFreeRunway(char *busyRunway, int nbrRunway)
{
	int freeRunway;

	for (freeRunway=0; freeRunway<nbrRunway; freeRunway++)
	{
		if (!busyRunway[freeRunway]) return freeRunway;
	}
	printf("\n***Fatal error: CS with more threads than expected***\n");
	exit(0);
}

// -------------------------------------
// Question 4
// -------------------------------------
priorityWaitingQueue *newPriorityWaitingQueue(int queueSize)
{
	priorityWaitingQueue *res = (priorityWaitingQueue*)malloc(sizeof(priorityWaitingQueue));

	res->queueSize				= queueSize;
	res->nbrThreadRunningInCS	= 0;
	res->planePriorityQueue		= NULL;
	pthread_cond_init(&(res->condVar), NULL);
	return res;
}
void destroyPriorityQueue(priorityQueue *queue)
{
	if (queue == NULL) return;

	destroyPriorityQueue(queue->next);
	free(queue);
}
void destroyPriorityWaitingQueue(priorityWaitingQueue *queue)
{
	pthread_cond_destroy(&(queue->condVar));
	destroyPriorityQueue(queue->planePriorityQueue);
}
void appendToPriorityQueue (priorityWaitingQueue *queue, int threadID, int threadPrio)
{
	priorityQueue *pq = queue->planePriorityQueue, *previous = NULL;
	priorityQueue *append = (priorityQueue*)malloc(sizeof(priorityQueue));

	append->threadId		= threadID;
	append->threadPriority	= threadPrio;
	append->next			= NULL;

	if (pq == NULL)	{queue->planePriorityQueue	= append;	return;}

	while (pq != NULL)
	{
		if (pq->threadPriority > threadPrio) break;
		previous	= pq;
		pq			= pq->next;
	}
	if (pq == NULL) previous->next = append;
	else
	{
		if (previous == NULL)	queue->planePriorityQueue	= append;
		else					previous->next				= append;
		append->next = pq;
	}
}
char isTurnPriorityQueue(priorityWaitingQueue *queue, int waiterId)
{
	if (queue->nbrThreadRunningInCS == queue->queueSize) return 0;
	return (queue->planePriorityQueue->threadId == waiterId);
}
void popPriorityQueue(priorityWaitingQueue *queue, int threadID)
{
	priorityQueue *pq	= queue->planePriorityQueue;
	priorityQueue *pq1	= pq->next;

	if (pq->threadId != threadID)
	{
		printf("\n**** Corrupted priorityWaitingQueue****\n");
		printf("**** Expected plane           : %d\n", threadID);
		printf("**** Found on the top of queue: %d\n", pq->threadId);
		printf("**** Queue state: \n");
		while (pq != NULL)
		{
			printf("\t PlaneID = %d,\t Priority = %d\n", pq->threadId, pq->threadPriority);
			pq = pq->next;
		}
		exit(0);
	}
	free(pq);
	queue->planePriorityQueue = pq1;
	queue->nbrThreadRunningInCS ++;
}
void leavePriorityQueue(priorityWaitingQueue *queue)
{
	queue->nbrThreadRunningInCS --;
}
int generatePriority(int nbrPriority)
{
//	A faire
return (rand()%nbrPriority);
}
