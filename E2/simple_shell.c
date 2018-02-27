#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

void main(){
	int running = 1;
	while(running){
		char input[100];
		char **argv = malloc(50 * sizeof(char*));
		char *arg;
		char path[100];

		putchar('%');
		fgets(input,100,stdin);
		arg = strtok(input," \t\n");
		strcpy(path,"");
		strcat(path,arg);
		int i = 0;
		while(arg != NULL){
			arg = strtok(NULL," \t\n");
			argv[i] = arg;
			i++;
		}

		pid_t childPID = fork();
		if(childPID == 0){
			execvp(path,argv);
		}else{
			wait(NULL);
		}
		putchar('\n');
	}
}
