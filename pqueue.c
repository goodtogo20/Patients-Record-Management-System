#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include "pqueue.h"

//dynamically allocates memory and returns it, also does intialisation
struct queue* create_queue(int cap)
{
	struct queue* q = (struct queue*) malloc(sizeof(struct queue));
	q->head = 0 ;
	q->tail = 0 ;
	q->size = 0 ;
	q->cap  = cap;
	return q;
}

//Checks if queue is full, if full returns one
int isFull(struct queue *q)
{
	if(q->cap  == q->size)
	{
		return 1;
	} else
	{
		return 0;
	}
}

//Checks if queue is empty, if empty returns one
int isEmpty(struct queue *q)
{
	if(q->head == q->tail)
	{
		return 1;
	} else
	{
		return 0;
	}
}

//Adds element to queue at tail, increases size by one
int enqueue(struct queue *q, int item)
{
	if(isFull(q))
	{
		printf("\nenqueue opertion aborted! : queue is full");
		return 1;
	}

	q->array[q->tail] = item;
	q->tail++;
	q->size++;

	if(q->tail >= (q->cap+1))
	{
		q->tail = 0;
	}
	//printf("\n%d is enqueued",item);
}

//Removes element at head from queue, decreases size by one
int dequeue(struct queue *q)
{
	int th;

	if(isEmpty(q))
	{
		printf("\ndequeue opertion aborted! : queue is empty");
		return 1;
	}

	th = q->array[q->head];
	q->size--;

	if(q->head >= (q->cap+1))
	{
		q->head = 0;
	}

	//printf("\n%d is dequeued",q->array[q->head]);
	q->head++;
	return th;
}
