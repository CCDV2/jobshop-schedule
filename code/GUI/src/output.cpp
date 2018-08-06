// authored by 杨子阳

#ifndef output_c
#define output_c

#include "src/output.h"
#include "src/datastructure.h"
#include <stdio.h>
#include<QDebug>

extern int machineNumber;
extern int currentT;

/**
 * standard(required) output
 */
void stdOutput(int elapsedTime)
{
	for(int i = 0; i < machineNumber; ++i)
	{
		printf("M%d", i);
		MachineSequence *p = &machineSequence[i];
		for(int j = 1; j <= p->length; ++j)
		{
			printf(" (%d,%d-%d,%d)",
					jobVertex[p->no[j]].current_time,
					jobVertex[p->no[j]].belong_job - 1,
					p->no[j] - headVertex[jobVertex[p->no[j]].belong_job],
					jobVertex[p->no[j]].current_time + jobEdge[p->no[j]].duration);
		}
		putchar('\n');
	}
	printf("Time Used: %d.%ds\n", elapsedTime / 1000, elapsedTime % 1000);
	printf("End Time: %d\n", currentT);
}

void stdOutput2()
{
    for(int i = 0; i < machineNumber; ++i)
    {
		printf("M%d", i);
        MachineSequence *p = &machineSequence[i];
		int j, k, count = 0;
		for(j = 0; j <= currentT; j = k + 1)
        {
            for(k = j; k < currentT && paintNumber[i][k] == paintNumber[i][k+1]; ++k);

			if(paintNumber[i][k] == -1) continue;

            if(paintNumber[i][k] != 0)
            {
                count++;
				if(count > p->length) break;

				printf(" (%d,%d-%d,%d)",
                        jobVertex[p->no[count]].current_time,
                        jobVertex[p->no[count]].belong_job - 1,
                        p->no[count] - headVertex[jobVertex[p->no[count]].belong_job],
                        jobVertex[p->no[count]].current_time + jobEdge[p->no[count]].duration);
            }
            else
            {
				printf(" (%d,检修,%d)", j, k + 1);
            }
        }
        putchar('\n');
    }
    printf("END %d\n", currentT);
}

#endif
