// This is the main part of the program.
// To connect all the parts.

// #define ANOTHER_SEARCH_METHOD
#define RELEASE_INPUT

#include "input.c"
#include "giffler.c"
#include "tabusearch.c"
#include "output.c"
#include <stdlib.h>
#include <time.h>

const int multiple = 1;
int jobNumber, machineNumber;

int main()
{
	srand((unsigned)time(NULL));
	long _start = clock();
	// freopen("test.txt", "w", stdout);
	// freopen("tempSave.txt", "r", stdin);
	inputData();
	// fclose(stdin);
	// freopen("CON","r",stdin);

	findOneSoulution();
	// printSolution();
	solve();

	// printf("%d\n", currentT);
	// printSolution();
	stdOutput();

	long _end = clock();
	printf("used time: %ld.%lds\n", (_end - _start) / 1000, (_end - _start) % 1000);
	// system("pause");
	// puts("fgffgfkf");
	return 0;
}
