#include <stdio.h>
#include <string.h>

int main(){
	int parsing = 1;
	int valid = 1;
	char arr[80];
	while(parsing){
		for(int i=0; i < 80; i++){
			char c = getchar();
			if(c == EOF){
				parsing = 0;
				if(i < 80){
					valid = 0;
				}
				break;
			}else if(c == '\n'){
				c = ' ';
				arr[i] = c;
			}else if(c == '*'){
				char c2 = getchar();
				if(c2 == '*'){
					arr[i] = '^';
				}else{
					if(c2 == EOF){
						arr[i] = c;
						parsing = 0;
						if(i < 80){
							valid = 0;
						}
						break;
					}else{
						arr[i] = c;
						i++;
						arr[i] = c2;
					}
				}
			}else{
				arr[i] = c;
			}
		}
		if(valid){
			for(int i=0; i < 80; i++){
				putchar(arr[i]);
			}
			printf("\n");
		}
	}
	return 0;
}
