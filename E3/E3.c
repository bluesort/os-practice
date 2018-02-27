#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <malloc.h>
#include "st.h"
#include "buffer.h"

buffer rawInput;
buffer parsed;

static void* input(void* arg){
	while(1){
		char c = getchar();
		deposit(rawInput,c);
		if(c == EOF){
			printf("<input terminating>\n");
			return 0;
		}
	}
}

static void* parse(void* arg){
	while(1){
		char c = remoove(rawInput);
		if(c == '\n'){
			deposit(parsed,' ');
		}else if(c == '*'){
			char c2 = remoove(rawInput);
			if(c2 == '*'){
				deposit(parsed,'^');
			}else{
				deposit(parsed,c);
				deposit(parsed,c2);
				if(c2 == EOF){
					return 0;
				}
			}
		}else{
			deposit(parsed,c);
		}
		if(c == EOF){
			return 0;
		}
	}
}

static void* output(void* arg){
	while(1){
		char c;
		char out[80];
		for(int i=0;i<80;i++){
			c = remoove(parsed);
			if(c == EOF){
				return 0;
			}
			out[i] = c;
		}
		for(int i=0;i<80;i++){
			putchar(out[i]);
		}
		putchar('\n');
	}
}


int main(){
	rawInput = makeBuffer();
	parsed = makeBuffer();

	st_init();
	st_thread_t inputThread = st_thread_create(input,NULL,1,0);
	st_thread_t parsingThread = st_thread_create(parse,NULL,1,0);
	st_thread_t outputThread = st_thread_create(output,NULL,1,0);

	st_thread_join(inputThread,NULL);
	return 0;
}
