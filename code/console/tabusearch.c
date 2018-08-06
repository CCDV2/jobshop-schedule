// the tabu search algorithm
// get initial/renewed solution by scheduling rule

#ifndef tabusearch_c
#define tabusearch_c

#include "tabusearch.h"
#include "datastructure.c"
#include "topograph.c"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

extern int currentT;
extern int jobNumber, machineNumber;
extern int max(int, int);
extern const int multiple;

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

/**
 * calculate the length of the tabu list
 */
void initTabuList()
{
	int min_size = 10 + jobNumber / machineNumber;
	int max_size;
	if(jobNumber <= machineNumber * 2) max_size = 1.4 * min_size;
	else max_size = 1.5 * min_size;
	tabuLength = rand() % (max_size - min_size + 1) + min_size;
}

/**
 * push a sequence into the tabu list
 * @param _machineNo machine number
 * @param l          start tabu-ed index
 * @param r          end tabu-ed index
 */
void insertTabuList(int _machineNo, int l, int r)
{
	MachineSequence *p = &machineSequence[_machineNo];
	if(currentTabuLength == tabuLength)
	{
		TabuList *del = tabuHead;
		tabuHead = tabuHead->next;
		free(del->no);
		free(del);
		del = NULL;
		--currentTabuLength;
	}
	++currentTabuLength;
	if(tabuHead == NULL)
	{
		tabuHead = (TabuList *)malloc(sizeof(TabuList));
		if(tabuHead == NULL)
		{
			perror("malloc failed\n");
			exit(-1);
		}
		tabuTail = tabuHead;
	}
	else
	{
		tabuTail->next = (TabuList *)malloc(sizeof(TabuList));
		if(tabuTail->next == NULL)
		{
			perror("malloc failed\n");
			exit(-1);
		}
		tabuTail = tabuTail->next;
	}
	tabuTail->machineNo = _machineNo;
	tabuTail->startIndex = l;
	tabuTail->endIndex = r;
	int size = r - l + 1;
	tabuTail->length = size;
	tabuTail->no = (int *)malloc(sizeof(int) * size);
	if(tabuTail->no == NULL)
	{
		perror("malloc failed\n");
		exit(-1);
	}
	for(int i = 0; i < size; ++i)
	{
		tabuTail->no[i] = p->no[l + i];
	}
	tabuTail->next = NULL;
}

/**
 * check whether a sequence is in tabu list
 * @param  _machineNo machine number
 * @param  l          start tabu-ed/swap index
 * @param  r          end tabu-ed/swap index
 * @return            true: in
 * 					  false: not in
 */
bool isinTabuList(int _machineNo, int l, int r)
{
	MachineSequence *p = &machineSequence[_machineNo];
	int current_length = r - l + 1;
	for(TabuList *t = tabuHead; t; t = t->next)
	{
		if(t->length == current_length && t->machineNo == _machineNo)
		{
			if(t->startIndex == l)
			{
				int i;
				for(i = 0; i < t->length; ++i)
				{
					if(t->no[i] != p->no[l + i]) break;
				}
				if(i == t->length) return true;
			}
		}
	}
	return false;
}

/**
 * clear the tabu list
 * used in the end
 */
void clearTabuList()
{
	while(tabuHead)
	{
		TabuList *del = tabuHead;
		tabuHead = tabuHead->next;
		if(del->no) free(del->no);
		free(del);
	}
}

/**
 * the main function in tabu search
 * iterate here
 */
void solve()
{
	copySolution(MACHINE_TO_BEST);
	initTabuList();
	int maxIterateStep = 15000, maxUnchangedStep = 350;//15000 200
	#ifdef ANOTHER_SEARCH_METHOD
	maxUnchangedStep = 400;
	#endif

	maxIterateStep *= multiple;

	int currentIterateStep = 0, currentUnchangedStep = 0;
	while(currentIterateStep <= maxIterateStep && maxUnchangedStep >= 0)
	{
		++currentIterateStep;
		// printf("iterating %d\n", currentIterateStep);
		smallestScore = 0x7fffffff;
		Recorded = false;
		existUndisabledSolution = false;

		// reached the max unchanged step
		// need to regenerate a solution for possible better solution
		if(currentUnchangedStep > maxUnchangedStep)
		{
		/**
		 * <!--readjust here-->
		 */
		readjust:
			// puts("----readjust-----");
			currentUnchangedStep = 0;
			// maxUnchangedStep -= 10;
			findAnotherSolution();
		}

		//belong to the topological graph
		initTopoVertex();
		getEarliest();
		longestTime = topoVertex[jobVertexCount + 1].earliestTime;
		currentT = longestTime;
		getLatest();
		findCriticalVertex();

		/*for debug
		if(currentIterateStep == 3)
		{
			printTopoGraph();
			// printConnection();
			existConflict();
		}
		 */

		// printf("  current longestTime: %d\n\n", longestTime);

		//to evaluate whether this move is better
		if(smallestTime <= longestTime)
		{
			++currentUnchangedStep;
			// printf("    unchanged %d\n", currentUnchangedStep);
		}
		else
		{
			if(calculatedVertexCount == jobVertexCount)
			{
				copySolution(MACHINE_TO_BEST);
				smallestTime = longestTime;
				currentUnchangedStep = 0;
			}
			else if(currentIterateStep != 1)
			{
				// puts("!!!");
				// printf("%d %d\n", longestTime, calculatedVertexCount);
				goto readjust;
			}
		}

		//generate new blocks and new solutions
		findCriticalPath(0, 0);

		// printf("the smallest score %d\n", smallestScore);

		//before swapping, disable the current solution
		insertTabuList(swapList.machineNo, swapList.startIndex, swapList.endIndex);

		/* for debug
		puts("---------the shift method is:");
		printf("machine: %d, l: %d, r: %d\n",
				swapList.machineNo,
				machineSequence[swapList.machineNo].no[swapList.startIndex],
				machineSequence[swapList.machineNo].no[swapList.endIndex]);
		puts(swapList.STATE == FORWARD_SHIFT ? "FORWARD" : "BACKWARD");
		*/

		// move into new solution
		if(swapList.STATE == FORWARD_SHIFT)
			shiftForward(swapList.machineNo, swapList.startIndex, swapList.endIndex);
		else//STATE == BACKWARD_SHIFT
			shiftBackward(swapList.machineNo, swapList.startIndex, swapList.endIndex);
	}

	finalSolution();
}

/**
 * get the index of a work in its machine sequence
 * @param  _machine machine number
 * @param  _no      work number
 * @return          index
 */
int findSequenceIndex(int _machine, int _no)
{
	MachineSequence *p = &machineSequence[_machine];
	for(int i = 1; i <= p->length; ++i)
	{
		if(p->no[i] == _no) return i;
	}
	perror("UB\n");
	return -1;
}

/**
 * find the block in the critical path
 * @param _length length of critical path
 */
void findBlock(int _length)
{
	int l, r;
	for(l = 1; l <= _length; l = r + 1)
	{
		for(r = l; r + 1 <= _length &&
			 jobVertex[criticalPath[r + 1]].belong_machine
			 == jobVertex[criticalPath[l]].belong_machine; ++r);
		// printf("block from %d(%d) to %d(%d)\n", l, criticalPath[l], r, criticalPath[r]);
		if(l == r) continue;

		int m = jobVertex[criticalPath[l]].belong_machine;

		int tl, tr;
		int ml, mr;

		ml = findSequenceIndex(m, criticalPath[l]);
		for(tl = l; tl <= r; ++tl, ++ml)
		{
			int no = criticalPath[tl];
			if(no != headVertex[jobVertex[no].belong_job] &&
				isInCriticalPath(no - 1, _length))
			{
				for(tr = tl + 1, mr = ml + 1; tr <= r; ++tr, ++mr)
				{
					// printf("  FORWARD_SHIFT l: %d r: %d\n", machineSequence[m].no[ml], machineSequence[m].no[mr]);
					evaluateSwap(m, ml, mr, FORWARD_SHIFT);

					#ifdef ANOTHER_SEARCH_METHOD
					if(existUndisabledSolution) return;
					#endif
				}
			}
		}

		mr = findSequenceIndex(m, criticalPath[r]);
		for(tr = r; tr >= l; --tr, --mr)
		{
			int no = criticalPath[tr];
			if(no != tailVertex[jobVertex[no].belong_job] &&
				isInCriticalPath(no + 1, _length))
			{
				for(tl = tr - 1, ml = mr - 1; tl >= l; --tl, --ml)
				{
					// printf("  BACKWARD_SHIFT l: %d r: %d\n", machineSequence[m].no[ml], machineSequence[m].no[mr]);
					evaluateSwap(m, ml, mr, BACKWARD_SHIFT);

					#ifdef ANOTHER_SEARCH_METHOD
					if(existUndisabledSolution) return;
					#endif
				}
			}
		}

	}
}

/**
 * check whether a work is in critical path
 * @param  _no     work number
 * @param  _length length of critical path
 * @return         true: in  false: not
 */
bool isInCriticalPath(int _no, int _length)
{
	for(int i = 1; i <= _length; ++i)
	{
		if(_no == criticalPath[i]) return true;
	}
	return false;
}

/**
 * insert r into the front of l
 * @param _machineNo machine number
 * @param l          start index
 * @param r          end index
 */
void shiftBackward(int _machineNo, int l, int r)
{
	MachineSequence *p = &machineSequence[_machineNo];
	int tempMachineNo = p->no[l];
	for(int i = l + 1; i <= r; ++i)
	{
		p->no[i - 1] = p->no[i];
	}
	p->no[r] = tempMachineNo;
}

/**
 * insert l into the back of r
 * @param _machineNo machine number
 * @param l          start index
 * @param r          end index
 */
void shiftForward(int _machineNo, int l, int r)
{
	MachineSequence *p = &machineSequence[_machineNo];
	int tempMachineNo = p->no[r];
	for(int i = r - 1; i >= l; --i)
	{
		p->no[i + 1] = p->no[i];
	}
	p->no[l] = tempMachineNo;
}

/**
 * evaluate whether a swap is good enough
 * @param _machineNo machine number
 * @param l          start index
 * @param r          end index
 * @param STATE      backward/forward
 */
void evaluateSwap(int _machineNo, int l, int r, int STATE)
{
	int score = 0x80000000;

	getLengthFromStart(_machineNo, l, r, STATE);
	getLengthToEnd(_machineNo, l, r, STATE);

	MachineSequence *p = &machineSequence[_machineNo];
	for(int i = l; i <= r; ++i)
	{
		score = max(score,
			 swapScore[p->no[i]].fromStart + swapScore[p->no[i]].toEnd);
	}
	// printf("the score of l: %d r: %d is %d\n", p->no[l], p->no[r], score);
	// printf("state %d, %d %d length %d\n", STATE, l, r, r - l);


	bool inList = false;
	if(STATE == FORWARD_SHIFT)
	{
		shiftForward(_machineNo, l, r);
		inList = isinTabuList(_machineNo, l, r);
		shiftBackward(_machineNo, l, r);
	}
	else
	{
		shiftBackward(_machineNo, l, r);
		inList = isinTabuList(_machineNo, l, r);
		shiftForward(_machineNo, l, r);
	}
	if(inList)
	{
		// puts("oops");
		// conform to special undisable rule
		if(!existUndisabledSolution && score < longestTime)
		{
			Recorded = true;
			// smallestScore = score;
			// puts("  but record");
			// printf("    %d\n", score);
			recordSwapList(_machineNo, l, r, STATE);
		}
		return;
	}
	else
	/* !inList */
	#ifndef ANOTHER_SEARCH_METHOD
	if(score < smallestScore)
	{
		Recorded = true;
		existUndisabledSolution = true;
		smallestScore = score;
		// printf("  score %d\n", score);
		// printf("record %d, l: %d, r: %d\n", _machineNo, p->no[l], p->no[r]);
		recordSwapList(_machineNo, l, r, STATE);
		return;
	}
	#else
	if(score < longestTime)
	{
		// puts("findone");
		Recorded = true;
		existUndisabledSolution = true;
		smallestScore = score;
		// printf("  score %d\n", score);
		// printf("record %d, l: %d, r: %d\n", _machineNo, p->no[l], p->no[r]);
		recordSwapList(_machineNo, l, r, STATE);
		return;
	}
	else if(score < smallestScore)
	{
		smallestScore = score;
		recordSwapList(_machineNo, l, r, STATE);
		return;
	}

	#endif


	// all the solution is disabled
	// random select one
	if(!Recorded)
	{
		if(rand() % 2)
		{
			recordSwapList(_machineNo, l, r, STATE);
		}
	}
	return;
}

/**
 * calculate the swap score from start
 * @param _machineNo machine number
 * @param l          start index
 * @param r          end index
 * @param STATE      backward/forward
 */
void getLengthFromStart(int _machineNo, int l, int r, int STATE)
{
	MachineSequence *p = &machineSequence[_machineNo];
	int i;
	//put l
	if(STATE == BACKWARD_SHIFT)
	{
		i = l + 1;
		/**
		 * l is the first work of the machine
		 */
		if(l == 1)
		{
			/**
			 * no[i] is the first work of the job
			 * thus after shift, i(l + 1) must start from time 0;
			 */
			if(p->no[i] == headVertex[jobVertex[p->no[i]].belong_job])
			{
				swapScore[p->no[i]].fromStart = 0;
			}
			else
			{
				swapScore[p->no[i]].fromStart =
					topoVertex[p->no[i] - 1].earliestTime + jobEdge[p->no[i] - 1].duration;
			}
		}
		else//l is not the first work of the machine
		{
			if(p->no[i] == headVertex[jobVertex[p->no[i]].belong_job])
			{
				swapScore[p->no[i]].fromStart =
					topoVertex[p->no[l - 1]].earliestTime + jobEdge[p->no[l - 1]].duration;
			}
			else
			{

				swapScore[p->no[i]].fromStart =
				max(topoVertex[p->no[i] - 1].earliestTime + jobEdge[p->no[i] - 1].duration,
					topoVertex[p->no[l - 1]].earliestTime + jobEdge[p->no[l - 1]].duration);
			}
		}

		for(++i; i <= r; ++i)
		{
			if(p->no[i] == headVertex[jobVertex[p->no[i]].belong_job])
			{
				swapScore[p->no[i]].fromStart =
					swapScore[p->no[i - 1]].fromStart + jobEdge[p->no[i - 1]].duration;
			}
			else
			{
				swapScore[p->no[i]].fromStart =
					max(topoVertex[p->no[i] - 1].earliestTime + jobEdge[p->no[i] - 1].duration,
						swapScore[p->no[i - 1]].fromStart + jobEdge[p->no[i - 1]].duration);
			}
		}

		if(p->no[l] == headVertex[jobVertex[p->no[l]].belong_job])
		{
			swapScore[p->no[l]].fromStart =
				swapScore[p->no[r]].fromStart + jobEdge[p->no[r]].duration;
		}
		else
		{
			swapScore[p->no[l]].fromStart =
				max(topoVertex[p->no[l] - 1].earliestTime + jobEdge[p->no[l] - 1].duration,
					swapScore[p->no[r]].fromStart + jobEdge[p->no[r]].duration);
		}
	}
	else//STATE == FORWARD_SHIFT
	{
		i = l + 1;
		if(l == 1)
		{
			if(p->no[r] == headVertex[jobVertex[p->no[r]].belong_job])
			{
				swapScore[p->no[r]].fromStart = 0;
			}
			else
			{
				swapScore[p->no[r]].fromStart =
					topoVertex[p->no[r] - 1].earliestTime
					+ jobEdge[p->no[r] - 1].duration;
			}
		}
		else
		{
			if(p->no[r] == headVertex[jobVertex[p->no[r]].belong_job])
			{
				swapScore[p->no[r]].fromStart =
					topoVertex[p->no[l - 1]].earliestTime
					+ jobEdge[p->no[l - 1]].duration;
			}
			else
			{
				swapScore[p->no[r]].fromStart =
					max(topoVertex[p->no[r] - 1].earliestTime
						+ jobEdge[p->no[r] - 1].duration,
						topoVertex[p->no[l - 1]].earliestTime
						+ jobEdge[p->no[l - 1]].duration);
			}
		}

		if(p->no[l] == headVertex[jobVertex[p->no[l]].belong_job])
		{
			swapScore[p->no[l]].fromStart =
				swapScore[p->no[r]].fromStart
				+ jobEdge[p->no[r]].duration;
		}
		else
		{
			swapScore[p->no[l]].fromStart =
				max(topoVertex[p->no[l] - 1].earliestTime
					+ jobEdge[p->no[l] - 1].duration,
					swapScore[p->no[r]].fromStart
					+ jobEdge[p->no[r]].duration);
		}

		for(; i <= r - 1; ++i)
		{
			if(p->no[i] == headVertex[jobVertex[p->no[i]].belong_job])
			{
				swapScore[p->no[i]].fromStart =
					swapScore[p->no[i - 1]].fromStart
					+ jobEdge[p->no[i - 1]].duration;
			}
			else
			{
				swapScore[p->no[i]].fromStart =
					max(topoVertex[p->no[i] - 1].earliestTime
						+ jobEdge[p->no[i] - 1].duration,
						swapScore[p->no[i - 1]].fromStart
						+ jobEdge[p->no[i - 1]].duration);
			}
		}
	}
}

/**
 * calculate the swap score to end
 * @param _machineNo machine number
 * @param l          start index
 * @param r          end index
 * @param STATE      backward/forward
 */
void getLengthToEnd(int _machineNo, int l, int r, int STATE)
{
	MachineSequence *p = &machineSequence[_machineNo];
	int i;
	if(STATE == BACKWARD_SHIFT)
	{
		i = r - 1;
		if(r == p->length)
		{
			if(p->no[l] == tailVertex[jobVertex[p->no[l]].belong_job])
			{
				swapScore[p->no[l]].toEnd =
					jobEdge[p->no[l]].duration;
			}
			else
			{
				swapScore[p->no[l]].toEnd =
					longestTime
					- topoVertex[p->no[l] + 1].latestTime
					+ jobEdge[p->no[l]].duration;
			}
		}
		else
		{
			if(p->no[l] == tailVertex[jobVertex[p->no[l]].belong_job])
			{
				swapScore[p->no[l]].toEnd =
					jobEdge[p->no[l]].duration
					+ longestTime
					- topoVertex[p->no[r + 1]].latestTime;
			}
			else
			{
				swapScore[p->no[l]].toEnd =
					jobEdge[p->no[l]].duration +
					max(longestTime
						- topoVertex[p->no[l] + 1].latestTime,
						longestTime
						- topoVertex[p->no[r + 1]].latestTime);
			}
		}

		if(p->no[r] == tailVertex[jobVertex[p->no[r]].belong_job])
		{
			swapScore[p->no[r]].toEnd =
				jobEdge[p->no[r]].duration
				+ swapScore[p->no[l]].toEnd;
		}
		else
		{
			swapScore[p->no[r]].toEnd =
				jobEdge[p->no[r]].duration +
				max(longestTime
					- topoVertex[p->no[r] + 1].latestTime,
					swapScore[p->no[l]].toEnd);
		}

		for(; i >= l + 1; --i)
		{
			if(p->no[i] == tailVertex[jobVertex[p->no[i]].belong_job])
			{
				swapScore[i].toEnd =
					jobEdge[p->no[i]].duration
					+ swapScore[p->no[i + 1]].toEnd;
			}
			else
			{
				swapScore[i].toEnd =
					jobEdge[p->no[i]].duration
					+ max(longestTime
						- topoVertex[p->no[i] + 1].latestTime,
						swapScore[p->no[i + 1]].toEnd);
			}
		}
	}
	else//STATE == FORWARD_SHIFT
	{
		i = r - 1;
		if(r == p->length)
		{
			if(p->no[i] == tailVertex[jobVertex[p->no[i]].belong_job])
			{
				swapScore[p->no[i]].toEnd = jobEdge[p->no[i]].duration;
			}
			else
			{
				swapScore[p->no[i]].toEnd =
					longestTime
					- topoVertex[p->no[i] + 1].latestTime
					+ jobEdge[p->no[i]].duration;
			}
		}
		else//r != p->length
		{
			if(p->no[i] == tailVertex[jobVertex[p->no[i]].belong_job])
			{
				swapScore[p->no[i]].toEnd =
					longestTime
					- topoVertex[p->no[r + 1]].latestTime
					+ jobEdge[p->no[i]].duration;
			}
			else
			{
				swapScore[p->no[i]].toEnd =
					jobEdge[p->no[i]].duration +
					max(longestTime
						- topoVertex[p->no[i] + 1].latestTime,
						longestTime
						- topoVertex[p->no[r + 1]].latestTime);
			}
		}

		for(--i; i >= l; --i)
		{
			if(p->no[i] == tailVertex[jobVertex[p->no[i]].belong_job])
			{
				swapScore[p->no[i]].toEnd =
					swapScore[p->no[i + 1]].toEnd
					+ jobEdge[p->no[i]].duration;
			}
			else
			{
				swapScore[p->no[i]].toEnd =
					jobEdge[p->no[i]].duration +
					max(longestTime
						- topoVertex[p->no[i] + 1].latestTime,
						swapScore[p->no[i + 1]].toEnd);
			}
		}

		if(p->no[r] == tailVertex[jobVertex[p->no[r]].belong_job])
		{
			swapScore[p->no[r]].toEnd =
				jobEdge[p->no[r]].duration
				+ swapScore[p->no[l]].toEnd;
		}
		else
		{
			swapScore[p->no[r]].toEnd =
				jobEdge[p->no[r]].duration +
				max(longestTime
					- topoVertex[p->no[r] + 1].latestTime,
					swapScore[p->no[l]].toEnd);
		}
	}
}

// no need to do so, because originalScore == longestTime
// for every vertex in the critical path, score == longestTime
/*
void getOriginalScore(int _machineNo, int l, int r)
{
	MachineSequence *p = &machineSequence[_machineNo];
	originalScore = 0x80000000;
	for(int i = l; i <= r; ++i)
	{
		originalScore =
			max(originalScore,
				topoVertex[p->no[l]].earliestTime
				+ longestTime
				- topoVertex[p->no[l]].latestTime);
	}
	// printf("ori %d\n", originalScore);
}
*/


/**
 * if the swap is good enough (than ever), record it
 * @param _machineNo machine number
 * @param l          start index
 * @param r          end index
 * @param STATE      backward/forward
 */
void recordSwapList(int _machineNo, int l, int r, int STATE)
{
	MachineSequence *p = &machineSequence[_machineNo];
	if(swapList.no != NULL) free(swapList.no);
	int size = r - l + 1;
	swapList.no = (int *)malloc(size * sizeof(int));
	swapList.machineNo = _machineNo;
	swapList.startIndex = l;
	swapList.endIndex = r;
	swapList.STATE = STATE;
	for(int i = 0; i < size; ++i)
	{
		swapList.no[i] = p->no[l + i];
	}
}

/**
 * copy the current best solution into bestSolution
 * or copy the overall best solution into machineSequence
 * @param STATE BEST_TO_MACHINE/MACHINE_TO_BEST
 */
void copySolution(int STATE)
{
	if(STATE == BEST_TO_MACHINE)
	{
		for(int i = 0; i < machineNumber; ++i)
		{
			machineSequence[i].length = bestSolution[i].length;
			for(int j = 1; j <= bestSolution[i].length; ++j)
			{
				machineSequence[i].no[j] = bestSolution[i].no[j];
			}
		}
	}
	else
	{
		for(int i = 0; i < machineNumber; ++i)
		{
			bestSolution[i].length = machineSequence[i].length;
			for(int j = 1; j <= bestSolution[i].length; ++j)
			{
				bestSolution[i].no[j] = machineSequence[i].no[j];
			}
		}
	}
}

/**
 * copy the best solution
 * reset the jobVertex
 */
void finalSolution()
{
	copySolution(BEST_TO_MACHINE);
	getEarliest();
	currentT = smallestTime;
	for(int i = 1; i <= jobVertexCount; ++i)
	{
		jobVertex[i].current_time = topoVertex[i].earliestTime;
	}
	clearTabuList();
}

/**
 * for test
 * check whether some move is illegal
 */
void existConflict()
{
	for(int i = 0; i < machineNumber; ++i)
	{
		MachineSequence *p = &machineSequence[i];
		for(int j = 1; j <= p->length; ++j)
		{
			printf("%d ", p->no[j]);
			for(int t = j - 1; t >= 1; --t)
			{
				if(jobVertex[p->no[t]].belong_job == jobVertex[p->no[j]].belong_job)
				{
					printf("comparing");
					if(p->no[t] > p->no[j])
						perror("conflict\n");
				}
			}
		}
		putchar('\n');
	}
}


#endif
