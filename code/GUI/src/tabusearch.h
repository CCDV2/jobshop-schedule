// the tabu search algorithm
// get initial/renewed solution by scheduling rule

#ifndef tabusearch_h
#define tabusearch_h

#include"mainwidget.h"
#include "src/datastructure.h"
#include <stdbool.h>

// parameter used in swap
#define FORWARD_SHIFT 1
#define BACKWARD_SHIFT 2

// parameter used in recording the best solution
#define BEST_TO_MACHINE 1
#define MACHINE_TO_BEST 2


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




/**
 * calculate the length of the tabu list
 */
 void initTabuList();

/**
 * push a sequence into the tabu list
 * @param _machineNo machine number
 * @param l          start tabu-ed index
 * @param r          end tabu-ed index
 */
 void insertTabuList(int _machineNo, int l, int r);

/**
 * check whether a sequence is in tabu list
 * @param  _machineNo machine number
 * @param  l          start tabu-ed/swap index
 * @param  r          end tabu-ed/swap index
 * @return            true: in
 * 					  false: not in
 */
 bool isinTabuList(int _machineNo, int l, int r);

/**
 * clear the tabu list
 * used in the end
 */
 void clearTabuList();




/**
 * the main function in tabu search
 * iterate here
 */
void solve(MainWidget *from);

/**
 * get the index of a work in its machine sequence
 * @param  _machine machine number
 * @param  _no      work number
 * @return          index
 */
 int findSequenceIndex(int _machine, int _no);

/**
 * find all critical paths
 * implementation in topograph.c
 */
 extern void findCriticalPath(int t, int _index);

/**
 * find the block in the critical path
 * @param _length length of critical path
 */
void findBlock(int _length);

/**
 * check whether a work is in critical path
 * @param  _no     work number
 * @param  _length length of critical path
 * @return         true: in  false: not
 */
bool isInCriticalPath(int _no, int _length);

/**
 * insert r into the front of l
 * @param _machineNo machine number
 * @param l          start index
 * @param r          end index
 */
void shiftForward(int _machineNo, int l, int r);

/**
 * insert l into the back of r
 * @param _machineNo machine number
 * @param l          start index
 * @param r          end index
 */
void shiftBackward(int _machineNo, int l, int r);

/**
 * evaluate whether a swap is good enough
 * @param _machineNo machine number
 * @param l          start index
 * @param r          end index
 * @param STATE      backward/forward
 */
void evaluateSwap(int _machineNo, int l, int r, int STATE);

/**
 * calculate the swap score from start
 * @param _machineNo machine number
 * @param l          start index
 * @param r          end index
 * @param STATE      backward/forward
 */
void getLengthFromStart(int _machineNo, int l, int r, int STATE);

/**
 * calculate the swap score to end
 * @param _machineNo machine number
 * @param l          start index
 * @param r          end index
 * @param STATE      backward/forward
 */
void getLengthToEnd(int _machineNo, int l, int r, int STATE);

/**
 * if the swap is good enough (than ever), record it
 * @param _machineNo machine number
 * @param l          start index
 * @param r          end index
 * @param STATE      backward/forward
 */
void recordSwapList(int _machineNo, int l, int r, int STATE);

/**
 * copy the current best solution into bestSolution
 * or copy the overall best solution into machineSequence
 * @param STATE BEST_TO_MACHINE/MACHINE_TO_BEST
 */
void copySolution(int STATE);

/**
 * copy the best solution
 * reset the jobVertex
 */
void finalSolution();

/**
 * for test
 * check whether some move is illegal
 */
void existConflict();

/**
 * tabu search has reached the max unchanged step
 * thus to regenerate a solution for tabu search
 * implementation in giffler.c
 */
extern void findAnotherSolution();





#endif
