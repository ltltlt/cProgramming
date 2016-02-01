#include<assert.h>
#include<stdlib.h>
#include"list.h"
#define NDEBUG
listptr createList(void* x){
	listptr p=(listptr)malloc(sizeof(listnode));
	p->x=x;
	p->next=NULL;
	return p;
}
listptr push(listptr list,void* x){
	assert(list);
	listptr p;
	p=(listptr)malloc(sizeof(listnode));
	p->next=list;
	p->x=x;
	return p;
}
void* pop(listptr* list){
	assert(*list);
	void* x=(*list)->x;
	listptr p=(*list)->next;
	free(*list);
	*list=p;
	return x;
}
void append(listptr list,void* x){
	assert(list);
	listptr *p=&list;
	while(*p)
		p=&(*p)->next;
	*p=(listptr)malloc(sizeof(listnode));
	(*p)->x=x;
}
void freelist(listptr* list){
	assert(*list);
	listptr next;
	for(;*list;*list=next){
		next=(*list)->next;
                free((*list)->x);
		free(*list);
		*list=NULL;
	}
}
void listmap(listptr list,void apply(void* x,void* cl),void* cl){
	assert(list);
	for(;list;list=list->next)
		apply(list->x,cl);
}
