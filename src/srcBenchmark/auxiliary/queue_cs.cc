#include "queue_cs.h"



#define LOGGER_FLAG_QUEUE			'q'




// -----------------------------
// Builder
// -----------------------------
Queue_CS::Queue_CS()
{
	this->synchronization	= new Synchronization();
	this->queue				= new Queue();
}


Queue_CS::~Queue_CS()
{
	delete this->synchronization;
	delete this->queue;
}


void Queue_CS::enqueue(void *ptrToEnqueue)
{
	this->synchronization->mutexLock();
	this->queue->enqueue(ptrToEnqueue);
	this->synchronization->broadcastSignal();
	LOGGER(LOGGER_FLAG_QUEUE,  "    enqueue pointer  : %p\n", ptrToEnqueue);
	this->synchronization->mutexUnlock();
}


void *Queue_CS::dequeue()
{
	this->synchronization->mutexLock();
	void *res = this->queue->dequeue();
	this->synchronization->broadcastSignal();
	LOGGER(LOGGER_FLAG_QUEUE,  "    dequeue pointer  : %p\n", res);
	this->synchronization->mutexUnlock();
	return res;
}

char Queue_CS::remove(void *ptrToRemove, char notify, Synchronization **pipeToNotifyLatter)
{
	this->synchronization->mutexLock();
	char res = this->queue->remove(ptrToRemove);
	if (notify)
		this->synchronization->broadcastSignal();
	else
		*pipeToNotifyLatter = this->synchronization;
	LOGGER(LOGGER_FLAG_QUEUE,  "    remove pointer  : %p  (found: %d)\n", ptrToRemove, res);
	this->synchronization->mutexUnlock();
	return res;
}


void Queue_CS::waitTillEmpty()
{
	this->synchronization->mutexLock();
	LOGGER(LOGGER_FLAG_QUEUE,  "    Begin sleep (till queue is empty) \n");
	while(!this->queue->isEmpty())
	{
		LOGGER(LOGGER_FLAG_QUEUE,  "    Queue sleep loop (queue not empty) \n");
		this->synchronization->waitSignalAndPauseLock();
	}
	LOGGER(LOGGER_FLAG_QUEUE,  "    End sleep (queue is empty) \n");
	this->synchronization->mutexUnlock();
}


void Queue_CS::printQueue()
{
	this->synchronization->mutexLock();
	this->queue->printQueue();
	this->synchronization->mutexUnlock();
}

