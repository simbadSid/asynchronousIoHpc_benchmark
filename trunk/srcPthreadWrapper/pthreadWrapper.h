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



#ifndef PTHREAD_WRAPPER_h
#define PTHREAD_WRAPPER_h

#include <stdio.h>
#include <stdlib.h>
#include <sched.h>

#include "pthreadProxy.h"




#define DEFAULT_NB_CPU	4



// -----------------------------
// Global methods
// -----------------------------
int pthread_create(pthread_t *pt, const pthread_attr_t *pthreadAttr, void *(*func)(void *), void *funcAttr);


#endif
