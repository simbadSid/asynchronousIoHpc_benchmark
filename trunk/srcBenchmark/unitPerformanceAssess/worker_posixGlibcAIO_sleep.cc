#include "worker.h"

#include <aio.h>
#include <errno.h>






/**
 * \brief This files gives an implementation of the unit test worker (C++ class Worker) on a UNIX-specific platform.
 * \detail The computation method (method compute) is processed by waiting a fixed number of seconds.
 * \detail The IO file writing (method save) is processed by an asynchronous posix Glibc aio_write function.
 */






#define NOTIFICATION_SIG SIGUSR1	//0;


// --------------------------------------
// Local functions
// --------------------------------------
/**
 * \brief Initializes the asynchronous IO library.
 * \param nbDevice	: Max number of memory devices (IO devices that may be accessed in parallel on a machine)
 * \param nbProc	: Max number of processor that share the AIO resources in parallel.
 */
void Worker::initAIO(unsigned int nbIoDevice, unsigned int nbProc)
{
	struct aioinit init;

	memset(&init, 0, sizeof(struct aioinit));

//	if (nbIoDevice)
		init.aio_threads 	= nbIoDevice;		// 1 thread per accessible IO device
//	if (nbProc)
//	{
		init.aio_locks		= 1+nbProc;			// The main thread + the number of worker threads
		init.aio_num		= 1+nbProc;
//	}

	aio_init(&init);
}


/**
 * \brief Simulates the computation operation by simply waiting a fixed number of seconds.
 * \param output: contains the result of the computation
 */
void Worker::compute (char *output)
{
	if (this->computeTime != 0)
		usleep(this->computeTime);
	memset(output, 'X', this->nbOutputChar);
}


/**
 * \brief Write the given parameter in the output file (this->outputFileFd).  The number of characters written is determined by this->nbOutputChar.
 * \details The writing is processed by calling the asynchronous posix Glibc aio_write function.
 * \details The parameter "output" and allocated pointers (aiocb, handlerParameter) are freed assynchronously by the notification handler function (Worker::handlerWrite).
 * \param output: the string to write in the file.
 */
void Worker::save(char *output, void *fileDescriptorPtr)
{
	struct aiocb *aiocb = (struct aiocb*) malloc(sizeof(struct aiocb ));				// This 2 pointer must be freed by the handler (Worker::handlerWrite)
	HandlerWriteParam *handlerParameter = new HandlerWriteParam(output, aiocb,			// May be preallocated (optimization)
											this->pendingAioRequest, Worker::synchronization, this->nbOutputChar);
	int		fileDescriptor = *((int*)fileDescriptorPtr);

	memset(aiocb, 0, sizeof(struct aiocb));
	aiocb->aio_fildes							= fileDescriptor;
	aiocb->aio_buf								= output;
	aiocb->aio_nbytes							= this->nbOutputChar;
	//TODO Temporary: change by a real file offset counter
	aiocb->aio_offset = 0;
	#ifdef NO_HANDLER_SIGNAL
	aiocb->aio_sigevent.sigev_notify			= SIGEV_NONE;							// Indicates to raise no signal to notify
	#else
	aiocb->aio_sigevent.sigev_notify			= SIGEV_THREAD;							// Indicates to raise a signal to notify
	aiocb->aio_sigevent.sigev_notify_function	= (Worker::handlerWrite);				// Indicates the signal handler
	aiocb->aio_sigevent.sigev_value.sival_ptr	= (void*)handlerParameter;				// Parameters of the signal handler
	aiocb->aio_sigevent.sigev_signo				= NOTIFICATION_SIG;						// Indicates the signal to raise
	#endif

	this->pendingAioRequest->enqueue((void*)handlerParameter);
	int test = aio_write(aiocb);

	if (test > 0)
	{
		LOGGER_error("Error wile writing in the output file", "", 1);
	}
}


void Worker::checkUnitAioWrite(HandlerWriteParam *param)
{
	struct aiocb *aiocb = (struct aiocb*)(param->param);
	int err = aio_error(aiocb);
	int ret = aio_return(aiocb);

	if (err != 0)						LOGGER_error("checkUnitAioWrite: Error at aio_error()", strerror (err), 1);
	if (ret != (long)aiocb->aio_nbytes)	LOGGER_error_float("checkUnitAioWrite: Error in the number of written char", ret, 1);
}


void Worker::waitUnitAioWrite(HandlerWriteParam *param)
{
	struct aiocb *aiocb = (struct aiocb*)(param->param);
	while (aio_error (aiocb) == EINPROGRESS);
}

