#ifndef SHARED_H_
#define SHARED_H_

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pthread.h>


#define NBR_PLANES		5							// Number of planes (thread) which want to land
#define NBR_RUNWAY		3							// Number of runways(shared resource)
#define NBR_PRIORITY	3							// Number of different priorities for a thread




// -------------------------------------
// Question 1
// -------------------------------------
typedef struct RUNWAY_HISTORY						// Structure used to share the accesses to a single runway
{													// Stored in the reverse order.
	int						id;
	struct RUNWAY_HISTORY	*next;
} runwayHistory;

// -------------------------------------
// Question 4
// -------------------------------------
typedef struct PRIORITY_QUEUE
{
	int						threadPriority;
	int						threadId;
	struct PRIORITY_QUEUE	*next;
}priorityQueue;
typedef struct WAITING_QUEUE_PRIORITY
{
	pthread_cond_t			condVar;
	int						queueSize;				// Max nbr thread which can access the CS
	int						nbrThreadRunningInCS;
	priorityQueue			*planePriorityQueue;
}priorityWaitingQueue;




// -------------------------------------
// Question 1, 2
// -------------------------------------
void					printAndRemoveHistory		(runwayHistory *history);
void					printAndRemoveRunwayList	(int nbrRunway, runwayHistory **runwayList);
int						getFreeRunway				(char *busyRunway, int nbrRunway);

// -------------------------------------
// Question 3, 4
// -------------------------------------
priorityWaitingQueue	*newPriorityWaitingQueue	(int queueSize);
void					appendToPriorityQueue		(priorityWaitingQueue *queue, int threadID, int threadPrio);
char					isTurnPriorityQueue			(priorityWaitingQueue *queue, int waiterId);
void					destroyPriorityWaitingQueue	(priorityWaitingQueue *queue);
void					popPriorityQueue			(priorityWaitingQueue *queue, int threadID);
void					leavePriorityQueue			(priorityWaitingQueue *queue);
int						generatePriority			(int nbrPriority);





#endif
