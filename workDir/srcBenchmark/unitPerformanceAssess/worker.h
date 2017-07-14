#ifndef WORKER_H
#define WORKER_H

#include <fcntl.h>
#include <unistd.h>

#include "../auxiliary/queue_cs.h"
#include "../auxiliary/util.h"
#include "../auxiliary/synchronization.h"
#include "handlerWriteParam.h"

#ifdef WINDOWS_PLATFORM
	#include <windows.h>
	// The signal header file for windows platform does not contain all the required struct.
	// All future one need to be re-implemented here (copy from linux version /usr/include/<file>.h ).
	typedef union sigval
	{
		int sival_int;
		void *sival_ptr;
	} sigval_t;
#else
	#include <signal.h>
	typedef sigval sigval_t;
#endif


#define LOGGER_FLAG_WORKER	'w'






class Worker
{
// -----------------------------
// Attributes
// -----------------------------
	private:
		#ifdef WINDOWS_PLATFORM
		HANDLE					*outputFileFd;						/** <File descriptor to write in at each save (windows style)*/
		#else
		int						*outputFileFd;						/** <File descriptor to write in at each save (UNIX style)*/
		#endif
		int						nbIteration;
		float					computeTime;						/** <Computation time (microseconds)*/
		unsigned int			nbOutputChar;						/** <Number of char produced by each compute and written by save */
		int						nbMpiLoopThread;					/** <Number of Mpi thread used within the main worker loop (only relevent if the MPI paradigm is defined)*/
		Queue_CS				*pendingAioRequest;
		static long long		memoryFootPrint;					/** <Max number of bytes simultaneously allocated (counts all the buffer)*/
		static long long		memoryFootPrint_tmp;
		static Synchronization	*synchronization;



// -----------------------------
// Builder
// -----------------------------
	public:
		Worker(int nbIteration, char *outputFileName, int nbMpiLoopThread, float computeTime, unsigned int nbOutputChar);
		~Worker();


// -----------------------------
// Local Methods
// -----------------------------
	public:
		void work();
		/**
		 * \brief Initializes the asynchronous IO library.
		 * \param nbDevice	: Max number of memory devices (IO devices that may be accessed in parallel on a machine)
		 * \param nbProc	: Max number of processor that share the AIO resources in parallel.
		 */
		void initAIO	(unsigned int nbIoDevice, unsigned int nbProc);
		/**
		 * \brief Calls the two basic function of each pattern: compute then save.
		 * \details Each of the basic operation will be executed according to the used implementation of the class Worker.
		 * \details The memory allocated for the asynchronous execution will freed by the notification handler function.
		 * \param fileDescriptor: A pointer to file descriptor where to write the data.   The file descriptor type depends on the considered OS platform (ex: in for Unix, HANDLE for windows, ...)
		 */
		void computeAndSave(void *fileDescriptorPtr);
		/**
		 * \brief Check the proper completion of the AIO write call.
		 * \details If an error is detected, or the number of written char is inconsistent, then
		 *  the error is printed and the process exits.
		 */
		static void checkUnitAioWrite(HandlerWriteParam *param);
		/**
		 *\brief Waits until the given IO write is effectively processed (written on physical memory).
		 *\details This waiting function uses active waiting.   The return of this function doesn't guarantee the correctness of the write in memory.   To do so, use the function: \"Worker::checkUnitAioWrite\".
		 */
		static void waitUnitAioWrite(HandlerWriteParam *param);
		/**
		 *\brief Waits until all the enqueued IO of the current workers are processed (written on physical memory).
		 *\details This waiting function uses no active waiting.
		 */
		void waitWorkerEffectiveIo();
		/**
		 * \brief Max number of bytes simultaneously allocated (counts all the buffer)
		 * \warning This function is not thread-safe.   Thus, it can't be called while worker threads (that write buffers) are still running.
		 */
		static long long getMemoryFootPrint();


	private:
		/**
		 * \brief Simulates the computation operation.
		 * \details Depending on the implementation, this function may sleep, or do an effective work.   No IO writing is processed.
		 * The number of characters (results) written in output are determined by this->nbOutputChar.
		 * \param output: contains the result of the computation
		 */
		void compute	(char *output);
		/**
		 * \brief Write the given parameter in the output file (this->outputFileFd).  The number of characters written is determined by this->nbOutputChar.
		 * \details Depending on the implementation, the output string my be written in a synchronous or an asynchronous way.
		 * \details The parameter "output" and allocated pointers (aiocb, handlerParameter) are freed assynchronously by the notification handler function (Worker::handlerWrite).
		 * \param output			: the string to write in the file.
		 * \param fileDescriptor	: A pointer to file descriptor where to write the data.   The file descriptor type depends on the considered OS platform (ex: in for Unix, HANDLE for windows, ...).
		 */
		void save		(char *output, void *fileDescriptorPtr);
		/**
		 * \brief This function must be called each time a buffer is allocated/freed.
		 * \warning This function is not thread-safe.   It needs to be calles within a critical section (CS arround the parameter this->memoryFootPrint_tmp).
		 * \param size: size of the buffer (positive if the buffer is allocated, negative otherwise)
		 */
		static void updateMemoryFootPrint(int size);
		/**
		 * \brief This method is asynchronously called when a write has effectively been appended to memory.
		 */
		static void handlerWrite(sigval_t sigval);


// -----------------------------
// Local Methods
// -----------------------------
	private:
	#ifdef WINDOWS_PLATFORM
		/**
		 * \brief Open a file with write access on Windows platform.
		 * \details This function needs to be re-implemented for each access type paradigm (synchronous/asynchronous).
		 */
		static HANDLE openFile_windows(char *fileName);
	#else
		/**
		 * \brief Open a file with write access on UNIX-like platform.
		 * \details This function is generic: it can be used no matter what's the access type paradigm (synchronous/asynchronous).
		 */
		static int openFile_unix(char *fileName)
		{
			int res = open(fileName, O_WRONLY | O_CREAT, S_IRWXU);
			if (res <= 0)
			{
				LOGGER_error("Can't open the output file", fileName, 1);
			}
			return res;
		}
	#endif
};




#endif
