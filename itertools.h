#ifndef ITERTOOLS_H
#define ITERTOOLS_H 1
#include <stdlib.h>
#include "ll.h"
//#include "tree.h"

#define array(t) struct { size_t n; t* a; } *
#define new_a(t) malloc(sizeof(array(t)))
#define init_array(b, t, arr, ne) {b = new_a(t); (b)->a = arr; (b)->n = ne; }

extern const int ARRAY_ITER;
extern const int LIST_ITER;
extern const int OTHER_ITER;
extern const int TREE_ITER;
extern const int GRAPH_ITER;
extern const int COUNT_ITER;
extern const int CYCLE_ITER;
extern const int REPEAT_ITER;
extern const int ACCUM_ITER;
extern const int COMP_ITER;
extern const int PERM_ITER;
extern const int COMB_ITER;

typedef struct it {
	int type;
	size_t size;
	int length;
	void* ptr;
	void* end;
	void* cont;
	void* temp;
	void* temp2;
	void* (*next)(struct it* i);
	int (*has_next)(struct it* i);
	void (*free_i)(struct it* i);
} iter;

int has_next(iter* i);
void* next(iter* i);
void free_iter(iter* i);

iter* newArrayIter(void* a, int n, size_t s);
iter* new_list_iter(list* l);
//iter* new_tree_iter_inorder(tree* t);
iter* count(int start, int step);
iter* cycle(void* a, int n, size_t s);
iter* repeat(int elem);
iter* repeatn(int elem, int n);
//iter* accumulate(void* a, int n, size_t s, int (*fun)(const void*));
iter* accumulate(iter* a, int (*fun)(const void*, const void*), int in);
iter* compress(iter* a, iter* b);
iter* islice(iter* a, int start, int stop, int step);
iter* permutations(void* a, int n, int r, size_t size);
iter* combinations(void* a, int n, int r, size_t s);
iter* combinations_replacement(void* a, int n, int r, size_t s);

#endif
