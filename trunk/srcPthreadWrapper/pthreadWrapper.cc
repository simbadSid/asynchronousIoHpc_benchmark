/****************************************************************************
**  CUBE        http://www.scalasca.org/                                   **
*****************************************************************************
**  Copyright (c) 2017                                                     **
**  Forschungszentrum Juelich GmbH, Juelich Supercomputing Centre          **
**                                                                         **
**  This software may be modified and distributed under the terms of       **
**  a BSD-style license.  See the COPYING file in the package base         **
**  directory for details.                                                 **
****************************************************************************/



#include "pthreadWrapper.h"





// -----------------------------
// Global attributes
// -----------------------------
static unsigned char	initialized			= 0;
static unsigned int		nbCpu				= 0;
static unsigned int		nbThreadCreation	= 0;


// -----------------------------
// Global methods
// -----------------------------
int pthread_create(pthread_t *pt, const pthread_attr_t *pthreadAttr, void *(*func)(void *), void *funcAttr)
{
	cpu_set_t cpuset;

	printf("*****************************\n");
	printf("Wrapper of the pthread create function\n");
	printf("*****************************\n");

	// Initialized the internal structures
	if (!initialized)
	{
		printf("----> Pthread (wrapper) initialization\n");
		initialized	= 1;
#ifdef NB_CPU
		nbCpu		= NB_CPU;
#else
		nbCpu		= DEFAULT_NB_CPU;
#endif
		// Pin the current thread to core 0
		pthread_t ptCurrent = pthread_self();
		CPU_ZERO(&cpuset);
		CPU_SET(0, &cpuset);
		int rc = proxy_pthread_setaffinity_np(ptCurrent, sizeof(cpu_set_t), &cpuset);
		if (rc != 0)
		{
			printf("Error calling pthread_setaffinity_np: %d\n", rc);
			exit(0);
		}
		nbThreadCreation = (nbThreadCreation + 1) % nbCpu;
	}

	// Create the required thread
	printf("-> Pthread (wrapper) creation: %d\n", nbThreadCreation);
	int res = proxy_pthread_create(pt, pthreadAttr, func, funcAttr);
	if (res < 0)
		return res;

	// Pin the created thread to a cpu (thread might start its execution before it is pinned)
	CPU_ZERO(&cpuset);
	CPU_SET(nbThreadCreation, &cpuset);
	int rc = proxy_pthread_setaffinity_np(*pt, sizeof(cpu_set_t), &cpuset);
	if (rc != 0)
	{
		printf("Error calling pthread_setaffinity_np: %d\n", rc);
		exit(0);
	}

	nbThreadCreation = (nbThreadCreation + 1) % nbCpu;
	return res;
}


