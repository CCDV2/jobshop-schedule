// the <int> Queue by linklist
// require to init before using it.
// used in topological sort
#ifndef queue_c
#define queue_c

#include "queue.h"
#include <stdlib.h>

void init(Queue *q)
{
	q->head = NULL;
	q->tail = NULL;
}

void push(Queue *q, int x)
{
	if(q->head == NULL)
	{
		q->head = (Node *)malloc(sizeof(Node));
		if(q->head == NULL)
		{
			perror("malloc failed\n");
			exit(-1);
		}
		q->tail = q->head;
	}
	else
	{
		q->tail->next = (Node *)malloc(sizeof(Node));
		q->tail = q->tail->next;
	}
	q->tail->data = x;
	q->tail->next = NULL;
}

int front(Queue *q)
{
	if(q->head == NULL)
	{
		perror("queue is empty\n");
		return 0x80000000;
	}
	return q->head->data;
}

void pop(Queue *q)
{
	Node *del = q->head;
	q->head = q->head->next;
	free(del);
}

int empty(Queue *q)
{
	return q->head == NULL;
}

#endif
