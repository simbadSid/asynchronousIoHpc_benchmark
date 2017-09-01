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



#ifndef PTHREAD_PROXY_h
#define PTHREAD_PROXY_h


#include <sys/types.h>





// -----------------------------
// Global methods
// -----------------------------
int proxy_pthread_create(pthread_t *pt, const pthread_attr_t *pthreadAttr, void *(*func)(void *), void *funcAttr);
int proxy_pthread_setaffinity_np(pthread_t thread, size_t cpusetsize, const cpu_set_t *cpuset);



#endif
