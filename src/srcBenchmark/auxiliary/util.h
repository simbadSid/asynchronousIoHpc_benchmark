#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cstdarg>




#define LOGGER_FLAG_ALL	'+'



/**
 * \brief Initializes the logger such that only messages with the given flag get printed.
 * \param flagList: String of characters for whose LOGGER messages are to be enabled.
 * If the flagList is "+", we enable all LOGGER messages.
 */
void LOGGER_Init (const char *flagList);


/**
 * \brief Return TRUE (1) if DEBUG messages with "flag" are to be printed.
 */
int LOGGER_IsEnabled (char flag);


/**
 * \brief Prints a message, if flag is enabled (equivalent to Like printf)
 * \details This function is implemented as __inline__ because it is called by performance-critical functions (sometimes without even triggering a print)
 * \param flag		: flag corresponding to the type of message to print. If flag is NULL, then we force the print.
 * \param format	: equivalent to printf format
 */
void LOGGER(char flag, const char *format, ...);


/**
 * \brief Prints a message(equivalent to Like printf)
 * \param format	: equivalent to printf format
 */
void LOGGER_force(const char *format, ...);


/**
 * \brief Print the parameters of the benchmark pattern.
 */
void LOGGER_parameters(char loggerFlag, char * patternName, unsigned int nbIteration, unsigned int nbMpiLoopThread, float computeTime, char* outputFile, unsigned int nbOutputChar, unsigned int nbIoDevice, unsigned int nbProc);


/**
 * \brief Prints a standardized error message.
 * \details prints also the file and line number of the error in the code.
 */
void LOGGER_error(const char *msg, const char *param, int exit_b);


/**
 * \brief Prints a standardized error message
 */
void LOGGER_error_float(const char *msg, float param, char exit_b);


/**
 * \brief If condition is false, print a message and dump core.
 * \details Useful for documenting assumptions in the code.
 *
 * \details: needs to be a #define, to be able to print the location where the error occurred.
 * \authors Cf code Thomas Ropars (project dynamic OS allocator)
 */
#define ASSERT(condition)			\
	if (!(condition))				\
	{								\
		fprintf(stderr, "Assertion failed: line %d, file \"%s\"\n", __LINE__, __FILE__);	\
		fflush(stderr);				\
		abort();					\
	}

#endif

#ifdef WINDOWS_PLATFORM
#include <windows.h>
	void usleep_windows(__int64 usec);
#endif
