#ifndef output_c
#define output_c

#include "output.h"
#include "datastructure.c"
#include <stdio.h>

extern int machineNumber;
extern int currentT;

/**
 * standard(required) output
 */
void stdOutput()
{
	for(int i = 0; i < machineNumber; ++i)
	{
		printf("M%d ", i);
		MachineSequence *p = &machineSequence[i];
		for(int j = 1; j <= p->length; ++j)
		{
			printf("(%d,%d-%d,%d) ",
					jobVertex[p->no[j]].current_time,
					jobVertex[p->no[j]].belong_job - 1,
					p->no[j] - headVertex[jobVertex[p->no[j]].belong_job],
					jobVertex[p->no[j]].current_time + jobEdge[p->no[j]].duration);
		}
		putchar('\n');
	}
	printf("END %d\n", currentT);
}

#endif
