#include "util.h"





// -----------------------------
// Global parameters
// -----------------------------
static const char *enableFlags = NULL;	// controls which LOGGER messages are printed





// -----------------------------
// Local functions
// -----------------------------
void LOGGER_Init (const char *flagList)
{
	enableFlags = flagList;
}


int LOGGER_IsEnabled (char flag)
{
	if (enableFlags == NULL)
		return 0;

	return	(0 != strchr(enableFlags, flag)) ||
			(0 != strchr(enableFlags, LOGGER_FLAG_ALL));
}


void __inline__ LOGGER(char flag, const char *format, ...)
{
	if ((flag != LOGGER_FLAG_ALL) && (!LOGGER_IsEnabled (flag)))
		return;

	va_list ap;

	va_start (ap, format);
	vfprintf (stdout, format, ap);
	va_end (ap);
	fflush (stdout);
}


void LOGGER_force(const char *format, ...)
{
	va_list ap;

	va_start (ap, format);
	vfprintf (stdout, format, ap);
	va_end (ap);
	fflush (stdout);
}


void LOGGER_parameters(char loggerFlag, char * patternName, unsigned int nbIteration, unsigned int nbMpiLoopThread, float computeTime, char* outputFile, unsigned int nbOutputChar, unsigned int nbIoDevice, unsigned int nbProc)
{
	LOGGER(loggerFlag, "\n\n\n\n+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
	LOGGER(loggerFlag, "Pattern name                    : %s\n",			patternName);
	LOGGER(loggerFlag, "Nb iteration                    : %ld\n",			nbIteration);
	LOGGER(loggerFlag, "Nb MPI-loop thread              : %ld\n",			nbMpiLoopThread);
	LOGGER(loggerFlag, "Computation time (per iteration): %f (seconds)\n",	computeTime);
	LOGGER(loggerFlag, "Nb output char                  : %ld\n",			nbOutputChar);
	LOGGER(loggerFlag, "Nb IO devices                   : %ld\n",			nbIoDevice);
	LOGGER(loggerFlag, "Nb processors                   : %ld\n",			nbProc);
	LOGGER(loggerFlag, "Output file                     : %s\n",			outputFile);
	LOGGER(loggerFlag, "+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
}


void LOGGER_error(const char *msg, const char *param, int exit_b)
{
	fprintf(stdout, "\n\n**********************************\n");
	fprintf(stdout, "%s: %s\n", msg, param);
	fprintf(stdout, "**********************************\n");
	fflush (stdout);

	if (exit_b)
		exit(2);
}


void LOGGER_error_float(const char *msg, float param, char exit_b)
{
	char paramStr[128];
	sprintf(paramStr, "%f", param);
	LOGGER_error(msg, paramStr, exit_b);
}


#ifdef WINDOWS_PLATFORM
	void usleep_windows(__int64 usec)
	{
		HANDLE timer;
		LARGE_INTEGER ft;

		ft.QuadPart = -(10*usec); // Convert to 100 nanosecond interval, negative value indicates relative time

		timer = CreateWaitableTimer(NULL, TRUE, NULL);
		SetWaitableTimer(timer, &ft, 0, NULL, NULL, 0);
		WaitForSingleObject(timer, INFINITE);
		CloseHandle(timer);
	}
#endif
