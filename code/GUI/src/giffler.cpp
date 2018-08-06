// authored by 邓昊元

// generate the initial/renewed solution for tabu search
// using scheduling rule

#ifndef giffler_c
#define giffler_c

#include "src/datastructure.h"
#include "src/giffler.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

extern int jobNumber;
extern int machineNumber;


/**
 * update the time and number need to be updated next
 * @return true: still exist some to update
 *         false: all updated
 */
bool updateCurrentTime()
{
	int minTime = 0x7fffffff, minNo;
	for(int i = 1; i <= jobNumber; ++i)
	{
		//printf("head %d\n", headVertex[i]);
		for(int t = headVertex[i]; t != jobVertexCount + 1;
			 	t = jobEdge[t].next_work)
		{
			if(t != headVertex[i] && !jobVertex[t - 1].visited) break;

			int completeTime = jobVertex[t].current_time
							   + jobEdge[t].duration;
			//printf("%d completeTime %d\n", t, completeTime);
			if(!jobVertex[t].visited)
			{
				if(completeTime >= currentT)
				{
					if(minTime > completeTime)
					{
						minTime = completeTime;
						minNo = t;
					}
				}
				else
				{
					perror("unexpected result\n");
					jobVertex[t].visited = true;
					currentT = completeTime;
					currentNo = t;
					return true;
				}
			}
		}
	}
	if(minTime == 0x7fffffff)
	{
		// puts("!!fasle");
		return false;
	}

	currentT = minTime;
	currentNo = minNo;
	jobVertex[currentNo].visited = true;
	// printf("currentT %d, currentNo %d\n", currentT, currentNo);

	return true;
}

/**
 * generate the initial solution
 */
void findOneSolution()
{
	initStartWork();
	while(updateCurrentTime())
	{
		if(jobEdge[currentNo].next_work == jobVertexCount + 1)
		{
			// printf("reached the tail\n");
			continue;
		}
		int jobsuccessor = currentNo + 1;
		insertIntoMachine(jobsuccessor);
		checkConflict(jobsuccessor);
	}
}

/**
 * push all the head vertex into caculation
 */
void initStartWork()
{
	if(isInitialSolution)
	{
		for(int i = 1; i <= jobNumber; ++i)
		{
			insertIntoMachine(headVertex[i]);
			checkConflict(headVertex[i]);
		}
	}
	else // !isInitialSolution
	{
		for(int i = 1; i <= jobNumber; ++i)
		{
			insertIntoMachine(headVertex[randomSequence[i]]);
			checkConflict(headVertex[randomSequence[i]]);
		}
	}
}

int max(int a, int b)
{
	return a > b ? a : b;
}


/**
 * insert current number's work into machine
 * regardless of conflict
 * @param _insertNo work number
 */
void insertIntoMachine(int _insertNo)
{
	MachineSequence *p = &machineSequence[jobVertex[_insertNo].belong_machine];
	p->no[++(p->length)] = _insertNo;
	int jp = 0;
	int l = p->length;
	if(p->no[l] != headVertex[jobVertex[p->no[l]].belong_job])
	{
		jp = jobVertex[p->no[l] - 1].current_time
			+ jobEdge[p->no[l] - 1].duration;
	}
	jobVertex[_insertNo].current_time = jp;
}

/**
 * check whether the insert exists conflict
 * if exists, readjust it
 * @param _insertNo work number
 */
void checkConflict(int _insertNo)
{
 	MachineSequence *p = &machineSequence[jobVertex[_insertNo].belong_machine];
	int curTime = jobVertex[_insertNo].current_time;
	int l = p->length;

	for(int i = 1; i <= l - 1; ++i)
	{
		if(jobVertex[p->no[i]].current_time
		   + jobEdge[p->no[i]].duration >
		   curTime
	   )
	   {
		   for(int j = l - 1; j >= i; --j)
		   {
			   if(jobVertex[p->no[l]].belong_machine ==
			   	  jobVertex[p->no[j]].belong_machine)
				  {
					  readjustConflict(jobVertex[_insertNo].belong_machine, j + 1);
					  return;
				  }
		   }
		   readjustConflict(jobVertex[_insertNo].belong_machine, i);
		   return;
	   }
	}
	return;
}

//not used
#ifdef NEED_MACHINE_EDGE
int findEdge(int _from, int _to)
{
	for(int i = headEdge[_from]; i; i = machineEdge[i].next_edge)
	{
		if(machineEdge[i].to == _to) return i;
	}
	perror("no edge found\n");
	return -1;
}
#endif

/**
 * readjust conflict
 * @param _machineNo  machine number
 * @param _startIndex the conflict strat index (to length)
 */
void readjustConflict(int _machineNo, int _startIndex)
{
	// printf("readjustConflict %d %d\n", _machineNo, _startIndex);
	MachineSequence *p = &machineSequence[_machineNo];
	int l = p->length;
	//rand() is ok, too
	if(isInitialSolution)
	{
		int restWork = tailVertex[jobVertex[p->no[l]].belong_job] - p->no[l];
		for(int i = _startIndex; i <= l - 1; ++i)
		{
			int curRestWork = tailVertex[jobVertex[p->no[i]].belong_job] - p->no[i];
			// printf("%d-------\n", curRestWork);
			/**
			 * the scheduling rule
			 * @param restWork number of rest work to be finished
			 */
			if(curRestWork <= restWork)
			{
				changePath(_machineNo, i);
				return;
			}
		}
		changePath(_machineNo, l);
	}
	else // !isInitialSolution
	{
		// printf("start %d end %d\n", _startIndex, l);
		int randomStart = rand() % (l - _startIndex + 1) + _startIndex;
		// printf("%d %d\n", _machineNo, randomStart);
		changePath(_machineNo, randomStart);
	}
	return;
}

/**
 * readjust conflict
 * @param _machineNo  machine number
 * @param _startIndex the conflict start index (to length)
 */
void changePath(int _machineNo, int _startIndex)
{
	MachineSequence *p = &machineSequence[_machineNo];
	int l = p->length;
	int changeNo = p->no[l];
	//swap the sequence
	for(int i = l - 1; i >= _startIndex; --i)
	{
		p->no[i + 1] = p->no[i];
	}
	p->no[_startIndex] = changeNo;

	//readjust the time
	for(int i = _startIndex; i <= l; ++i)
	{
		int mp = 0, jp = 0;
		if(i != 1)
		{
			mp = jobVertex[p->no[i - 1]].current_time
				+ jobEdge[p->no[i - 1]].duration;
		}
		if(p->no[i] != headVertex[jobVertex[p->no[i]].belong_job])
		{
			jp = jobVertex[p->no[i] - 1].current_time
				+ jobEdge[p->no[i] - 1].duration;
		}
		jobVertex[p->no[i]].current_time = max(mp, jp);
	}
}

/**
 * print the initial solution
 * check whether the initial solution is correct
 */
void printSolution()
{
	puts("-------------------------------------");
	puts("print the path");
	for(int i = 0; i < machineNumber; ++i)
	{
		MachineSequence *p = &machineSequence[i];
		for(int j = 1; j <= p->length; ++j)
		{
			printf("%d-->", p->no[j]);
		}
		putchar('\n');
	}
	puts("---------------------------------------");
	puts("print the time");
	for(int i = 0; i < machineNumber; ++i)
	{
		MachineSequence *p = &machineSequence[i];
		for(int j = 1; j <= p->length; ++j)
		{
			printf("%d-->", jobVertex[p->no[j]].current_time);
		}
		putchar('\n');
	}
	puts("--------------------------------------------");
	puts("print the gantt chart");
	for(int i = 0; i < machineNumber; ++i)
	{
		MachineSequence *p = &machineSequence[i];
		for(int t = 1; t <= jobVertex[p->no[1]].current_time; ++t)
		{
			printf("%-3c", '-');
		}
		for(int j = 1; j <= p->length - 1; ++j)
		{
			for(int t = 1; t <= jobEdge[p->no[j]].duration; ++t)
			{
				printf("%-3d", p->no[j]);
			}
			for(int t = 1; t <= jobVertex[p->no[j + 1]].current_time
							- jobVertex[p->no[j]].current_time
							- jobEdge[p->no[j]].duration; ++t)
			{
				printf("%-3c", '-');
			}
		}
		for(int t = 1; t <= jobEdge[p->no[p->length]].duration; ++t)
		{
			printf("%-3d", p->no[p->length]);
		}
		for(int t = 1; t <= currentT
							- jobVertex[p->no[p->length]].current_time
							- jobEdge[p->no[p->length]].duration; ++t)
		{
			printf("%-3c", '-');
		}
		putchar('\n');
	}
}

/**
 * tabu search has reached the max unchanged step
 * thus to regenerate a solution for tabu search
 */
void findAnotherSolution()
{
	// puts("regenerating..");
	for(int i = 1; i <= jobVertexCount; ++i)
	{
		jobVertex[i].current_time = 0;
		jobVertex[i].visited = false;
	}
	memset(machineSequence, 0, sizeof(machineSequence));
	currentT = 0;
	currentNo = 0;

	// change the method of generating
	isInitialSolution = false;
	randomPermutation(jobNumber);

	findOneSolution();
	// puts("done!");
	// printf("%d\n", currentT);
}

/**
 * randomly generate a permutation from 1 to n
 * @param n literally meaning
 */
void randomPermutation(int n)
{
	for(int i = 1; i <= n; ++i) randomSequence[i] = i;
	for(int i = 1; i <= n - 1; ++i)
	{
		int t = rand() % (n - i + 1) + i;
		int temp = randomSequence[t];
		randomSequence[t] = randomSequence[i];
		randomSequence[i] = temp;
	}
	// for(int i = 1; i <= n; ++i) printf("%d ", randomSequence[i]);
	// putchar('\n');
}

#endif
