#include <unistd.h>
#include <malloc.h>
#include <semaphore.h>
#include <sys/mman.h>
#include "buffer.h"

#define BUFFER_MAX 160

struct buffer{
	sem_t empty;
	sem_t full;
	int nextIn;
	int nextOut;
	char buff[BUFFER_MAX];
};

buffer makeBuffer(){
	// Buffer constructor
	buffer retVal = mmap(0,getpagesize(),PROT_READ | PROT_WRITE,MAP_SHARED | MAP_ANONYMOUS,-1,0);
	retVal->nextIn = 0;
	retVal->nextOut = 0;
	sem_init(&(retVal->empty),1,BUFFER_MAX);
	sem_init(&(retVal->full),1,0);
	return retVal;
}

void deposit(buffer b,char c){
	// Deposits character into buffer if( empty > 0 && full < BUFFER_MAX )
	sem_wait(&(b->empty));
	b->buff[b->nextIn] = c;
	b->nextIn++;
	if(b->nextIn == BUFFER_MAX)
		b->nextIn = 0;
	sem_post(&(b->full));
}

char remoove(buffer b){
	// Removes and returns character from buffer if( full > 0 && empty < BUFFER_MAX )
	sem_wait(&(b->full));
	char c = b->buff[b->nextOut];
	b->nextOut++;
	if(b->nextOut == BUFFER_MAX)
		b->nextOut = 0;
	sem_post(&(b->empty));
	return c;
}
