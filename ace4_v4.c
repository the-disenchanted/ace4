#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

#define MAX 512
#define pointer ">"

void exit(int status); /*function for exiting the program*/

void executeCMD(char **par){

	int status;
	pid_t pid;
	/* fork a child process */
	pid = fork();
	
	if (pid == 0) { /* child process */
		if (execvp(par[0], par)){
			printf("%s is an Unknown Command\n", *par);
		exit(0);
		}
	}
	else if (pid < 0) { /* error occurred */
		printf("Fork Failed");
		exit(1);
	}
	else { /* parent process */
		/* parent will wait for the child to complete */
		while(wait(&status)!=pid);
	}	
}

void tokenise(char *input){

	char tokeniser[50] = "\t \n <>&|;";
	char *p = strtok(input, tokeniser);
	int i = 0;
	char *par[MAX] = {NULL};
	int ex;
	
	if(NULL == p){
		return;
	}

	if (p != NULL){	
		ex = strcmp(p,"exit");
		if(ex == 0) {
			exit(0);
printf("\n");
		}
	}
	
	while(p != NULL){
		par[i] = p;
		++i;
		printf("(%s)\n", p);
		p = strtok(NULL, tokeniser);
	}
	executeCMD(par);
}

void getInput(){
	char input[1000];

while(1){
	printf(pointer);
	if(fgets(input, MAX, stdin) == NULL){
		exit(0);
	}
	tokenise(input);
}
}

int main(){
	
	getInput();
	return(0);
}
