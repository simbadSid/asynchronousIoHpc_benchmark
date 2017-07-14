																																															#include "worker.h"

#ifdef MPI_PARADIGM
	#include <omp.h>
#endif

#include <scorep/SCOREP_User.h>







// -----------------------------
// Definition of the static members
// -----------------------------
long long		Worker::memoryFootPrint;						/** <Max number of bytes simultaneously allocated (counts all the buffer)*/
long long		Worker::memoryFootPrint_tmp;
Synchronization	*Worker::synchronization;


// -----------------------------
// Builder
// -----------------------------
Worker::Worker(int nbIteration, char *outputFileName, int nbMpiLoopThread, float computeTime, unsigned int nbOutputChar)
{
	ASSERT(nbIteration	> 0);
	ASSERT(computeTime	>= 0);
	ASSERT(nbOutputChar	> 0);
	ASSERT(nbMpiLoopThread > 0);

	this->nbIteration			= nbIteration;
	this->nbOutputChar			= nbOutputChar;
	this->nbMpiLoopThread		= nbMpiLoopThread;
	this->computeTime			= computeTime*1000000.0;
	#ifdef WINDOWS_PLATFORM
	this->outputFileFd			= (HANDLE*)	malloc(nbIteration*sizeof(HANDLE));
	#else
	this->outputFileFd			= (int*)	malloc(nbIteration*sizeof(int));
	#endif
	this->pendingAioRequest		= new Queue_CS();
	#ifdef MEMORY_FOOTPRINT
		Worker::memoryFootPrint		= 0;
		Worker::memoryFootPrint_tmp	= 0;
		Worker::synchronization		= new Synchronization();
	#endif

	int i;
	for (i=0; i<nbIteration; i++)
	{
		char outputFileNameLoc[128];
		sprintf(outputFileNameLoc, "%s_%d", outputFileName, i);
		#ifdef WINDOWS_PLATFORM
			this->outputFileFd[i] = Worker::openFile_windows(outputFileNameLoc);
		#else
			this->outputFileFd[i] = Worker::openFile_unix(outputFileNameLoc);
		#endif
	}
}


Worker::~Worker()
{
	if (this->outputFileFd != NULL)
	{
		int i;
		for (i=0; i<nbIteration; i++)
		{
			#ifdef WINDOWS_PLATFORM
			CloseHandle(this->outputFileFd[i]);
			#else
			close(this->outputFileFd[i]);
			#endif
		}
		free(this->outputFileFd);
		this->outputFileFd = NULL;
	}
	delete this->pendingAioRequest;
	#ifdef MEMORY_FOOTPRINT
		delete Worker::synchronization;
	#endif
}


// -----------------------------
// Local Methods
// -----------------------------
void Worker::work()
{
	int i=0;

	//SCOREP_USER_REGION_DEFINE( mainWorkLoop )
	//SCOREP_USER_REGION_BEGIN( mainWorkLoop, "Worker::work", SCOREP_USER_REGION_TYPE_COMMON )
	//SCOREP_USER_PARAMETER_INT64("nbIteration", i)

#ifdef MPI_PARADIGM
	#pragma omp parallel num_threads(this->nbMpiLoopThread)
	{
		#pragma omp for
#endif
		for (i=0; i<this->nbIteration; i++)
		{
			this->computeAndSave(&(this->outputFileFd[i]));
		}
#ifdef MPI_PARADIGM
	}
#endif

	this->waitWorkerEffectiveIo();
	//SCOREP_USER_REGION_END( mainWorkLoop )
}


void Worker::computeAndSave(void *fileDescriptorPtr)
{
	char *buffer = (char*)malloc(this->nbOutputChar * sizeof(char));

	this->compute(buffer);
	#ifdef MEMORY_FOOTPRINT
		Worker::synchronization->mutexLock();
		Worker::updateMemoryFootPrint(this->nbOutputChar);
		this->save(buffer, fileDescriptorPtr);
		Worker::synchronization->mutexUnlock();
	#else
		this->save(buffer, fileDescriptorPtr);
	#endif
}


long long Worker::getMemoryFootPrint()
{
	return memoryFootPrint;
}


void Worker::updateMemoryFootPrint(int size)
{
	Worker::memoryFootPrint_tmp += size;
	ASSERT(Worker::memoryFootPrint_tmp >= 0);
	if (Worker::memoryFootPrint < Worker::memoryFootPrint_tmp)
		Worker::memoryFootPrint = Worker::memoryFootPrint_tmp;
}


void Worker::waitWorkerEffectiveIo()
{
	#ifdef NO_HANDLER_SIGNAL
		void *ptr;
		while ((ptr = this->pendingAioRequest->dequeue()) != NULL)
		{
			HandlerWriteParam *param = (HandlerWriteParam*)ptr;
			Worker::waitUnitAioWrite(param);
			Worker::checkUnitAioWrite(param);
		}
	#else
		this->pendingAioRequest->waitTillEmpty();
	#endif
}


void Worker::handlerWrite(sigval_t sigval)
{
	LOGGER(LOGGER_FLAG_WORKER, "Asynchronous handler\n");
	HandlerWriteParam *param = (HandlerWriteParam*) sigval.sival_ptr;
	Synchronization *pipeToOtherWorkerThread;
	if (!param->pendingAioRequest->remove(param, 0, &pipeToOtherWorkerThread))							// Do not notify main thread that the queue might be empty
		LOGGER_error("Worker thread handler", "Can't find and remove a pending request", 1);			//		The aim is to allow this handler to execute before main thread exits

	Worker::checkUnitAioWrite(param);
	LOGGER(LOGGER_FLAG_WORKER, "Remove pending request %p\n", param);
	// TODO update memory footprint

	#ifdef MEMORY_FOOTPRINT
		Worker::synchronization->mutexLock();
		Worker::updateMemoryFootPrint(-param->nbOutputChar);
		delete param;
		Worker::synchronization->mutexUnlock();
	#else
		delete param;
	#endif

	pipeToOtherWorkerThread->broadcastSignal();															// Notify main thread that queue might be empty
}
