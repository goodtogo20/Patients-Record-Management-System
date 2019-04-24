#include <stdio.h>
#include "pqueue.h"

void main()
{
    struct queue *test =create_queue(20);
    enqueue(test,10);
    enqueue(test,20);
    enqueue(test,30);

    for(int i = 0; i < test->size; i++)
    {
        printf("field: %d \n",test->array[i]);
    }
    dequeue(test);
    printf("\n");
    for(int i = 0; i < test->size; i++)
    {
        printf("field: %d \n",test->array[i]);
    }
}
