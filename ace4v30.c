/********************************************************
 * 
 * Synopsis:
 * 	The aim of this ace was to create a simple shell 
 * 	in C.
 * 
 * File: ace4v30.c
 * 
 * Authors:
 * 	Katie Reid, Carla Rankin, Rachel Maley,
 * 	Stephen Corcoran, Stephen Gray
 * 
 * Version: ace4v30
 * 
 *  Promise: We confirm that this submission is all my own work. 
 *
 *          (Signed)____________________________________________
 * (Katie Reid, Carla Rankin, Rachel Maley,
 * 	Stephen Corcoran, Stephen Gray)
 * 
 * ******************************************************/
 
 /*******************************************************
  * 
  * Stage One: The aim of stage 1 was to prompt the user for 
  * 	input, read that input and then parse it. Also if the 
  * 	user were to type exit or <ctrl>D the shell would exit.
  * 
  * Stage Two: The aim of stage 2 was to execute external 
  * 	commands the user input such as ls or clear. If 
  * 	the user were to input nonsense an error message 
  * 	should be returned. 
  * 
  * Stage Three: Stage three's aim was to set the current 
  * 	working directory to home, and getting and setting 
  * 	the path using getenv and setenv. The original path 
  * 	should also be restored at the exit of the program.
  * 
  * Stage Four: The aim of stage 4 was to change the directory
  * 	when the user input cd either with something after or without. 
  * 
  * Stage Five: The aim of stage 5 was adding commands to history, 
  * 	invoking these commands from history and printing the history. 
  * 
  * Stage Six: The aim of stage 6 was to create a persisent history. 
  * 	This was done by creating a .hist_list file which saved the
  * 	users history upon exit ready to be used next time the user 
  * 	opens the shell program. 
  * 
  * Stage Seven: The aim of stage 7 was to allow the user to set up
  * 	aliases, remove these aliases, invoke them and the print out 
  * 	all the aliases. 
  * 
  *		 Stage Eight was not attempted. 
  * 
  * ****************************************************************/


#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>

#define MAX 512
#define pointer ">"

char *par[MAX];
char input[1000];
char home[MAX];
char history[20][MAX];
int historyCount=0;
int histLoop = 0;
char aliasNames[10][MAX];
char aliasCommands[10][MAX];
int aliasCounter=0;
char tInp[1000];

char *inter[] = {"exit", "getpath", "setpath", "cd","history", "alias", "unalias"};

void updateHistory(char c[MAX]);/*function for updating history*/
void exit(int status); /*function for exiting the program*/

/*prints history*/
void printHistory(){
	int i;
	int j;
	
	if(histLoop == 0){
		for(i = 0; i< historyCount;i++){
			printf("%i %s \n",i+1,history[i]);
		}
	}
	else{
	for(j = 0; j< 20;j++){
			printf("%i %s \n",j+1,history[j]);
		}
	}

}

void loadHistory(){/*load history from file */

char loadPath[MAX];

strcpy(loadPath, getenv("HOME")); /*gets the path for users home dir*/
strcat(loadPath, "/.hist_list"); 

	FILE *fp;
	char line[MAX];
	int i;
	for (i = 0; i < 20; i++) {
		history[i][0] = '\0';
	}

	if( (fp = fopen(loadPath, "r")) != NULL){ /*check if file exists*/

		while(1) {
			if( (fgets(line, MAX, fp)) == NULL) /*checks if there is a history entry to be added */
				break;
			else{
			updateHistory(line);
}
	 	}

	 	fclose(fp);

	}
} //end loadHistory()

void saveHistoryFile(){

char savePath[MAX];

strcpy(savePath, getenv("HOME"));
strcat(savePath, "/.hist_list"); /*gets the path for users home dir*/

FILE *fp;
	fp = fopen(savePath, "w+");

if (fp == NULL)
{
printf("history file not found");
}
else{
 	int i = 0;
	while(strcmp(history[i], "") != 0){ 	
		fprintf(fp, "%s", history[i]);
		i++;
}

 	fclose(fp);
}
}


void updateHistory(char c[MAX]){  /*updates history file*/ 

	if(historyCount < 20) {

 		strcpy(history[historyCount], c);
 		historyCount++;

 	} else {
int i;
 		for(i=1; i<20; i++) //Array contents shifted to left.
 			strcpy(history[i-1], history[i]);

 		strcpy(history[19], c);

 	}
} //end updateHistory()


void saveHistory(){	
	int i;
	/*Shifts history array and then adds new user input to index 0*/
	for(i = 19;i >=0; i-- ){
		strcpy(history[i+1],history[i]);
	}
	
	strcpy(history[0],input);
	historyCount++;
	
	/*when the history has a max of 20, reset count */
	if(historyCount >19){
		histLoop = 1;
		historyCount = 0;
	}
}



void executeCMD(){ /*This executes any external commands the user enters*/

	int status;
	pid_t pid;
	/* fork a child process */
	pid = fork();
	
	if (pid == 0) { /* child process */
		if (execvp(par[0], par)){
			printf("%s is an Unknown Command 1\n", *par);
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

void addAlias() {/* adds an alias */
	int i;
	int inArray = 0;
	char tempArray[1000];
	int len;
	
	/*copies only the command to a temporary array and not the name */
	len = strlen(par[1]);
	len = len + 7;
	strncpy(tempArray,&tInp[len],MAX-4);

	
	if(aliasCounter < 10) {
		for(i = 0; i < aliasCounter; i++) {
			if(strcmp(aliasNames[i], par[1]) == 0) {				
				inArray = 1;
				
			}
		}
 		/*Overwrites an alias*/
		if(inArray == 1) {
			for(int j = 0; j <10;j++){
				int ret = strcmp(aliasNames[j],par[1]);
				if(ret == 0){
					strcpy(aliasCommands[j],tempArray);
				}
			}

			aliasCounter++;
		}
		else if(inArray == 0){
			strcpy(aliasNames[aliasCounter],par[1]);
			strcpy(aliasCommands[aliasCounter],tempArray);
			aliasCounter++;
		}		
	else {
		printf("\nNo more aliases can be set");
	}
}
}

void removeAlias() { /*removes an alias*/
	int i;
	int position = 0;
	int inArray = 0;
	
	for(i = 0; i < aliasCounter; i++) {		
		if(strcmp(aliasNames[i], par[1]) == 0) {		
			inArray = 1;
			position = i;	
		}
	}
	
	if(inArray == 1) { 
		if(aliasCounter == 1) {
			strcpy(aliasNames[0], " ");
			strcpy(aliasCommands[0], " ");
		}
		else {
			i = position;
		
			for(i = i; i < aliasCounter-1; i++) {
				strcpy(aliasNames[i], aliasNames[i+1]);
				strcpy(aliasCommands[i], aliasCommands[i+1]);				
			}
			strcpy(aliasNames[aliasCounter], " ");
			strcpy(aliasCommands[aliasCounter], " ");	
		}
		aliasCounter--;	
		printf("\nalias has been removed");
	}
	else {
		printf("\nno alias exists for this command");	
	}
	
}

void printAliases() {
	int i;
	if(aliasCounter == 0) {
		printf("There are no aliases to display");
	}
	else {	/*prints list of aliases next to their command*/
		for(i = 0;i < aliasCounter;i++) {
			printf("\n name: %s command: %s", aliasNames[i], aliasCommands[i]);
		}
	}
}

/*This runs any internal commands that we have coded as they
cannot be invoked using the executeCMD method as it does not 
recoginise them. 

The commands this method can invoke are, exit, getpath, setpath,
cd, history, alias and  unalias. */

void runInternal(int m){ 
int result;

	switch(m){
		case 0:
			strcpy(home, getenv("HOME"));
			int i = chdir(home);
			if(i<0) printf("Couldn't restore\n");
			else{
				printf("directory changed\n");
				printf("HOME: %s\n", home);
			}
			exit(0);
		case 1:
				printf("PATH = %s\n", getenv("PATH"));
				
			break;
		case 2:
			if(par[1] == NULL){
					printf("%s is an Unknown command 3\n", *par);
			}
			else{
				int newPath = setenv(par[1], par[1], 0);
				printf("%d", newPath);
				
			}
			break;
		case 3:
			
			result= 0;
			if(par[1] == NULL){
				chdir("..");
			}
			else{
				result = chdir(par[1]);
				
				if(result == 0){
					printf("\nDirectory changed to %s\n", get_current_dir_name());
				}	
				else{
					switch(errno){
					case EACCES: perror("Permission denied");
						break;
					case EIO:	 perror("");
						break;
					case ENAMETOOLONG: perror("");
						break;
					case ENOTDIR: perror(""); 
						break;
					case ENOENT: perror(""); 
      						break;
					}
				}
				
			}
			break;
		case 4:
			printHistory();			
			break;
		case 5: if(par[1] == NULL || par[2] == NULL) {
				printAliases();
			} else {
				addAlias();
			}
			break;
		case 6:	
			if(par[1] == NULL) {
				printf("No command has been specified");
			} else {
				removeAlias();				
			}
	}
}


/* This recognises whether or not the user has entered anything at all
 and if they have then checks if it is an internal command which will 
 invoke runInternal, if it is not an internal command executeCMD is called.*/
void run(){
	int m;
	int intersize = sizeof(inter)/sizeof(char *);
	int in;
	in = 0;

		if(par[0] == NULL){
			printf("No command entered");
		}
	
		for(m = 0; m < intersize; m++){
			if(strcmp(par[0], inter[m]) == 0){
				in = 1;
				runInternal(m);
			}
		}
		if(in == 0) executeCMD();
		memset(par,0,sizeof(par));
		
}

void invokeAlias() {
	int i;
	for(i = 0; i < aliasCounter; i++) {
		if(strcmp(aliasNames[i], par[1]) == 0) {
			strcpy(aliasCommands[i], par[1]);
		}
	}	
}

/*This parses the users input and uses the tokeniser array to check 
under what circumstances the input should be split into seperate tokens.*/

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
		p = strtok(NULL, tokeniser);
	}
	

	run();
}

void historyCommand(){


	char strin[1000];
	char *ptr;
	long ret;
	int choice;
	int i;
	int k = strncmp(input, "!-", 2);
	char tempArray[1000];
	int len;
if(k!=0){
	strncpy(strin, &input[1],3); //Ignores the first character of input and copies
					// the next 3 chars since first char is !

	ret = strtol(strin,&ptr,10);
	choice = ret;
	choice--;
	

	/*Validates user's choice for non minus*/
	if(choice >= 0 && choice < 10){
		strncpy(tempArray,&input[2],MAX-3);
		
	}
	else if(choice >= 10){
		strncpy(tempArray,&input[3],MAX-4);
	}
	else{
		printf("Invalid History number entered");
		return;
	}
	
	/*Copies the history command to the user input then command is tokenised*/
	if(histLoop == 0 && choice >= 0 && choice <= historyCount){
		
			strcpy(input,history[choice]);
			len = strlen(input);
			strcpy(&input[len],tempArray);
			
			tokenise();	
	}
	else if(histLoop == 1 && choice >= 0 && choice < 21){
			strcpy(input,history[choice]);
			len = strlen(input);
			strcpy(&input[len],tempArray);
			tokenise();
	}
	else{
		printf("%s : event not found here",input);
	}
}
else{
		strncpy(strin, &input[2],3);

		ret = strtol(strin,&ptr,10);
		choice = ret;


	/*Validates user's choice for minus history*/
	if(choice > 0 && choice < 10){
		strncpy(tempArray,&input[4],MAX-3);
		
	}
	else if(choice >= 10 && choice <20){
		strncpy(tempArray,&input[5],MAX-4);
	}
	else{
	printf("Invalid History number entered");
	return;
	}
	

	if(histLoop == 0 && choice > 0 && choice <= historyCount){	
		i = historyCount - choice;
		i--;
			if(i > 0){
			strcpy(input,history[i]);
			len = strlen(input);
			strcpy(&input[len],tempArray);
			tokenise();
			}
	}
	
	


	else if(histLoop == 1 && choice > 0 && choice < 20){
			i = 20 - choice;
			i--;
			if(i>0){
			strcpy(input,history[i]);
			len = strlen(input);
			strcpy(&input[len],tempArray);
			tokenise();	
			}
	}
	

}
	
}

/* This prints the pointer prompting the user for input 
and then checks if the user has entered an alias a history 
command or if the input should then be tokenised. */


void getInput(){
	
	int i, t, k;
	
	while(1){
		printf(pointer);
		if(fgets(input, MAX, stdin) == NULL){
			exit(0);
		}
		strcpy(tInp,input);
		/*Checks to see if user entered an alias*/
		for(i = 0;i < 10;i++){
			int len = strlen(input);
			int ret = strncmp(input,aliasNames[i],len-1);
			if(ret == 0){
				strcpy(input,aliasCommands[i]);
			}
		}

		/*Checks to see if user entered a history command*/
		 t = strncmp(input, "!", 1);
		 k = strncmp(input, "!-", 2);
		if( t != 0){
			saveHistory();
			saveHistoryFile();
		}

		if(t == 0 || k==0){
			historyCommand();
		}
		else tokenise() ;

	}
}

/*Main method that is called when the program initiates*/


int main(){

char path[MAX];
	strcpy(path, getenv("PATH"));
	strcpy(home, getenv("HOME"));
	loadHistory();
	chdir(home);
	printf("PATH :%s\n",(path!=NULL)? path : "getenv returned NULL");	
	printf("Home: %s\n ",home); 	
	getInput();
	return(0);
}
