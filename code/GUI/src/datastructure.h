#ifndef datastructure_h
#define datastructure_h


#define MAXN 35 //max number of job
#define MAXM 35 //max number of machine
#define MAXT 2000
extern const int FPS;
extern const int TIME_INTERVAL;
extern const int TIME_MULTIPLE;
#include<stdbool.h>

//the number of head/tail vertex of each job.
extern int headVertex[MAXN], tailVertex[MAXN];

extern int jobNumber, machineNumber;

//the data of each work
struct JobVertex
{
	int belong_machine;
	int belong_job;

	int current_time; //the earliest time in topological order

	int visited; // whether this vertex is put in the correct place
};
typedef struct JobVertex JobVertex;
extern struct JobVertex jobVertex[MAXN * MAXM];
extern int jobVertexCount; //the total number of the work

// the data of each work's time
// seems some of the part  overlaps with the JobVertex
struct JobEdge
{
	int belong_job;
	int next_work;
	int duration;
};
typedef struct JobEdge JobEdge;
extern struct JobEdge jobEdge[MAXN * MAXM + MAXN];
extern int jobEdgeCount;// the total number of the jobEdge
extern int *srcVex;// (not used)

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
struct MachineSequence
{
	int length;
	int no[MAXN];
};
typedef struct MachineSequence MachineSequence;
extern struct MachineSequence machineSequence[MAXM]; // the current sequence in the iteration
extern struct MachineSequence bestSolution[MAXM]; // the best sequence in the iteration

extern int criticalPath[MAXN * MAXM]; // one of the critical paths in topological graph

// the score of one swap (calculated by evaluateSwap() )
struct SwapScore
{
	int fromStart;
	int toEnd;
};
typedef struct SwapScore SwapScore;
extern struct SwapScore swapScore[MAXN * MAXM];

// the tabu list
// stored in queue
struct TabuList
{
	int machineNo;
	int startIndex; //the start of the disabled index
	int endIndex;
	int length; // the length between the startIndex and the endIndex
	int *no; // record the number in the disabled index
	struct TabuList *next; // next element in tabu list
};
typedef struct TabuList TabuList;
extern int tabuLength, currentTabuLength; // tabuLength is calculated at start (const)
typedef struct TabuList TabuList;
extern TabuList *tabuList;
extern TabuList *tabuHead, *tabuTail; // the front and the back of the list

// record the best solution in each iteration
struct SwapList
{
	int machineNo;
	int startIndex;
	int endIndex;
	int STATE;
	int *no;
};
extern struct SwapList swapList;

// belong to the queue
struct Node
{
	int data;
	struct Node *next;
};
typedef struct Node Node;

// used in topological graph
struct Queue
{
	Node *head;
	Node *tail;
};
typedef struct Queue Queue;

// used in topological graph
// some parts of it overlap with the jobVertex
struct TopoVertex
{
	int earliestTime; // calculated by topological sort
	int latestTime; // calculated by reverse topological sort

	// refers to the number of predecessor/successor
	// of the same job/machine
	int jp, mp, js, ms;
	int visited;

	int critical; //bool variable. whether the vertex is a critical vertex
} ;
typedef struct TopoVertex TopoVertex;
extern struct TopoVertex topoVertex[MAXN * MAXM];


extern int randomSequence[MAXN]; // a random permutation from 1 to n

extern int currentT; //current time in the generation
extern int currentNo; //current time's work number in generation

// change the method of generating
// initStartWork() and readjustConflict() will be changed
extern bool isInitialSolution;

// longestTime refers to the current solution's longestTime
// smallestTime refers to the smallest time among all the longestTime
extern int longestTime, smallestTime;

 // the smallest score among the neighbourhood in an interation
 // the corresponding swap will be used
extern int smallestScore;

// the score of current solution
// in fact, originalScore == longestTime
extern int originalScore;

// Recorded refers to whether a solution is recorded
// existUndisabledSolution refers to
// whether an undisabled solution is recorded
extern bool Recorded, existUndisabledSolution;

// save colors of each job
struct Color{
	int r, g, b;
	Color() {}
	Color(int _r, int _g, int _b): r(_r), g(_g), b(_b) {}
};
extern Color color[MAXM];
// count the FPS
extern int paintCount;
// save the belong job of every FPS
extern int paintNumber[MAXM][MAXT];

// repair time on each machine
struct RepairInterval
{
	int startTime;
	int endTime;
};
extern RepairInterval repairList[MAXM];

#endif
