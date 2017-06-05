#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <aio.h>
#include <signal.h>

#include "../srcBenchmark/auxiliary/util.h"


static void aio_CompletionRoutine(sigval_t sigval)
{
	printf("\t>>>>>>>>>Routine\n");
	printf("\tInt = %d\n", sigval.sival_int);
//	printf("\tPtr = %p\n", sigval.sival_ptr);
}



#define DIM_Y	2
#define DIM_X	10


int main()
{
	char tmpfname[256];
	char buffer[DIM_Y][DIM_X];
	int fd;
	struct aiocb aiocb;
	int err;
	int ret;

	snprintf(tmpfname, sizeof(tmpfname), "pts_aio_write.data");
	unlink(tmpfname);
	fd = open(tmpfname, O_CREAT | O_RDWR | O_EXCL, S_IRUSR | S_IWUSR);
	if (fd == -1)
		LOGGER_error("Error at open()", strerror(errno), 1);



void *ptr = malloc(1);
int value = 33;
printf("Int = %d\n", value);
printf("Ptr = %p\n", ptr);

	memset(&aiocb, 0,	sizeof(struct aiocb));
	aiocb.aio_fildes						= fd;
	aiocb.aio_nbytes						= DIM_X;
	aiocb.aio_sigevent.sigev_notify         = SIGEV_SIGNAL;
	aiocb.aio_sigevent.sigev_signo			= SIGCHLD;
	aiocb.aio_sigevent.sigev_notify_function= aio_CompletionRoutine;

	int y;
	for (y=0; y<DIM_Y; y++)
	{
		aiocb.aio_offset= DIM_X*y;
		aiocb.aio_buf	= buffer[y];
		memset((char*)aiocb.aio_buf, 'a'+y, DIM_X);

		if (aio_write(&aiocb) == -1)
		{
			close(fd);
			LOGGER_error("Error at aio_write()", strerror(errno), 1);
		}
	}


	// For each written str:
	// Wait until completion
	while (aio_error (&aiocb) == EINPROGRESS);

	err = aio_error(&aiocb);
	ret = aio_return(&aiocb);

	if (err != 0)
	{
		close (fd);
		LOGGER_error(" Error at aio_error()", strerror (err), 1);
	}

	if (ret != DIM_X)
	{
		close (fd);
		LOGGER_error(" Error at aio_return()\n", "", 1);
	}
	// check the values written

	if (lseek(fd, 0, SEEK_SET) == -1)
	{
		close(fd);
		LOGGER_error("Error at lseek(): %s\n", strerror(errno), 1);
	}

	// we try to read more than we wrote to be sure of the size written

	char check[DIM_X * DIM_Y];
	check[DIM_X*DIM_Y] = 1;
	if (read(fd, check, DIM_X*DIM_Y+ 1) != DIM_X*DIM_Y)
	{
		close(fd);
		LOGGER_error(" Error at read()", strerror(errno), 1);
	}

	if (check[DIM_X*DIM_Y] != 1)
	{
		close(fd);
		LOGGER_error(" Buffer overflow\n", "", 1);
	}

	char *checkPtr = check;
	for (y=0; y<DIM_Y; y++)
	{
		if (memcmp(buffer[y], check, DIM_X))
		{
			close(fd);
			LOGGER_error(" Bad value in buffer", "", 1);
		}

		checkPtr += DIM_X;
	}

	close(fd);
	printf ("Test PASSED\n");
	return 0;
}
