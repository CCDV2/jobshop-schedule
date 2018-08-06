// The input of the program
// to store the data into the data structure

#ifndef input_c
#define input_c


#include "datastructure.h"
#include "input.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define RELEASE_INPUT

extern int jobNumber, machineNumber;

/**
 * main function in input
 */
void inputData()
{
	#ifdef NEED_MACHINE_EDGE
	initTemp(&temp);
	#endif

	// printf("input the number of the job and the machine\n");
	scanf("%d%d", &jobNumber, &machineNumber);
	getchar();
	for(int i = 1; i <= jobNumber; ++i)
	{
		// puts("input the number of the process");
		int k = 0;
		int _machine, _duration;

		// the test input which is easy to input
		#ifdef TEST_INPUT
		scanf("%d", &k);

		for(int j = 1; j <= k; ++j)
		{
			// puts("machine, duration");
			scanf("%d%d", &_machine, &_duration);
			buildJobVertex(_machine, i);

			#ifdef NEED_MACHINE_EDGE
			insertTemp(_machine, _duration, jobVertexCount);
			#endif

			if(j == 1)
			{
				headVertex[i] = jobVertexCount;
			}
			if(j == k)
			{
				tailVertex[i] = jobVertexCount;
			}
			buildJobEdge(i, _duration);
		}
		#endif

		// the release(required) input which is hard and confucious
		// and some info useless
		#ifdef RELEASE_INPUT

		char inputString[500];
		gets(inputString);
		int l = strlen(inputString);
		int t = 0;
		// while(isdigit(inputString[t])) ++t;
		for(; t < l; ++t)
		{
			if(isdigit(inputString[t]))
			{
				++k;
				_machine = 0;
				_duration = 0;
				while(isdigit(inputString[t]))
				{
					_machine *= 10;
					_machine += inputString[t] - '0';
					++t;
				}
				while(!isdigit(inputString[t])) ++t;
				while(isdigit(inputString[t]))
				{
					_duration *= 10;
					_duration += inputString[t] - '0';
					++t;
				}
				buildJobVertex(_machine, i);
				if(k == 1)
				{
					headVertex[i] = jobVertexCount;
				}
				buildJobEdge(i, _duration);
			}
		}
		tailVertex[i] = jobVertexCount;

		#endif
	}
	/*
	#ifdef RELEASE_INPUT
	int useless;
	scanf("%d", &useless);
	#endif
	*/
	// buildHeadJobEdge();
	// buildHeadJobVertex();
	buildTailJobEdge();

	#ifdef NEED_MACHINE_EDGE
	// is it necessary to build machineEdge?
	viewTemp();
	free(temp);
	temp = NULL;
	printGraph();
	#endif
}

/**
 * connect the super source vertex with each headVertex
 * actually this function is not used
 */
void buildHeadJobEdge()
{
	srcVex = (int *)malloc(sizeof(int) * MAXM);
	for(int i = 1; i <= jobNumber; ++i)
	{
		int no = ++jobEdgeCount;
		jobEdge[no].belong_job = 0;
		jobEdge[no].next_work = headVertex[i];
		jobEdge[no].duration = 0;
		srcVex[i] = no;
	}
}

/**
 * connect the super sink vertex with each tailVertex
 * acctually this function is not used, too
 */
void buildTailJobEdge()
{
	for(int i = 1; i <= jobNumber; ++i)
	{
		int no = tailVertex[i];
		jobEdge[no].next_work = jobVertexCount + 1;
	}
}

/**
 * connect the current vertex with the next vertex (same job)
 * @param _jobnum   the number of the belonged job
 * @param _duration the time of this work
 */
void buildJobEdge(int _jobnum, int _duration)
{
	++jobEdgeCount;
	int no = jobEdgeCount;
	//printf("jobEdge:%d duration:%d\n", no, _duration);

	jobEdge[no].belong_job = _jobnum;
	jobEdge[no].next_work = jobVertexCount + 1;
	jobEdge[no].duration = _duration;
}

/**
 * record the current vertex's info
 * @param _machineNum the number of the belonged machine
 * @param _jobnum     the number of the belonged job
 */
void buildJobVertex(int _machineNum ,int _jobnum)
{
	int no = ++jobVertexCount;
	//printf("vertex: %d\n", no);
	jobVertex[no].belong_machine = _machineNum;
	jobVertex[no].belong_job = _jobnum;
	jobVertex[no].current_time = 0;
}

void buildHeadJobVertex()
{
	//no need to do so
}

// not used
#ifdef NEED_MACHINE_EDGE

void initTemp(struct TempMachineGroup **p)
{
	*p = (struct TempMachineGroup *)malloc(sizeof(struct TempMachineGroup) * MAXM);
	if(p == NULL)
	{
		perror("malloc failed\n");
		exit(-1);
	}
	return;
}

void insertTemp(int _machine, int _duration, int _workNo)
{
	int i = ++temp[_machine].machineTotalWork;
	temp[_machine].work[i].workNo = _workNo;
	temp[_machine].work[i].duration = _duration;
}

void viewTemp()
{
	for(int i = 0; i < machineNumber; ++i)
	{
		int size = temp[i].machineTotalWork;
		//printf("size %d: %d\n", i, size);
		for(int j = 1; j <= size; ++j)
		{
			for(int k = j + 1; k <= size; ++k)
			{
				int workNo1 = temp[i].work[j].workNo;
				int workNo2 = temp[i].work[k].workNo;
				if(jobEdge[workNo1].belong_job == jobEdge[workNo2].belong_job)
				{
					continue;
				}
				//printf("connecting %d %d\n", workNo1, workNo2);
				buildMachineEdge(workNo1, workNo2, temp[i].work[j].duration, i);
				buildMachineEdge(workNo2, workNo1, temp[i].work[k].duration, i);
			}
		}
	}
}


void buildMachineEdge(int _from, int _to, int _duration, int _belong_machine)
{
	//printf("from %d, to %d, duratoin %d, belong_machine %d\n", _from, _to, _duration, _belong_machine);
	int no = ++machineEdgeCount;
	machineEdge[no].belong_machine = _belong_machine;
	machineEdge[no].to = _to;
	machineEdge[no].duration = _duration;
	machineEdge[no].next_edge = headEdge[_from];
	headEdge[_from] = no;
}

/**
 * to test if the input is correct
 */
void printGraph()
{
	puts("Printing the A");
	for(int i = 1; i <= jobNumber; ++i)
	{
		printf("V %d--", headVertex[i]);
		for(int j = headVertex[i]; j != jobVertexCount + 1;)
		{
			int no = jobVertex[j].next_edge;
			printf("E %d->", no);
			j = jobEdge[no].next_work;
			printf(" V %d--", j);
		}
		putchar('\n');
	}
	system("pause");

	puts("Printing the E");
	for(int i = 1; i <= jobNumber; ++i)
	{
		for(int j = srcVex[i]; jobEdge[j].next_work != jobVertexCount + 1; j = jobEdge[j].next_work)
		{
			int no = jobEdge[j].next_work;
			printf("the no.%d vertex's edges:\n", no);
			for(int t = headEdge[no]; t; t = machineEdge[t].next_edge)
			{
				MachineEdge *p = &machineEdge[t];
				printf("machine %d\t to:%d, time %d, next_edge %d\n", p->belong_machine, p->to, p->duration, p->next_edge);
			}
			putchar('\n');
		}
	}
}

#endif

#endif
