#include <stdio.h>
#include "itertools.h"

int fun(const void* a, const void* b)
{
	int i1 = *(int*) a;
	int i2 = *(int*) b;
	return i1 < i2? i1: i2;
}

int main()
{
	int a[] = {20, 1, 4, 6, 9, 60};
	iter* ai = newArrayIter(a, 6, sizeof(*a));
	iter* i = accumulate(ai, fun, 1000);
	if (!i)
	{
		printf("Invalid input\n");
		return 1;
	}
	printf("Min : ");
	// for (int i = 0; i < n; i++)
	int min;
	while (has_next(i))
	{
		min = *(int*) next(i);
		//printf("%d ", min);
	}
	printf("%d\n", min);
	free_iter(i);
	free_iter(ai);
}