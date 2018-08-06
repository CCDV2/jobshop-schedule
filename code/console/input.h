// The input of the program
// to store the data into the data structure

#ifndef input_h
#define input_h

#include "datastructure.h"

/**
 * main function in input
 */
extern void inputData();

/**
 * connect the super source vertex with each headVertex
 * actually this function is not used
 */
extern void buildHeadJobEdge();

/**
 * connect the super sink vertex with each tailVertex
 * acctually this function is not used, too
 */
extern void buildTailJobEdge();

/**
 * connect the current vertex with the next vertex (same job)
 * @param _jobnum   the number of the belonged job
 * @param _duration the time of this work
 */
extern void buildJobEdge(int _jobnum, int _duration);

/**
 * record the current vertex's info
 * @param _machineNum the number of the belonged machine
 * @param _jobnum     the number of the belonged job
 */
extern void buildJobVertex(int _machineNum ,int _jobnum);

/**
 * record the super source vertex's info
 */
extern void buildHeadJobVertex();

/**
 * record the super sink vertex's info
 */
extern void buildTailJobVertex();

// not used
#ifdef NEED_MACHINE_EDGE


struct Work
{
	int workNo;
	int duration;
};

struct TempMachineGroup
{
	struct Work work[MAXM];
	int machineTotalWork;
} *temp;

extern void initTemp(struct TempMachineGroup **p);
extern void insertTemp(int _machine, int _duration, int _workNo);

extern void viewTemp();
extern void buildMachineEdge(int _from, int _to, int _duration, int _belong_machine);

/**
 * to test if the input is correct
 */
extern void printGraph();

#endif

#endif
