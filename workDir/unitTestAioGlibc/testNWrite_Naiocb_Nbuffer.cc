#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <aio.h>

#include "../srcBenchmark/auxiliary/util.h"




#define DIM_Y	20
#define DIM_X	10



int main()
{
	char tmpfname[256];
	int fd;
	struct aiocb aiocb[DIM_Y];
	int err;
	int ret;

	snprintf(tmpfname, sizeof(tmpfname), "pts_aio_write.data");
	unlink(tmpfname);
	fd = open(tmpfname, O_CREAT | O_RDWR | O_EXCL, S_IRUSR | S_IWUSR);
	if (fd == -1)
		LOGGER_error("Error at open()", strerror(errno), 1);

	int y;
	for (y=0; y<DIM_Y; y++)
	{
		memset(&aiocb[y],	0,	sizeof(struct aiocb));
		aiocb[y].aio_fildes	= fd;
		aiocb[y].aio_offset	= DIM_X*y;
		aiocb[y].aio_nbytes	= DIM_X;
		aiocb[y].aio_buf	= (char*)malloc(sizeof(char) * DIM_X);
		memset((void*)aiocb[y].aio_buf, 'a'+y, DIM_X);

		if (aio_write(&aiocb[y]) == -1)
		{
			close(fd);
			LOGGER_error("Error at aio_write()", strerror(errno), 1);
		}
	}


	// For each written str:
	for (y=0; y<DIM_Y; y++)
	{
		/* Wait until completion */
		while (aio_error (&aiocb[y]) == EINPROGRESS);

		err = aio_error(&aiocb[y]);
		ret = aio_return(&aiocb[y]);

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
	}
	// check the values written

	if (lseek(fd, 0, SEEK_SET) == -1)
	{
		close(fd);
		LOGGER_error("Error at lseek(): %s\n", strerror(errno), 1);
	}

	// we try to read more than we wrote to be sure of the size written

	char *check = (char*)malloc(sizeof(char) * DIM_X * DIM_Y);
	*(check+(DIM_X*DIM_Y)) = 1;
	if (read(fd, check, DIM_X*DIM_Y+ 1) != DIM_X*DIM_Y)
	{
		close(fd);
		LOGGER_error(" Error at read()", strerror(errno), 1);
	}

	if ( *(check+(DIM_X*DIM_Y)) != 1)
	{
		close(fd);
		LOGGER_error(" Buffer overflow\n", "", 1);
	}

	for (y=0; y<DIM_Y; y++)
	{
		if (memcmp((char*)aiocb[y].aio_buf, check, DIM_X))
		{
			close(fd);
			LOGGER_error(" Bad value in buffer", "", 1);
		}

		free((char*)aiocb[y].aio_buf);
		check += DIM_X;
	}

	close(fd);
	printf ("Test PASSED\n");
	return 0;
}
