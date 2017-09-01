#include "handlerWriteParam.h"






HandlerWriteParam::HandlerWriteParam(char *output, void* param, Queue_CS *pendingAioRequest, Synchronization *synchronization, unsigned int nbOutputChar)
{
	this->output				= output;
	this->param					= param;
	this->pendingAioRequest		= pendingAioRequest;
	this->synchronization		= synchronization;
	this->nbOutputChar			= nbOutputChar;
}


HandlerWriteParam::~HandlerWriteParam()
{
	free(this->output);
	free(this->param);
}








