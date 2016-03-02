/************************Revision History*********************************
 *
 *	v1.1 18/2/2016 When the user inputs more than 512 characters the
 *      input is broken into blocks of 512 and then each block printed
 *      seperately rather than only the first 512 being printed. 
 *
 *  v1.2 25/02/2016 Removed characters from parsing that weren't needed
 *		and removed the error message for a blank line as Sotirios wasn't
 *		happy with it. Created a variable 'tokeniser' to hold the parsing characters
 *		so that it is easier to make changes in the future. 
 *		I have added moved the string compare method to the place where the strings
 *		are tokenised so that we can control the parameters of 'exit'. Quick hack to
 *		begin with but probably be better using a case statement later when dealing with
 * 		multiple commands.
 *************************************************************************/

#define VERSION "ACE2 (v3.0) by Stephen Corcoran, Katie Reid, Carla Rankin, Stephen Gray, Rachel Maley. Updated: 11/02/2016\n"

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

#define Max_Input 512
#define pointer ">"

char userInput[Max_Input];

void exit(int status); /*function for exiting the program*/

char *getUserInput(void){
	
	static char input[Max_Input];
	
	if(fgets(input, Max_Input,stdin)!=NULL)
	{
	if('\n' == input[0]){
		return("");
	}
		return input;
	}
	return(0);
	exit(0);
	
	
}

void execProcess(char **userInput){
	
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
		printf("*%s*\n", userInput[0]);
		execvp(userInput[0], userInput);
		printf("Error");
	}
	else { /* parent process */
		/* parent will wait for the child to complete */
		printf("reached else\n");
		while(wait(&status)!=pid);
	}	
}

void tokeniser(char** userInput){
	char s[50];
	char tokeniser[20] = "\t \n <>&|;";
	char *p = strtok(s, tokeniser);
	int ex;
	char *par[100] = {NULL};
	int i = 0;
	
	printf(VERSION);

	while(1){
		printf(pointer);
		strcpy(userInput, getUserInput());
		p = strtok(userInput, tokeniser);
		
		if (p != NULL){	
			ex = strcmp("exit",p);
			if(ex == 0) {
				exit(0);
			}
		}
		while (p != NULL){
      			printf ("(%s)\n",p);
			strcpy(p,par[i]);
			++i;
			printf("*%s*\n", par[i]);
			p = strtok (NULL, tokeniser);
		}
	}
	execProcess(par);
	
}

int main(int args, char** userInput){

	tokeniser(userInput);
	printf("*%s*\n", userInput[0]);
	/*execProcess(userInput);*/
	return 0;
	
}
