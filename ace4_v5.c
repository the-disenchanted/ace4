#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

#define MAX 512
#define pointer ">"

char *par[MAX] = {NULL};
char input[1000];
const char* home;

void exit(int status); /*function for exiting the program*/

void executeCMD(){

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




void runInternal(){
	int ex;
	int setp;
	int i;
	for(i = 0; i < MAX - 1; i++) {
		if(par[i] != NULL) {
			ex = strcmp(par[i],"exit");
			setp = strcmp(par[0], "getpath");

			if(setp == 0) {
			
			}
			if(ex == 0) {
				exit(0);
}
			else{
				executeCMD();
			}
		}	
	}
}

void tokenise(){
	char tokeniser[50] = "\t \n <>&|;";
	char *p = strtok(input, tokeniser);
	int i = 0;
			
	if(NULL == p){
		return;
	}
	
	while(p != NULL){
		par[i] = p;
		++i;
		printf("(%s)\n", p);
		p = strtok(NULL, tokeniser);
	}
	runInternal();
}

void getInput(){
	/*char input[1000];*/

	while(1){
		printf(pointer);
		if(fgets(input, MAX, stdin) == NULL){
			exit(0);
		}
		tokenise(input);
	}
}



int main(){
const char* path = getenv("PATH");
	home = getenv("HOME");
	chdir(home);
	printf("PATH :%s\n",(path!=NULL)? path : "getenv returned NULL");	
	printf("Home: %s\n ",home); 	
	getInput();
	return(0);
}
