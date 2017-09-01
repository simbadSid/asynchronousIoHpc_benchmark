#include "unitPerformanceAssess/worker.h"


// --------------------------------------
// Input
// --------------------------------------
	#define LOGGER_FLAG_MAIN				'm'

	#define	DEFAULT_NB_ITERATION			3
	#define DEFAULT_NB_MPI_LOOP_THREAD		5
	#define	DEFAULT_COMPUTE_TIME			1
	#define	DEFAULT_OUTPUTFILE				"../resource/ioFiles/default"
	#define	DEFAULT_NB_CHAR_OUTPUT			10
	#define DEFAULT_NB_IO_DEVICE			1
	#define DEFAULT_NB_PROCESSOR			2
	#define DEFAULT_LOGGER_FLAG				'm'

	#define PARAMETER_NB_ITERATION			"-nbIteration"
	#define PARAMETER_NB_MPI_LOOP_THREAD	"-nbMpiLoopThread"
	#define PARAMETER_COMPUTE_TIME			"-computeTime"
	#define PARAMETER_OUTPUT_FILE			"-outputFile"
	#define	PARAMETER_NB_CHAR_OUTPUT		"-nbCharOutput"
	#define PARAMETER_NB_IO_DEVICE			"-nbIoDevice"
	#define PARAMETER_NB_PROCESSOR			"-nbProcessor"
	#define PARAMETER_LOGGER_FLAG			"-logger"
	#define PARAMETER_PRINT_HELP			"-help"


// --------------------------------------
// Local functions
// --------------------------------------


	void printHelp(char exit_b)
	{
		LOGGER_force("\nUsage: \n");
		LOGGER_force("\t%s\t <Number of iterations>\n",													PARAMETER_NB_ITERATION);
		LOGGER_force("\t%s\t <Number of MPI thread that execute the main loop>\n",						PARAMETER_NB_MPI_LOOP_THREAD);
		LOGGER_force("\t%s\t <Computation time of each iteration (in seconds > 0)>\n",					PARAMETER_COMPUTE_TIME);
		LOGGER_force("\t%s\t <Output file>\n",															PARAMETER_OUTPUT_FILE);
		LOGGER_force("\t%s\t <Nuber of bytes written at each iteration>\n",								PARAMETER_NB_CHAR_OUTPUT);
		LOGGER_force("\t%s\t <Nuber of IO devices(that may be used in parallel)>\n",					PARAMETER_NB_IO_DEVICE);
		LOGGER_force("\t%s\t <Nuber of processors (that may share the AIO resources in parallel)>\n",	PARAMETER_NB_PROCESSOR);
		LOGGER_force("\t%s\t\t <Flgs indicating which part of code prints info>\n",						PARAMETER_LOGGER_FLAG);
		LOGGER_force("\t%s\t\t to print the current help\n",											PARAMETER_PRINT_HELP);

		if (exit_b)
			exit(0);
	}


	/**
	 * \details Read the parameters from the command line and put them into the output parameters
	 */
	void extractParameter(int argc, char **argv, unsigned int *nbIteration, unsigned int *nbMpiLoopThread, float* computeTime, char* outputFile, unsigned int *nbOutputChar, unsigned int *nbIoDevice, unsigned int *nbProc, char *loggerFlag)
	{
		int i;

		// Initialize parameters with default value
		*nbIteration	= DEFAULT_NB_ITERATION;
		*nbMpiLoopThread= DEFAULT_NB_MPI_LOOP_THREAD;
		*computeTime	= DEFAULT_COMPUTE_TIME;
		*nbOutputChar	= DEFAULT_NB_CHAR_OUTPUT;
		*nbIoDevice		= DEFAULT_NB_IO_DEVICE;
		*nbProc			= DEFAULT_NB_PROCESSOR;
		sprintf(loggerFlag, "%c", DEFAULT_LOGGER_FLAG);
		sprintf(outputFile, "%s", DEFAULT_OUTPUTFILE);

		for (i=1; i<argc; i+=2)
		{
			if (!strcmp(argv[i], PARAMETER_PRINT_HELP))
			{
				printHelp(1);
			}

			if (i == argc-1) LOGGER_error("Missing value after parameter", argv[i], 1);
			if      (!strcmp(argv[i], PARAMETER_NB_ITERATION))		*nbIteration	= atoi(argv[i+1]);
			else if (!strcmp(argv[i], PARAMETER_NB_MPI_LOOP_THREAD))*nbMpiLoopThread= atoi(argv[i+1]);
			else if (!strcmp(argv[i], PARAMETER_COMPUTE_TIME))		sscanf(argv[i+1], "%f", computeTime);
			else if (!strcmp(argv[i], PARAMETER_OUTPUT_FILE))		sprintf(outputFile, "%s", argv[i+1]);
			else if (!strcmp(argv[i], PARAMETER_NB_CHAR_OUTPUT))	*nbOutputChar	= atoi(argv[i+1]);
			else if (!strcmp(argv[i], PARAMETER_NB_IO_DEVICE))		*nbIoDevice		= atoi(argv[i+1]);
			else if (!strcmp(argv[i], PARAMETER_NB_PROCESSOR))		*nbProc			= atoi(argv[i+1]);
			else if (!strcmp(argv[i], PARAMETER_LOGGER_FLAG))		sprintf(loggerFlag, "%s", argv[i+1]);
			else
			{
				LOGGER_error("Unknown parameter", argv[i], 0);
				printHelp(1);
			}
		}
		if (computeTime <= 0)
			LOGGER_error_float("Computation time must be > 0", *computeTime, 1);
	}


/**
 * \brief Main function of the unit test.
 * \details Depending on the parameters, it will loop on the 2 operations: compute and IO-save.
 * The worker implementation needs to be set at compile time.
 * \param (cmd line)-nbIteration		: number of iterations of the main loop.
 * \param (cmd line)-nbCharOutput	: number of characters managed at each iteration: created by compute and written by save.
 */
int main(int argc, char **argv)
{
	unsigned int nbIteration, nbMpiLoopThread, nbIoDevice, nbProc;
	float computeTime;
	unsigned int nbOutputChar;
	char outputFile[128], loggerFlag[128];

	extractParameter(argc, argv, &nbIteration, &nbMpiLoopThread, &computeTime, outputFile, &nbOutputChar, &nbIoDevice, &nbProc, loggerFlag);
	LOGGER_Init(loggerFlag);
	LOGGER_parameters(LOGGER_FLAG_MAIN, *argv, nbIteration, nbMpiLoopThread, computeTime, outputFile, nbOutputChar, nbIoDevice, nbProc);

	Worker *worker = new Worker(nbIteration, outputFile, nbMpiLoopThread, computeTime, nbOutputChar);
	worker->initAIO(nbIoDevice, nbProc);
	LOGGER(LOGGER_FLAG_MAIN, ">>>>>>> Beginning\n");
//if (0)
//    printf("AAAAAAA\n");
	worker->work();
	LOGGER(LOGGER_FLAG_MAIN, ">>>>>>> End\n");

#ifdef MEMORY_FOOTPRINT
	LOGGER_force("%lld\n", Worker::getMemoryFootPrint());
#endif
	delete worker;
	return 0;
}

