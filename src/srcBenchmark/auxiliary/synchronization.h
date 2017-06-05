#ifndef SYNCHRONIZATION_H_
#define SYNCHRONIZATION_H_


#include "util.h"

#ifdef WINDOWS_PLATFORM

#else
	#include <pthread.h>
#endif


/**
 * \brief This class defines a wrapper for the synchronization tools: Mutex and Condition_Variable.
 * \details Its main purpose is to allow the portability between Unix and Windows platforms.
 * \details By default, the Unix libraries are used.   In order to be compatible with the Windows platform, one needs to compile this class with the macros WINDOWS_PLATFORM.
 */


class Synchronization
{
// -----------------------------
// Attributes
// -----------------------------
	private:
	#ifdef WINDOWS_PLATFORM

	#else
		pthread_mutex_t	*mutex;
		pthread_cond_t	*conditionPipe;
	#endif


// -----------------------------
// Builder
// -----------------------------
	public:
		Synchronization();
		~Synchronization();


// -----------------------------
// Local methods
// -----------------------------
	public:
		void mutexLock();
		void mutexUnlock();
		void broadcastSignal();
		void waitSignalAndPauseLock();
};
#endif
