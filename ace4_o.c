#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

#define MAX 9000

void executeCMD(char **par){
	int status;
	pid_t pid;
	printf("runnig execp\n");
	/* fork a child process */
	pid = fork();
		
	if (pid < 0) { /* error occurred */
		printf("Fork Failed");
		exit(1);
	}
	
	if (pid == 0) { /* child process */
		printf("*%s*\n", par[0]);
		execvp(par[0], par);
		printf("Error");
	}
	else { /* parent process */
		/* parent will wait for the child to complete */
		printf("reached else\n");
		while(wait(&status)!=pid);
	}	
}

void tokenise(char *input){
	const char tokeniser[20] = "\t \n <>&|;";
	char *p = strtok(input, tokeniser);
	int i = 0;
	char *par[MAX] = {NULL};
	
	if(NULL == p){
		return;
	}
	
	while(p != NULL){
		par[i] = p;
		++i;
		p = strtok(NULL, tokeniser);
	}
	executeCMD(par);
}

void getInput(){
	char input[1000];
	if(fgets(input, MAX, stdin) == NULL){
		exit(0);
	}
	tokenise(input);
}

int main(){
	
	getInput();
	return(0);
}
