#include "worker.h"



/**
 * This files gives the simplest implementation of the unit test worker (C++ class Worker) on a UNIX-specific platform.
 * The computation method (method compute) is processed by waiting a fixed number of seconds.
 * The IO file writing (method save) is processed by a synchronous posix Glibc write function.
 */








// --------------------------------------
// Local functions
// --------------------------------------
void Worker::initAIO(unsigned int nbIoDevice, unsigned int nbProc)
{
	#ifdef WINDOWS_PLATFORM
		LOGGER_error("Internal error\nUsing a UNIX-specific function on a windows platform", "", 1);
	#endif
	return;
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
 * \details The writing is processed by calling the synchronous posix Glibc write function.
 * \param output: the string to write in the file.
 */
void Worker::save(char *output, void *fileDescriptorPtr)
{
	#ifdef WINDOWS_PLATFORM
		LOGGER_error("Internal error\nUsing a UNIX-specific function on a windows platform", fileNfileName, 1);
	#endif

	int fileDescriptor = *((int*)fileDescriptorPtr);
	unsigned int test = write(fileDescriptor, output, this->nbOutputChar);
	if (test != this->nbOutputChar)
	{
		LOGGER_error("Error wile writing in the output file", "", 1);
	}
	fflush(stdout);

	free(output);
	#ifdef MEMORY_FOOTPRINT
	// No need to synchronization: there is no concurent handler
	Worker::updateMemoryFootPrint(-this->nbOutputChar);
	#endif
}


void Worker::checkUnitAioWrite(HandlerWriteParam *param)
{

}


void Worker::waitUnitAioWrite(HandlerWriteParam *param)
{

}
