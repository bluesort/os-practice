#include <malloc.h>
#include <stdio.h>
#include "buffer.h"
#include "semaphore.h"

#define BUFFER_MAX 160

struct buffer{
	semaphore empty;
	semaphore full;
	int nextIn;
	int nextOut;
	char buff[BUFFER_MAX];
};

buffer makeBuffer(){
	buffer retVal = malloc(sizeof(struct buffer));
	retVal->nextIn = 0;
	retVal->nextOut = 0;
	createSem(&(retVal->empty),BUFFER_MAX);
	createSem(&(retVal->full),0);
	return retVal;
}

void deposit(buffer b,char c){
	down(&(b->empty));
	b->buff[b->nextIn] = c;
	b->nextIn++;
	if(b->nextIn == BUFFER_MAX)
		b->nextIn = 0;
	up(&(b->full));
}

char remoove(buffer b){
	down(&(b->full));
	char c = b->buff[b->nextOut];
	b->nextOut++;
	if(b->nextOut == BUFFER_MAX)
		b->nextOut = 0;
	up(&(b->empty));
	return c;
}
