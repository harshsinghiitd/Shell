#include<stdio.h> 
#include<string.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<sys/wait.h> 
#include<readline/readline.h> 
#include<readline/history.h>



#define MAX_CMD_LEN  128
#define HISTORY_COUNT 20
#define MAXCOM 1000 // max number of letters to be supported 
#define MAXLIST 100 // max number of commands to be supported 

int history(char *hist[], int current)
{
        int i = current;
        int hist_num = 1;
        do {
                if (hist[i]) {
                        printf("%4d  %s\n", hist_num, hist[i]);
                        hist_num++;
                }

                i = (i + 1) % HISTORY_COUNT;
        } while (i != current);
        return 0;
}
int clear_historyy(char *hist[])
{
        int i;
        for (i = 0; i < HISTORY_COUNT; i++) {
                free(hist[i]);
                hist[i] = NULL;
        }
        return 0;
}
// Clearing the shell using escape sequences 
#define clear() printf("\033[H\033[J") 
  

// Function to take input 
int takeInput(char* str) 
{ 
    char* buf; 
  
    buf = readline("\n>>> "); 
    if (strlen(buf) != 0) { 
        add_history(buf); 
        strcpy(str, buf); 
        return 0; 
    } else { 
        return 1; 
    } 
} 
  

  
// Function where the system command is executed 
void execArgs(char** parsed) 
{ 
    // Forking a child 
    pid_t pid = fork();  
  
    if (pid == -1) { 
        printf("\nFailed forking child.."); 
        return; 
    } else if (pid == 0) { 
        if (execvp(parsed[0], parsed) < 0) { 
            printf("\nCould not execute command.."); 
        } 
        exit(0); 
    } else { 
        // waiting for child to terminate 
        wait(NULL);  
        return; 
    } 
} 
  


  
// Function to execute builtin commands 
int ownCmdHandler(char** parsed) 
{ 
    int NoOfOwnCmds = 3, i, switchOwnArg = 0; 
    char* ListOfOwnCmds[NoOfOwnCmds]; 
    char* username; 
  
    ListOfOwnCmds[0] = "history"; 
    ListOfOwnCmds[2] = "history_clear"; 
    ListOfOwnCmds[1] = "cd";  
  
    for (i = 0; i < NoOfOwnCmds; i++) { 
        if (strcmp(parsed[0], ListOfOwnCmds[i]) == 0) { 
            switchOwnArg = i + 1; 
            break; 
        } 
    } 
  
    switch (switchOwnArg) { 
    case 1: 
	return 1;
 
    case 2: 

        return 1;
    case 3: 
        return 1; 
    default: 
        break; 
    } 
  
    return 0; 
} 
  
  
// function for parsing command words 
void parseSpace(char* str, char** parsed) 
{ 
    int i; 
    for (i = 0; i < MAXLIST; i++) { 
        parsed[i] = strsep(&str, " "); 
        if (parsed[i] == NULL) 
            break; 
        if (strlen(parsed[i]) == 0) 
            i--; 
    } 
} 
  
int processString(char* str, char** parsed) 
{   
    char* strpiped[2]; 
    parseSpace(str, parsed); 
    if (ownCmdHandler(parsed)) 
        return 0; 
    else
        return 1; 
} 
  
int main() 
{ 
    char inputString[MAXCOM], *parsedArgs[MAXLIST]; 
    int execFlag = 0; 
    char cmd[MAX_CMD_LEN];
    char *hist[HISTORY_COUNT];
    int i, current,counter = 0;

    for (i = 0; i < HISTORY_COUNT; i++)
            hist[i] = NULL;
    while (1) { 
        // print shell line 
        printf("MTL458");

        // take input 
        if (takeInput(inputString)) 
            continue; 
        // process 
        execFlag = processString(inputString, parsedArgs);
        free(hist[current]);

        hist[current] = strdup(parsedArgs[0]);
        current = (current + 1) % HISTORY_COUNT;
        if (strcmp(parsedArgs[0], "history") == 0)
                history(hist, current);
        else if (strcmp(parsedArgs[0], "history_clear") == 0)

                clear_historyy(hist);
        else if (strcmp(parsedArgs[0], "cd") == 0){
          	if(counter<0){
			counter=0;}
		if(strcmp(parsedArgs[1], "..") == 0){
			counter-=1;}
		else{counter+=1;}
		
	if(counter>=0){
			chdir(parsedArgs[1]);} 
	}
        // execflag returns zero if there is no command 
        // or it is a builtin command, 
        // 1 if it is a simple command 
        // 2 if it is including a pipe. 
  
        // execute 
        if (execFlag == 1) 
            execArgs(parsedArgs); 
    } 
    clear_historyy(hist);

    return 0; 
}
