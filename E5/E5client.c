#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Socket.h"

#define MAX_INPUT 100

int main(int argc, char* argv[]){
	if(argc != 3){
		printf("Requires arguments: server DNS name, server port number\n");
		return -1;
	}
	
	Socket s = Socket_new(argv[1],atoi(argv[2]));
	char input[MAX_INPUT];
	int i;

	if(s < 0){
		printf("Connection failed\n");
		return -1;
	}
	while(1){
		putchar('%');
		if(fgets(input,MAX_INPUT,stdin) == NULL){
			putchar('\n');
			Socket_close(s);
			return 1;
		}
		int len = strlen(input) + 1;
		input[len - 1] = '\0';
		for(i = 0; i < len; i++){
			int c = input[i];
			if(Socket_putc(c,s) == EOF){
				printf("EOF or error\n");
				Socket_close(s);
				return -1;
			}
		}
		// Exec results
		while(1){
			int c = Socket_getc(s);
			if(c == EOF){
				Socket_close(s);
				exit(-1);
			}else{
				printf("%c",c);
				if(c == '\0'){
					break;
				}
			}
		}
		// Response
		while(1){
			int c = Socket_getc(s);
			if(c == EOF){
				Socket_close(s);
				exit(-1);
			}else{
				printf("%c",c);
				if(c == '\0'){
					break;
				}
			}
		}
	}
	Socket_close(s);
	return 1;
}
