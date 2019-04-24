#ifndef QUEUE
#define QUEUE 1

struct queue
{
	int head, tail, size, cap;
	int array[100];
};

struct queue* create_queue(int cap);

int isFull(struct queue *q);

int isEmpty(struct queue *q);

int enqueue(struct queue *q, int item);

int dequeue(struct queue *q);

#endif
