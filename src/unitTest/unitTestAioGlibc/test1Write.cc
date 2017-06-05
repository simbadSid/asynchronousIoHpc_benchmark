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


#define DIM_X	25


int main()
{
	char tmpfname[256];
	char buf[DIM_X];
	char check[DIM_X+1];
	int fd;
	struct aiocb aiocb;
	int err;
	int ret;

	snprintf(tmpfname, sizeof(tmpfname), "pts_aio_write.data");
	unlink(tmpfname);
	fd = open(tmpfname, O_CREAT | O_RDWR | O_EXCL, S_IRUSR | S_IWUSR);
	if (fd == -1)
		LOGGER_error("Error at open()", strerror(errno), 1);

//  unlink(tmpfname);

	memset(&aiocb,	0,	sizeof(struct aiocb));
	memset(buf,		'X',DIM_X);
	aiocb.aio_fildes	= fd;
	aiocb.aio_nbytes	= DIM_X;
	aiocb.aio_buf		= buf;

	if (aio_write(&aiocb) == -1)
	{
		close(fd);
		LOGGER_error("Error at aio_write()", strerror(errno), 1);
	}

	/* Wait until completion */
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

	/* check the values written */

	if (lseek(fd, 0, SEEK_SET) == -1)
	{
		close(fd);
		LOGGER_error("Error at lseek(): %s\n", strerror(errno), 1);
	}

	/* we try to read more than we wrote to be sure of the size written */

	check[DIM_X] = 1;
	if (read(fd, check, DIM_X+ 1) != DIM_X)
	{
		close(fd);
		LOGGER_error(" Error at read()", strerror(errno), 1);
	}

	if ( check[DIM_X] != 1)
	{
		close(fd);
		LOGGER_error(" Buffer overflow\n", "", 1);
	}

	if (memcmp(buf, check, DIM_X))
	{
		close(fd);
		LOGGER_error(" Bad value in buffer", "", 1);
	}

	close(fd);
	printf ("Test PASSED\n");
	return 0;
}
