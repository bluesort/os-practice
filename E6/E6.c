#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <malloc.h>
#include "buffer.h"

buffer rawInput;
buffer parsed;

static int input(){
	// Deposits characters into rawInput buffer until EOF is encountered
	while(1){
		char c = getchar();
		deposit(rawInput,c);
		if(c == EOF){
			return 0;
		}
	}
}

static int parse(){
	// Reads characters from rawInput buffer, parses them, and places results
	// in parsed buffer. Loops until encountering EOF
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

static int output(){
	// Reads characters from parsed buffer and prints them in complete 80
	// character lines until EOF is encountered
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
	pid_t mainPID, inputPID, parsePID;

	// Create two buffers with anonymous memory mapped shared files
	rawInput = makeBuffer();
	parsed = makeBuffer();

	// Initial of 1 so 0 can be used as condition for identifying process
	inputPID = 1;
	parsePID = 1;

	// Fork and assign PIDs, with main process handling output
	mainPID = getpid();
	if(getpid() == mainPID){
		inputPID = fork();
	}
	if(getpid() == mainPID){
		parsePID = fork();
	}

	// Send each process to the corresponding function
	if(inputPID == 0){
		input();
		exit(0);
	}else if(parsePID == 0){
		parse();
		exit(0);
	}else if(getpid() == mainPID){
		output();
		exit(0);
	}

	return 0;
}
