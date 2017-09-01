#ifndef QUEUE_CS_H
#define QUEUE_CS_H

#include "util.h"
#include "queue.h"
#include "synchronization.h"





class Queue_CS
{
// -----------------------------
// Attributes
// -----------------------------
	private:
		Synchronization	*synchronization;
		Queue			*queue;


// -----------------------------
// Builder
// -----------------------------
	public:
		Queue_CS();
		~Queue_CS();


// -----------------------------
// Local methods
// -----------------------------
	public:
		/**
		 * \brief Enqueue the parameter \"ptrToEnqueue\" in the current queue.
		 * \details The opperation is thread-safe (critical section).
		 */
		void enqueue(void *ptrToEnqueue);


		/**
		 * \brief Returns the last enqueued element.
		 * \details The returned element is removed from the structure.
		 * \details The opperation is thread-safe (critical section).
		 * \returns Null if the queue is empty
		 */
		void *dequeue();


		/**
		 * \brief Remove the parameter \"ptrToRemove\" from the current queue.
		 * \details The opperation is thread-safe (critical section).
		 * \returns True (1) if the element was found in the queue.
		 * \parameter ptrToRemove        : pointer to find and remove from queue
		 * \parameter notify             : Indicates whether or not to inform the function \"waitTillEmpty\" that the queue might be empty.
		 *                                 If this parameter is false is specified, the notification might be done latter through the output parameter \"pipeToNotify\".   In this case and if the notification is not done by the caller, the function \"waitTillEmpty\" will stall however the queue is empty.
		 * \parameter pipeToNotifyLatter : Output parameter that allows to notify the function \"waitTillEmpty\".   It allows to delay the return of that function.   This parameter is only set if the parameter \"notify\" is set to false.
		 */
		char remove(void *ptrToRemove, char notify, Synchronization **pipeToNotifyLatter);


		void waitTillEmpty();

		void printQueue();
};



#endif
