#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <signal.h>
#include <stdlib.h>

void child_proc(char* args[]);

int main(int ac, char* args[])
{
	//input variable
	char input[100];
	
	//ignore ctrl-c
	signal(SIGINT, SIG_IGN);
	
	//main while loop, breaks if user inputs "exit"
	while(1){
		printf("$ ");
		fgets(input,100, stdin);
		input[strlen(input)-1] = '\0';
		
		if(strcmp(input, "exit") == 0){
			break;
		}

		int i = 0;
		
		//split command line into tokens and into array args
		args[i] = strtok(input, " ");
		while((args[++i] = strtok(NULL, " ")));
		
		int cd_result;
		
		//if user inputs "cd" use chdir
		if(args[0] == NULL){}
		else if(strcmp(args[0], "cd") == 0){
			cd_result = chdir(args[1]);
			if(cd_result == -1){
				perror("Error");
			}
		}
		
		//call fork to create child process
		pid_t result = fork();
		
		//if successful, call function child_proc to call execvp to execute user command
		//else wait for parent
		if(result == 0){
			child_proc(args);
			_exit(1);
		}
		else{
			wait(NULL);
		}
	}	
	return 0;
}

//function called in main to execute proper command and arguments using execvp
void child_proc(char* args[]){
	if(strcmp(args[0], "cd") != 0){
		if(execvp(args[0], args) == -1){
			perror("Error");
		}
	}
}
