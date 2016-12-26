#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "itertools.h"

const int ARRAY_ITER = 0x001;
const int LIST_ITER = 0x002;
const int TREE_ITER = 0x003;
const int GRAPH_ITER = 0x004;
const int COUNT_ITER = 0x005;
const int CYCLE_ITER = 0x006;
const int REPEAT_ITER = 0x007;
const int ACCUM_ITER = 0x008;
const int COMP_ITER = 0x009;
const int PERM_ITER = 0x00A;
const int COMB_ITER = 0x00B;
const int OTHER_ITER = 0x010;

int has_next(iter* i)
{
	if (i)
		return i->has_next(i);
	return 0;
}

void* next(iter* i)
{
	if (i)
		return i->next(i);
	return NULL;
}

void free_iter(iter* i)
{
	if (i->free_i)
		i->free_i(i);
	else
		free(i);
}

/* Array iterator */
int has_next_arr(iter* i)
{
	return i->ptr < i->end;
}

void* next_arr(iter* i)
{
	void* n = i->ptr;
	i->ptr += i->size;
	return n;
}

iter* newArrayIter(void* a, int n, size_t s)
{
	iter* i = (iter*) malloc(sizeof(iter));
	i->size = s;
	i->length = n;
	i->cont = a;
	i->ptr = a;
	i->end = a + s * n;
	i->next = &next_arr;
	i->has_next = &has_next_arr;
	i->free_i = NULL;
	i->type = ARRAY_ITER;
	return i;
}

/* Array iterator */
int fact(int n)
{
	if (n == 0)
		return 1;
	return n * fact(n-1);
}

int has_next_perm(iter* it)
{
	int* fin = (int*) it->ptr;
	if (*fin)
		return 0;
	return 1;
}

void* getcont(void* cont, int i, size_t s)
{
	while (i--)
		cont += s;
	return cont;
}

int find_next_index(int a[], int s, int l)
{
	int i2 = s+1;
	int fst = a[s];
	for (int i = s+2; i < l; i++)
		if (a[i] > fst && a[i] < a[i2])
			i2 = i;
	return i2;
}

int cmp(const void *a, const void * b)
{
	return *(int*) a - *(int*) b;
}

void* next_perm(iter* it)
{
	void* temp =  it->temp;
	void* t2 = temp;
	int n = it->length;
	int* a = (int*) it->temp2;
	for (int i = 0; i < n; i++)
	{
		memcpy(temp, getcont(it->cont, a[i], it->size), it->size);
		temp += it->size;
	}
	int* fin = (int*) it->ptr;
	int i = n-2;
	while (i >= 0 && a[i] >= a[i+1])
		i--;
	if (i == -1)
	{
		*fin = 1;
	} else
	{
		int i2 = find_next_index(a, i, n);
		int tmpv = a[i];
		a[i] = a[i2];
		a[i2] = tmpv;
		qsort(a+i+1, n-i-1, sizeof(int), cmp);
		it->temp2 = (void*) a;
	}
	return t2;
}

void* next_perm_r(iter* it)
{
	void* temp =  it->temp;
	void* t2 = temp;
	int n = it->length;
	int r = *(int*) it->end;
	int* a = (int*) it->temp2;
	for (int i = 0; i < r; i++)
	{
		memcpy(temp, getcont(it->cont, a[i], it->size), it->size);
		temp += it->size;
	}

	int* fin = (int*) it->ptr;
	int i = n-2;
	while (!*fin && i >= r)
	{
		i = n-2;
		while (i >= 0 && a[i] >= a[i+1])
			i--;
		if (i == -1)
		{
			*fin = 1;
		} else
		{
			int i2 = find_next_index(a, i, n);
			int tmpv = a[i];
			a[i] = a[i2];
			a[i2] = tmpv;
			qsort(a+i+1, n-i-1, sizeof(int), cmp);
			it->temp2 = (void*) a;
		}
	}
	return t2;
}

void free_perm(iter* i)
{
	free(i->ptr);
	free(i->temp);
	free(i->temp2);
	free(i);
}

iter* permutations(void* a, int n, int r, size_t s)
{
	if (n < r || n <= 0)
		return NULL;
	iter* i = (iter*) malloc(sizeof(iter));
	i->size = s;
	i->length = n;
	i->cont = a;
	int* fin = (int*) malloc(sizeof(int));
	*fin = 0;
	i->ptr = (void*) fin;
	void* tmp = (void*) malloc(n * s);
	memcpy(tmp, a, n*s);
	i->temp = tmp;

	int* t = (int*) malloc((n+1) * sizeof(int));
	for (int j = 0; j < n; j++)
	{
		t[j] = j;
	}
	i->temp2 = (void*) t;
	i->has_next = &has_next_perm;
	if (n == r || n-1 == r)
	{
		i->next = &next_perm;
		i->end = 0;
	} else
	{
		i->next = &next_perm_r;
		int* pr = (int*) malloc(sizeof(int));
		*pr = (r);
		i->end = pr;
	}
	i->free_i = &free_perm;
	i->type = PERM_ITER;
	return i;
}

int has_next_count(iter* i)
{
	return (*(int*) i->cont) > 0;
}

void* next_count(iter* i)
{
	int* c = (int*) i->cont;
	*c = *c + *(int*) i->ptr;
	return c;
}

void free_count(iter* i)
{
	free(i->cont);
	free(i->ptr);
	free(i);
}

iter* count(int start, int step)
{
	iter* i = (iter*) malloc(sizeof(iter));
	int* pr = (int*) malloc(sizeof(int));
	*pr = start-step;
	i->cont = pr;

	int* sr = (int*) malloc(sizeof(int));
	*sr = step;
	i->ptr = sr;
	
	i->next = &next_count;
	i->has_next = &has_next_count;
	i->free_i = &free_count;
	i->type = COUNT_ITER;
	return i;
}

int has_next_cycle(iter* i)
{
	return 1;
}

void* next_cycle(iter* i)
{
	void* n = i->ptr;
	i->ptr += i->size;
	if (i->ptr >= i->end)
	{
		i->ptr = i->cont;
	}
	return n;
}

iter* cycle(void* a, int n, size_t s)
{
	iter* i = (iter*) malloc(sizeof(iter));
	i->size = s;
	i->length = n;
	i->cont = a;
	i->ptr = a;
	i->end = a + s * n;
	i->next = &next_cycle;
	i->has_next = &has_next_cycle;
	i->free_i = NULL;
	i->type = CYCLE_ITER;
	return i;
}

int has_next_repeat(iter* i)
{
	return 1;
}

void* next_repeat(iter* i)
{
	return i->cont;
}

void free_repeat(iter* i)
{
	free(i->cont);
	free(i->ptr);
	free(i);
}

iter* repeat(int elem)
{
	iter* i = (iter*) malloc(sizeof(iter));
	int* pr = (int*) malloc(sizeof(int));
	*pr = elem;
	i->cont = pr;
	int* a = (int*) malloc(sizeof(int));
	i->ptr = a;
	i->next = &next_repeat;
	i->has_next = &has_next_repeat;
	i->free_i = NULL;
	i->type = REPEAT_ITER;
	return i;
}

int has_next_repeat_n(iter* i)
{
	return *(int*) i->ptr < i->length;
}

iter* repeatn(int elem, int n)
{
	iter* i = (iter*) malloc(sizeof(iter));
	i->length = n;
	int* pr = (int*) malloc(sizeof(int));
	*pr = elem;
	i->cont = pr;
	
	int* ptr = (int*) malloc(sizeof(int));
	*ptr = n;
	i->ptr = ptr;

	i->next = &next_repeat;
	i->has_next = &has_next_repeat_n;
	i->free_i = &free_repeat;
	i->type = ARRAY_ITER;
	return i;
}

/*void* next_accumulate(iter* i)
{
	int (*f)(const void*) = (int (*)(const void*)) i->temp;
	int* sum = (int*) i->temp2;
	*sum += f(i->ptr);
	i->ptr += i->size;
	return (void*) sum;
}

iter* accumulate(void* a, int n, size_t s, int (*fun)(const void*))
{
	iter* i = (iter*) malloc(sizeof(iter));
	i->size = s;
	i->length = n;
	i->cont = a;
	i->ptr = a;
	i->end = a + s * n;

	i->temp = fun;
	
	int* ptr = (int*) malloc(sizeof(int));
	*ptr = 0;
	i->temp2 = ptr;

	i->next = &next_accumulate;
	i->has_next = &has_next_arr;
	i->free_i = NULL;
	i->type = ARRAY_ITER;
	return i;
}*/

int has_next_accumulate_iter(iter* i)
{
	return has_next((iter*) i->cont);
}

void* next_accumulate_iter(iter* i)
{
	int (*f)(const void*, const void*) = (int (*)(const void*, const void*)) i->temp;
	int* sum = (int*) i->temp2;
	*sum = f(sum, next((iter*) i->cont));
	return (void*) sum;
}

void free_acc(iter* i)
{
	free(i->temp2);
	free(i);
}

iter* accumulate(iter* a, int (*fun)(const void*, const void*), int in)
{
	iter* i = (iter*) malloc(sizeof(iter));
	i->cont = a;
	i->temp = fun;
	
	int* ptr = (int*) malloc(sizeof(int));
	*ptr = in;
	//memcpy(ptr, next(a), a->size);
	i->temp2 = ptr;

	i->next = &next_accumulate_iter;
	i->has_next = &has_next_accumulate_iter;
	i->free_i = &free_acc;
	i->type = ACCUM_ITER;
	return i;
}

int has_next_compress(iter* i)
{
	return *(int*)i->end;// || has_next((iter*) i->cont);
}

void* next_compress(iter* i)
{
	i->temp2 = i->temp;
	if (has_next((iter*) i->cont))
	{
		int t = *(int*) next((iter*) i->ptr);
		void* a = next((iter*) i->cont);
		while (has_next((iter*) i->cont) && !t)
		{
			t = *(int*) next((iter*) i->ptr);
			a = next((iter*) i->cont);
		}
		i->temp = a;
		// printf("a - %d %d\n", *(int*) a, *(int*) i->temp2);
		int* e = (int*) i->end;
		*e = t? 1: 0;//has_next((iter*) i->cont);
	} else {
		int* e = (int*) i->end;
		*e = 0;//has_next((iter*) a);
	}
	return i->temp2;
}

void free_com(iter* i)
{
	free(i->end);
	free(i);
}

iter* compress(iter* a, iter* b)
{
	iter* i = (iter*) malloc(sizeof(iter));
	i->cont = a;
	i->ptr = b;

	if (has_next((iter*) a))
	{
		int t = *(int*) next((iter*) b);
		void* an = next((iter*) a);
		while (has_next((iter*) a) && !t)
		{
			t = *(int*) next((iter*) b);
			an = next((iter*) a);
		}
		i->temp = an;
		//printf("an - %d\n", *(int*) an);
		int* end = (int*) malloc(sizeof(int));
		*end = 1;
		i->end = end;
	} else {
		int* end = (int*) malloc(sizeof(int));
		*end = 0;//has_next((iter*) a);
		i->end = end;
	}

	i->next = &next_compress;
	i->has_next = &has_next_compress;
	i->free_i = &free_com;
	i->type = COMP_ITER;
	return i;
}

/* islice */
int has_next_islice(iter* i)
{
	return *(int*)i->end;// || has_next((iter*) i->cont);
}

void* next_islice(iter* i)
{
	i->temp2 = i->temp;
	if (has_next((iter*) i->cont))
	{
		int* st = (int*) i->ptr;
		int b = 0;
		while (b < st[1] && has_next((iter*) i->cont))
		{
			void* an = next((iter*) i->cont);
			i->temp = an;
			b++;
		}
		if (b == st[1])
			st[0] += b;
		else
			*(int*)i->end = 0;
	} else
		*(int*)i->end = 0;
	return i->temp2;
}

void free_isl(iter* i)
{
	free(i->ptr);
	free(i->end);
	free(i);
}

iter* islice(iter* a, int start, int stop, int step)
{
	if (start > stop || start < 0 || step <= 0)
		return NULL;
	start++;
	iter* i = (iter*) malloc(sizeof(iter));
	i->cont = a;
	int* st = (int*) malloc(3*sizeof(int));
	st[0] = 0;
	st[1] = step;
	st[2] = stop;
	i->ptr = st;

	int* end = malloc(sizeof(int));
	if (has_next((iter*) a))
	{
		int b = 0;
		while (b < start && has_next((iter*) a))
		{
			void* an = next((iter*) a);
			i->temp = an;
			b++;
		}
		if (b == start)
			st[0] = b;
		*end = b == start; //has_next((iter*) a);
	} else
		*end = 0;
	i->end = end;

	i->next = &next_islice;
	i->has_next = &has_next_islice;
	i->free_i = &free_isl;
	i->type = COMP_ITER;
	return i;
}

/* Combinations */
int has_next_comb(iter* it)
{
	int* a = (int*) it->temp2;
	int k = *(int*) it->ptr;
	return a[k-1] < it->length;
}

void* next_comb(iter* it)
{
	void* temp = it->temp;
	void* t2 = temp;
	int n = it->length;
	int* a = (int*) it->temp2;
	int k = *(int*) it->ptr;
	for (int i = 0; i < k; i++)
	{
		//printf("%d\n", a[i]);
		memcpy(temp, getcont(it->cont, a[i], it->size), it->size);
		temp += it->size;
	}
	int t = k - 1;
	while (t != 0 && a[t] == n - k + t) 
		t--;
	a[t]++;
	for (int i = t + 1; i < k; i++) 
		a[i] = a[i - 1] + 1;
	return t2;
}

void free_comb(iter* i)
{
	free(i->ptr);
	free(i->temp);
	free(i->temp2);
	free(i);
}

iter* combinations(void* a, int n, int r, size_t s)
{
	if (n < r || n <= 0)
		return NULL;
	iter* i = (iter*) malloc(sizeof(iter));
	i->size = s;
	i->length = n;
	i->cont = a;
	
	int* ptr = (int*) malloc(sizeof(int));
	*ptr = r;
	i->ptr = (void*) ptr;
	i->end = 0;
	
	void* tmp = (void*) malloc(n * s);
	memcpy(tmp, a, n*s);
	i->temp = tmp;

	int* t = (int*) malloc((r) * sizeof(int));
	for (int j = 0; j < r; j++)
	{
		t[j] = j;
	}
	i->temp2 = (void*) t;

	i->has_next = &has_next_comb;
	i->next = &next_comb;
	i->free_i = &free_comb;
	i->type = COMB_ITER;
	return i;
}

void* next_comb_r(iter* it)
{
	void* temp = it->temp;
	void* t2 = temp;
	int n = it->length;
	int* a = (int*) it->temp2;
	int k = *(int*) it->ptr;
	for (int i = 0; i < k; i++)
	{
		//printf("%d\n", a[i]);
		memcpy(temp, getcont(it->cont, a[i], it->size), it->size);
		temp += it->size;
	}
	int t = k - 1;
	while (t != 0 && a[t] == n - k + t) 
		t--;
	a[t]++;
	for (int i = t + 1; i < k; i++) 
		a[i] = a[t];
	return t2;
}

iter* combinations_replacement(void* a, int n, int r, size_t s)
{
	if (n < r || n <= 0)
		return NULL;
	iter* i = (iter*) malloc(sizeof(iter));
	i->size = s;
	i->length = n;
	i->cont = a;
	
	int* ptr = (int*) malloc(sizeof(int));
	*ptr = r;
	i->ptr = (void*) ptr;
	i->end = 0;
	
	void* tmp = (void*) malloc(n * s);
	memcpy(tmp, a, n*s);
	i->temp = tmp;

	int* t = (int*) malloc((r) * sizeof(int));
	for (int j = 0; j < r; j++)
	{
		t[j] = 0;
	}
	i->temp2 = (void*) t;

	i->has_next = &has_next_comb;
	i->next = &next_comb;
	i->free_i = &free_comb;
	i->type = COMB_ITER;
	return i;
}

int has_next_list(iter* i)
{
	listNode* l = (listNode*) i->cont;
	return l != NULL;
}

void* next_list(iter* i)
{
	listNode* l = (listNode*) i->cont;
	void* a = l->data;
	i->cont = l->next;
	return a;
}

iter* new_list_iter(list* l)
{
	iter* i = (iter*) malloc(sizeof(iter));
	i->cont = l->head;
	i->next = &next_list;
	i->has_next = &has_next_list;
	i->free_i = NULL;
	i->type = LIST_ITER;
	return i;
}
