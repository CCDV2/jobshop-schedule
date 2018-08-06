// authored by 邓昊元

// The data structure used in the program.

#ifndef datastructure_c
#define datastructure_c

#include"src/datastructure.h"

#include<stdbool.h>

const int FPS = 60;
const int TIME_INTERVAL = 1000 / FPS;
const int TIME_MULTIPLE = 60;

//the number of head/tail vertex of each job.
int headVertex[MAXN], tailVertex[MAXN];

int jobNumber, machineNumber;

//the data of each work
//the data of each work
typedef struct JobVertex JobVertex;
JobVertex jobVertex[MAXN * MAXM];
int jobVertexCount; //the total number of the work

// the data of each work's time
// seems some of the part  overlaps with the JobVertex
typedef struct JobEdge JobEdge;
JobEdge jobEdge[MAXN * MAXM + MAXN];
int jobEdgeCount;// the total number of the jobEdge
int *srcVex;// (not used)

// later found the MachineEdge is useless
// replaced by MachineSequence
#ifdef NEED_MACHINE_EDGE

struct MachineEdge
{
	int belong_machine;
	int to;
	int duration;
	int next_edge;
};
typedef struct MachineEdge MachineEdge;
MachineEdge machineEdge[MAXN * MAXN * MAXM];
int machineEdgeCount = 1;
int headEdge[MAXN * MAXM];

#endif

// record the sequence of the work in one machine
typedef struct MachineSequence MachineSequence;
MachineSequence machineSequence[MAXM]; // the current sequence in the iteration
MachineSequence bestSolution[MAXM]; // the best sequence in the iteration

int criticalPath[MAXN * MAXM]; // one of the critical paths in topological graph

// the score of one swap (calculated by evaluateSwap() )
typedef struct SwapScore SwapScore;
SwapScore swapScore[MAXN * MAXM];

// the tabu list
// stored in queue
typedef struct TabuList TabuList;
TabuList *tabuList;
int tabuLength, currentTabuLength; // tabuLength is calculated at start (const)
TabuList *tabuHead, *tabuTail; // the front and the back of the list

// record the best solution in each iteration
struct SwapList swapList;


// used in topological graph
// some parts of it overlap with the jobVertex
typedef struct TopoVertex TopoVertex;
TopoVertex topoVertex[MAXN * MAXM];


int randomSequence[MAXN]; // a random permutation from 1 to n

int currentT = 0; //current time in the generation
int currentNo; //current time's work number in generation

// change the method of generating
// initStartWork() and readjustConflict() will be changed
bool isInitialSolution = true;

// longestTime refers to the current solution's longestTime
// smallestTime refers to the smallest time among all the longestTime
int longestTime, smallestTime = 0x7fffffff;

 // the smallest score among the neighbourhood in an interation
 // the corresponding swap will be used
int smallestScore;

// the score of current solution
// in fact, originalScore == longestTime
int originalScore;

// Recorded refers to whether a solution is recorded
// existUndisabledSolution refers to
// whether an undisabled solution is recorded
bool Recorded, existUndisabledSolution;
// save colors of each job
Color color[MAXN];
// count the FPS
int paintCount;
// save the belong job of every FPS
int paintNumber[MAXM][MAXT];
// repair time on each machine
RepairInterval repairList[MAXM];


#endif
