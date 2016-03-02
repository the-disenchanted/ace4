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
	exit(0);
	return(0);
	
}

void execProcess(char **userInput){
	
	pid_t pid;
		/* fork a child process */
		pid = fork();
		
		if (pid < 0) { /* error occurred */
		printf("Fork Failed");
		exit(1);
	}
	else if (pid == 0) { /* child process */
		execvp(userInput[0], userInput);
	}
	else { /* parent process */
		/* parent will wait for the child to complete */
		wait(NULL);
		printf("Child Complete");
	}	
}

void tokeniser(){
	char s[50];
	char tokeniser[20] = "\t \n <>&|;";
	char *p = strtok(s, tokeniser);
	int ex;
	
	printf(VERSION);

	while(1){
		printf(pointer);
		strcpy(userInput, getUserInput());
		p = strtok(userInput, tokeniser);
		
		if (p != NULL){	
			ex = strncmp(p,"exit",4);
			if(ex == 0) {
				exit(0);
			}
		}
		while (p != NULL){
      			printf ("(%s)\n",p);
			p = strtok (NULL, tokeniser);
		}
	}
}
	

int main(){

	tokeniser();
	execProcess(userInput);
	return 0;
	
}
