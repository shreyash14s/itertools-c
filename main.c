#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "itertools.h"

int fun(const void* a, const void* b)
{
	return *(int*) a + *(int*) b;
}

int main()
{
	int n;
	printf("Enter the size of array: ");
	scanf("%d", &n);
	printf("Enter the array elements: ");
	int* b = (int*) malloc(n * sizeof(int));
	for (int i = 0; i < n; i++)
	{
		scanf("%d", &b[i]);
	}
	printf("Array : ");
	iter* i = newArrayIter(b, n, sizeof(*b));
	if (!i)
	{
		printf("Invalid input\n");
		return 1;
	}
	while(has_next(i))
	{
		printf("%d ", *(int*) next(i));
	}
	printf("\n");
	free_iter(i);

	iter* i3 = count(0, 2);
	if (!i3)
	{
		printf("Invalid input\n");
		return 1;
	}
	printf("Count : ");
	for (int i = 0; i < n; i++)
	{
		printf("%d ", *(int*) next(i3));
	}
	printf("\n");

	iter* i4 = cycle(b, n, sizeof(*b));
	if (!i4)
	{
		printf("Invalid input\n");
		return 1;
	}
	printf("Cycle : ");
	for (int i = 0; i < 2*n+1; i++)
	{
		printf("%d ", *(int*) next(i4));
	}
	printf("\n");
	free_iter(i4);

	iter* i5 = repeat(10);
	if (!i5)
	{
		printf("Invalid input\n");
		return 1;
	}
	printf("Repeat : ");
	for (int i = 0; i < n; i++)
	{
		printf("%d ", *(int*) next(i5));
	}
	printf("\n");
	free_iter(i5);

	iter* ai = newArrayIter(b, n, sizeof(*b));
	iter* i6 = accumulate(ai, fun, 0);
	if (!i6)
	{
		printf("Invalid input\n");
		return 1;
	}
	printf("Accumulate : ");
	for (int i = 0; i < n; i++)
	{
		printf("%d ", *(int*) next(i6));
	}
	printf("\n");
	free_iter(i6);
	free_iter(ai);

	int a[] = {1, 0, 1};
	ai = newArrayIter(b, n, sizeof(*b));
	iter* ai2 = cycle(a, 3, sizeof(*a));
	iter* i7 = compress(ai, ai2);
	if (!i7)
	{
		printf("Invalid input\n");
		return 1;
	}
	printf("Compress : ");
	while (has_next(i7))
	{
		printf("%d ", *(int*) next(i7));
	}
	printf("\n");
	free_iter(i7);
	free_iter(ai);
	free_iter(ai2);

	ai = newArrayIter(b, n, sizeof(*b));
	iter* i8 = islice(ai, 0, n, 2);
	if (!i8)
	{
		printf("Invalid input\n");
		return 1;
	}
	printf("islice : ");
	while (has_next(i8))
	{
		printf("%d ", *(int*) next(i8));
	}
	printf("\n");
	free_iter(i8);
	free_iter(ai);

	list* l = (list*) malloc(sizeof(list));
	list_new(l, sizeof(int), NULL);
	ai = newArrayIter(b, n, sizeof(*b));
	while (has_next(ai))
		list_append(l, next(ai));
	free_iter(ai);
	iter* i9 = new_list_iter(l);
	if (!i9)
	{
		printf("Invalid input\n");
		return 1;
	}
	printf("List : ");
	while (has_next(i9))
	{
		printf("%d ", *(int*) next(i9));
	}
	printf("\n");
	free_iter(i9);

	char d[] = "Hello World!!";
	printf("Character Iter - ");
	iter* i2 = newArrayIter(d, strlen(d), sizeof(*d));
	if (!i2)
	{
		printf("Invalid input\n");
		return 1;
	}
	while(has_next(i2))
	{
		printf("%c", *(char*) next(i2));
	}
	printf("\n");
	free_iter(i2);

	return 0;
}
