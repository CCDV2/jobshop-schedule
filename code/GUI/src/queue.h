// the <int> Queue by linklist
// require to init before using it.
// used in topological sort
#ifndef queue_h
#define queue_h


#include "src/datastructure.h"


extern void init(Queue *q);
extern void push(Queue *q, int x);
extern int front(Queue *q);
extern void pop(Queue *q);
extern int empty(Queue *q);



#endif
