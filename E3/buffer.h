#ifndef _BUFFER_H_
#define _BUFFER_H_

typedef struct buffer* buffer;

// Buffer constructor
buffer makeBuffer();
// Deposits char c into buffer b
void deposit(buffer b, char c);
// Removes and returns a char from buffer b
// Function name 'remove' is reserved so had to
// name 'remoove'
char remoove(buffer b);

#endif
