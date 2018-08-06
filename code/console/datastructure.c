// The data structure used in the program.

#ifndef datastructure_c
#define datastructure_c

#define MAXN 35 //max number of job
#define MAXM 35 //max number of machine

//the number of head/tail vertex of each job.
int headVertex[MAXN], tailVertex[MAXN];

//the data of each work
struct JobVertex
{
	int belong_machine;
	int belong_job;

	int current_time; //the earliest time in topological order

	int visited; // whether this vertex is put in the correct place
};
typedef struct JobVertex JobVertex;
JobVertex jobVertex[MAXN * MAXM];
int jobVertexCount; //the total number of the work

// the data of each work's time
// seems some of the part  overlaps with the JobVertex
struct JobEdge
{
	int belong_job;
	int next_work;
	int duration;
};
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
struct MachineSequence
{
	int length;
	int no[MAXN];
};
typedef struct MachineSequence MachineSequence;
MachineSequence machineSequence[MAXM]; // the current sequence in the iteration
MachineSequence bestSolution[MAXM]; // the best sequence in the iteration

int criticalPath[MAXN * MAXM]; // one of the critical paths in topological graph

// the score of one swap (calculated by evaluateSwap() )
typedef struct SwapScore
{
	int fromStart;
	int toEnd;
} SwapScore;
SwapScore swapScore[MAXN * MAXM];

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
} *tabuList;
int tabuLength, currentTabuLength; // tabuLength is calculated at start (const)
typedef struct TabuList TabuList;
TabuList *tabuHead, *tabuTail; // the front and the back of the list

// record the best solution in each iteration
struct SwapList
{
	int machineNo;
	int startIndex;
	int endIndex;
	int STATE;
	int *no;
};
struct SwapList swapList;

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
TopoVertex topoVertex[MAXN * MAXM];


int randomSequence[MAXN]; // a random permutation from 1 to n


#endif
