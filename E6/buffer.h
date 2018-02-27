#ifndef _BUFFER_H_
#define _BUFFER_H_

typedef struct buffer* buffer;

buffer makeBuffer();
void deposit(buffer b, char c);
char remoove(buffer b);

#endif
