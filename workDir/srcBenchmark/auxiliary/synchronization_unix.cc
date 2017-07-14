#include "synchronization.h"




// -----------------------------
// Builder
// -----------------------------
Synchronization::Synchronization()
{
	this->mutex			= (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t));
	this->conditionPipe	= (pthread_cond_t*)malloc(sizeof(pthread_cond_t));

	ASSERT(this->mutex			!= NULL);
	ASSERT(this->conditionPipe	!= NULL);
	ASSERT(pthread_mutex_init(this->mutex, NULL)		== 0);
	ASSERT(pthread_cond_init(this->conditionPipe, NULL)	== 0);
	ASSERT(1);
}


Synchronization::~Synchronization()
{
	ASSERT(pthread_mutex_destroy(this->mutex)		== 0);
	ASSERT(pthread_cond_destroy(this->conditionPipe)== 0);
	free(this->mutex);
	free(this->conditionPipe);
}


// -----------------------------
// Local methods
// -----------------------------
void Synchronization::mutexLock()
{
	pthread_mutex_lock(this->mutex);
}


void Synchronization::mutexUnlock()
{
	pthread_mutex_unlock(this->mutex);
}


void Synchronization::broadcastSignal()
{
	pthread_cond_signal(this->conditionPipe);
}


void Synchronization::waitSignalAndPauseLock()
{
	pthread_cond_wait(this->conditionPipe, this->mutex);
}
