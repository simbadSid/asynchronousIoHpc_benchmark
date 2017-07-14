#ifndef HANDLER_WRITE_PARAM_H
#define HANDLER_WRITE_PARAM_H

#include "../auxiliary/queue_cs.h"



/**
 * \brief Class used to store the parameters of the handler raised when
 *  a write request has been effectively processed (Worker::handlerWrite).
 *  \details The main purpose is to remove them after the execution of the notification handler (Worker::handlerWrite)
 */

class HandlerWriteParam
{
public:
// --------------------------
// Attribute
// --------------------------
	char				*output;							/** <Buffer used by the aio_write function.  Maybe included into param.   But needed for free*/
	void				*param;								/** <Parameter of the aio_write (depends on the used AIO implementation)
	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 ex: struct aiocb for the gLibc implementation*/
	Queue_CS			*pendingAioRequest;
	Synchronization		*synchronization;
	unsigned int		nbOutputChar;


// --------------------------
// Builder
// --------------------------
	HandlerWriteParam(char *output, void* param, Queue_CS *pendingAioRequest, Synchronization *synchronization, unsigned int nbOutputChar);
	~HandlerWriteParam();
};




#endif
