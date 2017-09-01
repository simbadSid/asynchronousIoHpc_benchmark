#ifndef QUEUE_H
#define QUEUE_H

#include <stdlib.h>
#include "util.h"



class Queue
{
// -----------------------------
// Attributes
// -----------------------------
	private:
		void	*elem;
		Queue	*next;


// -----------------------------
// Builder
// -----------------------------
	public:
		Queue();
		~Queue();


// -----------------------------
// Local methods
// -----------------------------
	public:
		/**
		 * \brief Enqueue the parameter \"ptrToEnqueue\" in the current queue.
		 */
		void enqueue(void *ptrToEnqueue);


		/**
		 * \brief Returns the last enqueued element.
		 * \details The returned element is removed from the structure.
		 * \returns Null if the queue is empty
		 */
		void *dequeue();


		/**
		 * \brief Remove the parameter \"ptrToRemove\" from the current queue.
		 * \returns True (1) if the element was found in the queue.
		 */
		char remove(void *ptrToRemove);


		char isEmpty();
		void printQueue();

};


#endif
