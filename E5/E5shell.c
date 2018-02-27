#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_INPUT 100

void main(){
	int running = 1;
	while(running){
		char input[MAX_INPUT];
		char **argv = malloc(MAX_INPUT * sizeof(char*));
		char *arg;
		char path[MAX_INPUT];

		putchar('%');
		if(!fgets(input,MAX_INPUT,stdin)){
			putchar('\n');
			return;
		}
		arg = strtok(input," \t\n");
		argv[0] = arg;
		strcpy(path,"");
		strcat(path,arg);
		int i = 1;
		while(arg != NULL){
			arg = strtok(NULL," \t\n");
			argv[i] = arg;
			i++;
		}

		pid_t childPID = fork();
		if(childPID == 0){
			execvp(path,argv);
			return;
		}else{
			wait(NULL);
		}
	}
}
