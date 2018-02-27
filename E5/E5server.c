#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "Socket.h"

#define MAX_INPUT 100
int parse(void);

ServerSocket s1;
Socket s2;

int main(int argc, char* argv[]){
	if(argc != 2){
		printf("Requires argument: receiving port number\n");
		return -1;
	}

	pid_t cpid, tpid;

	s1 = ServerSocket_new(atoi(argv[1]));
	if(s1 < 0){
		printf("Connection failed\n");
		return -1;
	}
	while(1){
		s2 = ServerSocket_accept(s1);
		if(s2 < 0){
			printf("Server socket accept failed\n");
			exit(-1);
		}
		cpid = fork();
		if(cpid == -1){
			char response[MAX_INPUT];
			sprintf(response,"Fork error\n");
			for(int j = 0; j < strlen(response); j++){
				Socket_putc(response[j],s2);
			}
			Socket_putc('\0',s2);
			exit(-1);
		}else if(cpid == 0){
			parse();
			Socket_close(s2);
			exit(0);
		}else{
			Socket_close(s2);
			tpid = waitpid(-1, NULL, WNOHANG);
		}
	}

}

int parse(){

	Socket_close(s1);
	while(1){
		char input[MAX_INPUT];
		char* arg;
		char* args[MAX_INPUT];
		char path[MAX_INPUT];
		pid_t cpid;
		FILE *f;
		char fileName[MAX_INPUT];
		sprintf(fileName, "tmp%ld.txt", (long)getpid());

		for(int i = 0; i < MAX_INPUT; i++){
			input[i] = Socket_getc(s2);
			if(input[i] == EOF)
				return 0;
			if(input[i] == '\0')
				break;
		}
		arg = strtok(input," \t\n");
		strcpy(path,"");
		strcat(path,arg);
		args[0] = path;
		int i = 1;
		while(arg != NULL){
			arg = strtok(NULL," \t\n");
			args[i] = arg;
			i++;
		}
		cpid = fork();
		if(cpid==0){
			// Child
			if(freopen(fileName, "w+", stdout) == '\0'){
				char response[MAX_INPUT];
				sprintf(response,"Error opening tmp file\n");
				for(int j = 0; j < strlen(response); j++){
					Socket_putc(response[j],s2);
				}
				Socket_putc('\0',s2);
			}
			execvp(path,args);
			exit(1);
		}else{
			// Parent
			int childStatus;
			// Wait for child
			waitpid(-1, &childStatus, 0);
			// Read file
			f = fopen(fileName,"r");
			char c;
			while((c=fgetc(f))!=EOF){
				Socket_putc(c,s2);
			}
			Socket_putc('\0',s2);

			char response[MAX_INPUT];
			sprintf(response,"End of redirected stdout. Child terminated with status %d\n",WEXITSTATUS(childStatus));
			for(int j = 0; j < strlen(response); j++){
				Socket_putc(response[j],s2);
			}
			Socket_putc('\0',s2);

			fclose(f);
			remove(fileName);
		}
	}
	Socket_close(s2);
	return 1;
}
