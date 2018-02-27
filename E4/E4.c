#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(){
	pid_t input, parse, output;
	// Initial of 1 so 0 can be used as condition for identifying process
	input = 1;
	parse = 1;
	output = 1;

	int p1[2];
	pipe(p1);
	int p2[2];
	pipe(p2);

	pid_t main = getpid();
	if(getpid() == main)
		input = fork();
	if(getpid() == main)
		parse = fork();
	// if(getpid() == main)
	// 	output = fork();

	if(input == 0){
		close(p1[0]);
		close(p2[0]);
		close(p2[1]);

		while(1){
			char pc[1];
			pc[0] = getchar();
			write(p1[1],pc,1);
			if(pc[0] == EOF){
				break;
			}
		}
		close(p1[1]);

		return 0;
	}
	else if(parse == 0){
		close(p1[1]);
		close(p2[0]);

		while(1){
			char pc[1];
			read(p1[0],pc,1);
			char c = pc[0];
			if(c == '\n'){
				pc[0] = ' ';
				write(p2[1],pc,1);
			}else if(c == '*'){
				char pc2[1];
				read(p1[0],pc2,1);
				char c2 = pc2[0];
				if(c2 == '*'){
					pc[0] = '^';
					write(p2[1],pc,1);
				}else{
					write(p2[1],pc,1);
					write(p2[1],pc2,1);
					if(c2 == EOF){
						break;
					}
				}
			}else{
				write(p2[1],pc,1);
			}
			if(c == EOF){
				break;
			}
		}
		close(p1[0]);
		close(p2[1]);

		return 0;
	}else if(getpid() == main){
		close(p2[1]);
		close(p1[0]);
		close(p1[1]);

		while(1){
			char out[80];
			char pc[1];
			for(int i=0;i<80;i++){
				read(p2[0],pc,1);
				if(pc[0] == EOF){
					break;
				}
				out[i] = pc[0];
			}
			if(pc[0] == EOF){
				break;
			}
			for(int i=0;i<80;i++){
				putchar(out[i]);
			}
			putchar('\n');
		}
		close(p2[0]);

		return 0;
	}

	wait(NULL);
	return 0;
}
