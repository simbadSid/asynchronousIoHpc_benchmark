#include "queue.h"







// -----------------------------
// Builder
// -----------------------------
Queue::Queue()
{
	this->elem	= NULL;
	this->next	= NULL;
}
Queue::~Queue()
{
	ASSERT(	(this->elem	== NULL) &&
			(this->next	== NULL))
}


// -----------------------------
// Local methodes
// -----------------------------
void Queue::enqueue(void *ptrToEnqueue)
{
	if (this->elem == NULL)
		this->elem = ptrToEnqueue;
	else
	{
		Queue *newQueue = new Queue();
		newQueue->elem	= this->elem;
		newQueue->next	= this->next;
		this->elem		= ptrToEnqueue;
		this->next		= newQueue;
	}
}


void *Queue::dequeue()
{
	void *res = this->elem;

	if (res != NULL)
	{
		if (this->next == NULL)
			this->elem = NULL;
		else
		{
			Queue *tmp	= this->next;
			this->elem	= this->next->elem;
			this->next	= this->next->next;
			tmp->elem	= NULL;
			tmp->next	= NULL;
			delete tmp;
		}
	}
	return res;
}


char Queue::remove(void *ptrToRemove)
{
	Queue *tmp = this;
	char res = 0;

	if (this->elem == NULL)
		res = 0;
	else
	{
		do
		{
			if (tmp->elem == ptrToRemove)
			{
				ASSERT(ptrToRemove == tmp->dequeue());
				res = 1;
				break;
			}
			tmp = tmp->next;
		}while(tmp != NULL);
	}

	return res;
}


char Queue::isEmpty()
{
	return ((this->next == NULL) && (this->elem == NULL));
}


void Queue::printQueue()
{
	Queue *tmp = this;

	printf("    [ ");
	if (this->elem != NULL)
	{
		do
		{
			printf("%p%s", tmp->elem, (tmp->next!=NULL) ? ", " : " ");
			tmp = tmp->next;
		}while(tmp != NULL);
	}
	printf(" ]\n");

}

