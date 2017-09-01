#include "worker.h"



/**
 * This files gives the simplest implementation of the unit test worker (C++ class Worker) on a WINDOWS-specific platform.
 * The computation method (method compute) is processed by waiting a fixed number of seconds.
 * The IO file writing (method save) is processed by a synchronous posix Glibc write function.
 */








// --------------------------------------
// Local functions
// --------------------------------------
HANDLE Worker::openFile_windows(char *fileName)
{
	#ifndef WINDOWS_PLATFORM
		LOGGER_error("Internal error\nUsing a Windows-specific function to open a file on a non Windows platform", fileName, 1);
		return NULL;
	#endif

	HANDLE res = CreateFile(fileName,
							GENERIC_WRITE,			// open for writing
							0,						// share write access
							NULL,					// default security
							CREATE_ALWAYS,			// create new/overwrite existing
							 FILE_ATTRIBUTE_NORMAL,	//FILE_FLAG_OVERLAPPED | FILE_FLAG_NO_BUFFERING,   // overlapped file
							NULL);					// no attr. template
	if (res == INVALID_HANDLE_VALUE)
	{
		LOGGER_error("Can't open the output file", fileName, 1);
	}
	return res;
}


void Worker::initAIO(unsigned int nbIoDevice, unsigned int nbProc)
{
	#ifndef WINDOWS_PLATFORM
		LOGGER_error("Internal error\nUsing Windows-specific function on a non-windows platform", fileNfileName, 1);
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
		usleep_windows(this->computeTime);
	memset(output, 'X', this->nbOutputChar);
}


/**
 * \brief Write the given parameter in the output file (this->outputFileFd).  The number of characters written is determined by this->nbOutputChar.
 * \details The writing is processed by calling the synchronous posix Glibc write function.
 * \param output: the string to write in the file.
 */
void Worker::save(char *output, void *fileDescriptorPtr)
{
	#ifndef WINDOWS_PLATFORM
		LOGGER_error("Internal error\nUsing Windows-specific function on a non-windows platform", fileNfileName, 1);
	#else
		DWORD nbBytesWritten;
		HANDLE fileDescriptor = *((HANDLE*)fileDescriptorPtr);
		BOOL test = WriteFile(	fileDescriptor,				// open file handle
								output,						// start of data to write
								(DWORD)this->nbOutputChar,	// number of bytes to write
								&nbBytesWritten,			// number of bytes that were written
								NULL);						// no overlapped structure
	    if ((FALSE == test) || (nbBytesWritten < this->nbOutputChar))
	    {
			LOGGER_error("Error wile writing in the output file", "", 1);
	    }
	#endif


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
