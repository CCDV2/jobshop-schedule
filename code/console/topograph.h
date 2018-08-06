// get all the critical path in the graph
// part of the tabu search

#ifndef topograph_h
#define topograph_h

#include "datastructure.c"
#include "queue.c"

extern int max(int, int);
extern int min(int a, int b);

/**
 * reset all the topological vertex's visited condtion into false
 */
extern void initTopoVertex();

/**
 * connect to current vertex's jp, js, mp, ms
 * @param i current vertex
 */
extern void connectTopoVertex(int i);

/**
 * get the earliest time of each vertex in topological sort
 */
extern void getEarliest();

/**
 * get the latest time of each vertex in reverse topological sort
 */
extern void getLatest();

/**
 * get the index of a work in its machine sequence
 * implementation in tabusearch.c
 */
extern int findSequenceIndex(int _machine, int _no);

/**
 * scan through the topological graph.
 * get the critical vertex
 */
extern void findCriticalVertex();

/**
 * find all the critical path by dfs
 * @param t      current work number
 * @param _index current depth/index in criticalPath
 */
extern void findCriticalPath(int t, int _index);

/**
 * the interface with tabusearch
 * find block in the critical path
 * implementation in tabu search
 */
extern void findBlock(int _length);

/**
 * print the topograph
 * test whether all the calculation above is correct
 */
extern void printTopoGraph();

/**
 * print the connection of each vertex
 * test whether all the calculatoin above is correct
 */
extern void printConnection();

#endif
