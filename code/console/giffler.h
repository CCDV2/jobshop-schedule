// generate the initial/renewed solution for tabu search
// using scheduling rule

#ifndef giffler_h
#define giffler_h

#include <stdbool.h>

/**
 * update the time and number need to be updated next
 * @return true: still exist some to update
 *         false: all updated
 */
extern bool updateCurrentTime();

/**
 * generate the initial solution
 */
extern void findOneSoulution();

/**
 * push all the head vertex into caculation
 */
extern void initStartWork();

extern int max(int a, int b);

/**
 * insert current number's work into machine
 * regardless of conflict
 * @param _insertNo work number
 */
extern void insertIntoMachine(int _insertNo);

/**
 * check whether the insert exists conflict
 * if exists, readjust it
 * @param _insertNo work number
 */
extern void checkConflict(int _insertNo);

// not used
#ifdef NEED_MACHINE_EDGE
extern int findEdge(int _from, int _to);
#endif

/**
 * readjust conflict
 * @param _machineNo  machine number
 * @param _startIndex the conflict strat index (to length)
 */
extern void readjustConflict(int _machineNo, int _startIndex);

/**
 * readjust conflict
 * @param _machineNo  machine number
 * @param _startIndex the conflict start index (to length)
 */
extern void changePath(int _machineNo, int _startIndex);

/**
 * print the initial solution
 * check whether the initial solution is correct
 */
extern void printSolution();

/**
 * tabu search has reached the max unchanged step
 * thus to regenerate a solution for tabu search
 */
extern void findAnotherSolution();

/**
 * randomly generate a permutation from 1 to n
 * @param n literally meaning
 */
extern void randomPermutation(int n);

#endif
