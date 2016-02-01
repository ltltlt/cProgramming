#ifndef LIST_INCLUDED
#define LIST_INCLUDED

typedef struct listnode *listptr;
typedef struct listnode{
	listptr next;
	void* x;
}listnode;
extern void* pop(listptr*);
extern listptr createList(void* );
extern listptr push(listptr,void* );
extern void append(listptr,void* );
extern void freelist(listptr*);
extern void listmap(listptr ,void apply(void*,void*),void* );
#endif
