#ifndef __LIST_H
#define __LIST_H

typedef void (*freeFunction)(void *);
 
typedef enum { FALSE, TRUE } bool;
 
typedef bool (*listIterator)(void *);
 
typedef struct _listNode {
	void *data;
	struct _listNode *next;
} listNode;
 
typedef struct {
	int logicalLength;
	int elementSize;
	listNode *head;
	listNode *tail;
	freeFunction freeFn;
} list;
 
void list_new(list *list, int elementSize, freeFunction freeFn);
void list_destroy(list *list);
 
void list_prepend(list *list, void *element);
void list_append(list *list, void *element);
int list_size(list *list);

void list_remove_head(list *list, void *element);

#endif
