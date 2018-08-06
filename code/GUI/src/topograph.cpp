// get all the critical path in the graph
// part of the tabu search
// authored by 杨子阳

#ifndef topograph_c
#define topograph_c

#include <stdio.h>
#include <stdbool.h>
#include<string.h>
#include<ctype.h>
#include "src/topograph.h"

extern int jobNumber;
extern bool existUndisabledSolution;

// due to some unknown bugs, some swap operations may
// lead to an infeasible solution
// Thus, use this variable to ensure all the vertex have been calculated
int calculatedVertexCount;

int min(int a, int b)
{
	return a > b ? b : a;
}

/**
 * reset all the topological vertex's critical condtion into false
 */
void initTopoVertex()
{
	for(int i = 1; i <= jobVertexCount; ++i)
	{
		topoVertex[i].critical = false;
	}
}

/**
 * connect to current vertex's jp, js, mp, ms
 * @param i current vertex
 */
void connectTopoVertex(int i)
{
	if(i == headVertex[jobVertex[i].belong_job])
	{
		topoVertex[i].jp = 0;
	}
	else
	{
		topoVertex[i].jp = i - 1;
	}
	if(i == tailVertex[jobVertex[i].belong_job])
	{
		topoVertex[i].js = jobVertexCount + 1;
	}
	else
	{
		topoVertex[i].js = i + 1;
	}
	MachineSequence *p = &machineSequence[jobVertex[i].belong_machine];
	int index = findSequenceIndex(jobVertex[i].belong_machine, i);
	if(i == p->no[1])
	{
		topoVertex[i].mp = 0;
	}
	else
	{
		topoVertex[i].mp = p->no[index - 1];
	}
	if(i == p->no[p->length])
	{
		topoVertex[i].ms = jobVertexCount + 1;
	}
	else
	{
		topoVertex[i].ms = p->no[index + 1];
	}
}

/**
 * get the earliest time of each vertex in topological sort
 */
void getEarliest()
{
	calculatedVertexCount = 0;
	for(int i = 1; i <= jobVertexCount; ++i)
	{
		connectTopoVertex(i);
		topoVertex[i].earliestTime = 0;
		topoVertex[i].visited = false;
	}
	topoVertex[jobVertexCount + 1].earliestTime = 0;

	Queue q;
	init(&q);
	for(int i = 1; i <= jobNumber; ++i)
	{
		if(topoVertex[headVertex[i]].mp == 0)
		{
			//printf("head push %d\n", headVertex[i]);
			topoVertex[headVertex[i]].visited = true;
			push(&q, headVertex[i]);
			++calculatedVertexCount;
		}
	}
	while(!empty(&q))
	{
		int t = front(&q);
		pop(&q);
		if(t == jobVertexCount + 1) continue;
		// printf("%d\n", t);

		int et = topoVertex[t].earliestTime;
		int js = topoVertex[t].js;
		int ms = topoVertex[t].ms;

		topoVertex[js].earliestTime =
			max(topoVertex[js].earliestTime,
				et + jobEdge[t].duration);
		if(js != jobVertexCount + 1)
		{
			topoVertex[js].jp = 0;
		}

		topoVertex[ms].earliestTime =
			max(topoVertex[ms].earliestTime,
				et + jobEdge[t].duration);
		if(ms != jobVertexCount + 1)
		{
			topoVertex[ms].mp = 0;
		}

		if(!topoVertex[js].visited &&
			topoVertex[js].jp == 0 && topoVertex[js].mp == 0)
		{
			topoVertex[js].visited = true;
			//printf("pushing %d\n", js);
			push(&q, js);
			++calculatedVertexCount;
		}
		if(!topoVertex[ms].visited &&
			topoVertex[ms].jp == 0 && topoVertex[ms].mp == 0)
		{
			topoVertex[ms].visited = true;
			//printf("pushing %d\n", ms);
			push(&q, ms);
			++calculatedVertexCount;
		}
	}
}

/**
 * get the latest time of each vertex in reverse topological sort
 */
void getLatest()
{
	int end = jobVertexCount + 1;
	for(int i = 1; i <= jobVertexCount; ++i)
	{
		connectTopoVertex(i);
		topoVertex[i].latestTime = 0x3f3f3f3f;
		topoVertex[i].visited = false;
	}
	topoVertex[jobVertexCount + 1].latestTime = topoVertex[jobVertexCount + 1].earliestTime;
	Queue q;
	init(&q);
	int T = topoVertex[end].latestTime;
	for(int i = 1; i <= jobNumber; ++i)
	{
		if(topoVertex[tailVertex[i]].ms == end)
		{
			topoVertex[tailVertex[i]].latestTime = T - jobEdge[tailVertex[i]].duration;
			topoVertex[tailVertex[i]].visited = true;
			push(&q, tailVertex[i]);
			//printf("push tail %d\n", tailVertex[i]);
		}
	}
	while(!empty(&q))
	{
		int t = front(&q);
		pop(&q);
		if(t == 0) continue;

		int lt = topoVertex[t].latestTime;
		int jp = topoVertex[t].jp;
		int mp = topoVertex[t].mp;
		topoVertex[jp].latestTime =
			min(topoVertex[jp].latestTime,
				lt - jobEdge[jp].duration);
		if(jp != 0)
		{
			topoVertex[jp].js = end;
		}

		topoVertex[mp].latestTime =
			min(topoVertex[mp].latestTime,
				lt - jobEdge[mp].duration);
		if(mp != 0)
		{
			topoVertex[mp].ms = end;
		}

		if(!topoVertex[jp].visited && topoVertex[jp].js == end && topoVertex[jp].ms == end)
		{
			topoVertex[jp].visited = true;
			//printf("pushing %d\n", jp);
			push(&q, jp);
		}
		if(!topoVertex[mp].visited && topoVertex[mp].js == end && topoVertex[mp].ms == end)
		{
			topoVertex[mp].visited = true;
			//printf("pusing %d\n", mp);
			push(&q, mp);
		}
	}
}

/**
 * scan through the topological graph.
 * get the critical vertex
 */
void findCriticalVertex()
{
	for(int i = 1; i <= jobVertexCount; ++i)
	{
		connectTopoVertex(i);
	}

	for(int i = 1; i <= jobVertexCount; ++i)
	{
		//printf("ve %d\tvl %d\n", topoVertex[i].earliestTime, topoVertex[i].latestTime);
		if(topoVertex[i].earliestTime == topoVertex[i].latestTime)
		{
			//printf("%d is critical vertex\n", i);
			topoVertex[i].critical = true;
		}
	}
}

/**
 * find all the critical path by dfs
 * @param t      current work number
 * @param _index current depth/index in criticalPath
 */
void findCriticalPath(int t, int _index)
{
	if(_index == 0)
	{
		for(int i = 1; i <= jobNumber; ++i)
		{
			if(topoVertex[headVertex[i]].critical
				 && topoVertex[headVertex[i]].mp == 0)
			{
				// printf("depth %d, no %d\n", _index + 1, headVertex[i]);
				criticalPath[_index + 1] = headVertex[i];
				findCriticalPath(headVertex[i], _index + 1);
			}
		}
		return;
	}

	int js = topoVertex[t].js, ms = topoVertex[t].ms;
	if(js == jobVertexCount + 1 && ms == jobVertexCount + 1)
	{
		/*
		printf("find a critical path\n");
		for(int i = 1; i <= _index; ++i)
		{
			printf("%-3d", criticalPath[i]);
		}
		putchar('\n');
		// */
		findBlock(_index); // move into tabu search
		return;
	}

	if(topoVertex[js].critical &&
		topoVertex[js].earliestTime ==
		topoVertex[t].earliestTime + jobEdge[t].duration)
	{
		// printf("depth %d, no %d\n", _index + 1, js);
		criticalPath[_index + 1] = js;
		findCriticalPath(js, _index + 1);

		#ifdef ANOTHER_SEARCH_METHOD
		if(existUndisabledSolution) return;
		#endif
	}
	if(topoVertex[ms].critical &&
		topoVertex[ms].earliestTime ==
		topoVertex[t].earliestTime + jobEdge[t].duration)
	{
		// printf("depth %d, no %d\n", _index + 1, js);
		criticalPath[_index + 1] = ms;
		findCriticalPath(ms, _index + 1);

		#ifdef ANOTHER_SEARCH_METHOD
		if(existUndisabledSolution) return;
		#endif
	}
}

/**
 * print the topograph
 * test whether all the calculation above is correct
 */
void printTopoGraph()
{
	for(int i = 1; i <= jobVertexCount; ++i)
	{
		printf("%-4d", topoVertex[i].earliestTime);
	}
	putchar('\n');
	printf("%d\n", topoVertex[jobVertexCount + 1].earliestTime);
	for(int i = 1; i <= jobVertexCount; ++i)
	{
		printf("%d ", topoVertex[i].latestTime);
	}
	putchar('\n');
}


/**
 * print the connection of each vertex
 * test whether all the calculatoin above is correct
 */
void printConnection()
{
	for(int i = 1; i <= jobVertexCount; ++i)
	{
		printf("vertex %d:\n", i);
		printf("jp %d, js %d, mp %d, ms %d\n\n",
				topoVertex[i].jp,
				topoVertex[i].js,
				topoVertex[i].mp,
				topoVertex[i].ms);
	}
}

/**
 * @brief get the overlapping cases between job and repair
 * @param checkNo the number of the work
 * @return
 */
int getCase(int checkNo)
{
	int op = topoVertex[checkNo].earliestTime, ed = op + jobEdge[checkNo].duration;
	int m = jobVertex[checkNo].belong_machine;
	if(repairList[m].startTime == 0) return 0;

	if(op < repairList[m].startTime && ed >= repairList[m].startTime)
	{
		return 1;
	}
	if(op < repairList[m].endTime && op >= repairList[m].startTime)
	{
		return 2;
	}
	return 0;
}


/**
 * @brief readjust time with repair condtion
 * @param start_time repair starts time
 */
void readjustEarliestTime(int start_time)
{
	int a, b;
	memset(repairList, 0, sizeof(repairList));

	char input[500];
	while(gets(input))
	{
		a = b = 0;
		int pos = 0;
		while(!isdigit(input[pos])) ++pos;
		while(isdigit(input[pos]))
		{
			a *= 10;
			a += input[pos] - '0';
			++pos;
		}
		while(!isdigit(input[pos])) ++pos;
		while(isdigit(input[pos]))
		{
			b *= 10;
			b += input[pos] - '0';
			++pos;
		}

		repairList[a].startTime = start_time;
		repairList[a].endTime = repairList[a].startTime + b;
	}


	for(int i = 1; i <= jobVertexCount; ++i)
	{
		connectTopoVertex(i);
//		topoVertex[i].earliestTime = 0;
		topoVertex[i].visited = false;
	}
	topoVertex[jobVertexCount + 1].earliestTime = 0;

	Queue q;
	init(&q);
	for(int i = 1; i <= jobNumber; ++i)
	{
		if(topoVertex[headVertex[i]].mp == 0)
		{
			//printf("head push %d\n", headVertex[i]);
			topoVertex[headVertex[i]].visited = true;
			push(&q, headVertex[i]);
		}
	}
	while(!empty(&q))
	{
		int t = front(&q);
//		qDebug() << t;
		pop(&q);
		if(t == jobVertexCount + 1) continue;


		int et = topoVertex[t].earliestTime;
		int js = topoVertex[t].js;
		int ms = topoVertex[t].ms;

		topoVertex[js].earliestTime =
			max(topoVertex[js].earliestTime,
				et + jobEdge[t].duration);
		if(js != jobVertexCount + 1)
		{
			topoVertex[js].jp = 0;
		}

		int m, delay;
		int _case = getCase(t);
//		qDebug() << "case: " << _case;
		switch(_case)
		{
		case 2://the job should be done later
			topoVertex[t].earliestTime = repairList[jobVertex[t].belong_machine].endTime;
			et = topoVertex[t].earliestTime;
			topoVertex[ms].earliestTime =
				max(topoVertex[ms].earliestTime,
					et + jobEdge[t].duration);
			break;
		case 1://the repair should be done later
			m = jobVertex[t].belong_machine;
			delay = et + jobEdge[t].duration - repairList[m].startTime;
			repairList[m].startTime += delay;
			repairList[m].endTime += delay;
			topoVertex[ms].earliestTime =
					max(topoVertex[ms].earliestTime,
						repairList[m].endTime);
			break;
		case 0://nothing conflict
			topoVertex[ms].earliestTime =
				max(topoVertex[ms].earliestTime,
					et + jobEdge[t].duration);
			break;
		}
		if(ms != jobVertexCount + 1)
		{
			topoVertex[ms].mp = 0;
		}

		if(!topoVertex[js].visited &&
			topoVertex[js].jp == 0 && topoVertex[js].mp == 0)
		{
			topoVertex[js].visited = true;
			//printf("pushing %d\n", js);
			push(&q, js);
		}
		if(!topoVertex[ms].visited &&
			topoVertex[ms].jp == 0 && topoVertex[ms].mp == 0)
		{
			topoVertex[ms].visited = true;
			//printf("pushing %d\n", ms);
			push(&q, ms);
		}
	}

	for(int i = 1; i <= jobVertexCount; ++i)
	{
		jobVertex[i].current_time = topoVertex[i].earliestTime;
	}
	currentT = topoVertex[jobVertexCount + 1].earliestTime;
}

#endif
