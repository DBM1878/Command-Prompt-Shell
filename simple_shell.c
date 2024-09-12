/*
project: 01
author: David Middour
email: dmiddou1@umbc.edu
student id: RI67575
description: a simple linux shell designed to perform basic linux commands
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <ctype.h>
#include "utils.h"

/*
In this project, you are going to implement a number of functions to 
create a simple linux shell interface to perform basic linux commands
*/

/* 
DEFINE THE FUNCTION PROTOTYPES
user_prompt_loop()
get_user_command()
parse_command()
execute_command()
*/
void user_prompt_loop();
void get_user_command(char*, size_t);
int parse_command(char*);
void execute_command(char**);
void execute_proc_command(char*);
void write_history(char*);
void retrieve_history();

int main(int argc, char **argv)
{
    /*
    Write the main function that checks the number of argument passed to ensure 
    no command-line arguments are passed; if the number of argument is greater 
    than 1 then exit the shell with a message to stderr and return value of 1
    otherwise call the user_prompt_loop() function to get user input repeatedly 
    until the user enters the "exit" command.
    */

    /*
    ENTER YOUR CODE HERE
    */
    //check number of arguments
    if(argc > 1) {
	//error, too many arguments
	fprintf(stderr, "Too many arguments\n");
	exit(1);
    } else {
	//call user_prompt_loop()
        user_prompt_loop();
    }
}

/*
user_prompt_loop():
Get the user input using a loop until the user exits, prompting the user for a command.
Gets command and sends it to a parser, then compares the first element to the two
different commands ("/proc", and "exit"). If it's none of the commands, 
send it to the execute_command() function. If the user decides to exit, then exit 0 or exit 
with the user given value. 
*/

void user_prompt_loop()
{
    // initialize variables
    size_t bufferSize = 32;
    char* userInput = (char*)malloc(bufferSize * sizeof(char));
    /*
    loop:
        1. prompt the user to type command by printing >>
        2. get the user input using get_user_command() function 
        3. parse the user input using parse_command() function 
        Example: 
            user input: "ls -la"
            parsed output: ["ls", "-la", NULL]
        4. compare the first element of the parsed output to "/proc"and "exit"
        5. if the first element is "/proc" then you have the open the /proc file system 
           to read from it
            i) concat the full command:
                Ex: user input >>/proc /process_id_no/status
                    concated output: /proc/process_id_no/status
            ii) read from the file line by line. you may user fopen() and getline() functions
            iii) display the following information according to the user input from /proc
                a) Get the cpu information if the input is /proc/cpuinfo
                - Cpu Mhz
                - Cache size
                - Cpu cores
                - Address sizes
                b) Get the number of currently running processes from /proc/loadavg
                c) Get how many seconds your box has been up, and how many seconds it has been idle from /proc/uptime
                d) Get the following information from /proc/process_id_no/status
                - the vm size of the virtual memory allocated the vbox 
                - the most memory used vmpeak 
                - the process state
                - the parent pid
                - the number of threads
                - number of voluntary context switches
                - number of involuntary context switches
                e) display the list of environment variables from /proc/process_id_no/environ
                f) display the performance information if the user input is /proc/process_id_no/sched
        6. if the first element is "exit" the use the exit() function to terminate the program
        7. otherwise pass the parsed command to execute_command() function 
        8. free the allocated memory using the free() function
    */

    /*
    Functions you may need: 
        get_user_command(), parse_command(), execute_command(), strcmp(), strcat(), 
        strlen(), strncmp(), fopen(), fclose(), getline(), isdigit(), atoi(), fgetc(), 
        or any other useful functions
    */

    /*
    ENTER YOUR CODE HERE
    */
    do {
	//print user prompt token
	printf("$");
	//get the user input
	get_user_command(userInput, bufferSize);
	//call parse_command
	//parse_command reuturns -1 on exit command
	//otherwise execute command
    } while(parse_command(userInput) != -1);
    //clean memory
    free(userInput);
    //exit
    exit(1);
}

/*
get_user_command():
Take input of arbitrary size from the user and return to the user_prompt_loop()
*/

void get_user_command(char* inputStr, size_t bufferSize)
{
    /*
    Functions you may need: 
        malloc(), realloc(), getline(), fgetc(), or any other similar functions
    */

    /*
    ENTER YOUR CODE HERE
    */
    //get user input and save it into inputStr
    getline(&inputStr, &bufferSize, stdin);
}

/*
parse_command():
Take command grabbed from the user and parse appropriately.
Example: 
    user input: "ls -la"
    parsed output: ["ls", "-la", NULL]
Example: 
    user input: "echo     hello                     world  "
    parsed output: ["echo", "hello", "world", NULL]
*/

int parse_command(char* userInput)
{
    /*
    Functions you may need: 
        malloc(), realloc(), free(), strlen(), first_unquoted_space(), unescape()
    */

    /*
    ENTER YOUR CODE HERE
    */
    //create temp value equal to userInput
    char * tempInput = userInput;
    //get number of tokens(separated by unquoted spaces) in user input
    int bufSize = first_unquoted_space(tempInput);
    int numTokens = 0;
    while(bufSize > 0) {
	numTokens++;
	tempInput += bufSize + 1;
	bufSize = first_unquoted_space(tempInput);
    }
    //reset tempInput and bufSize
    tempInput = userInput;
    bufSize = first_unquoted_space(tempInput);
    //create char pointer array to hold parsed string
    char **tokenArray = malloc((numTokens+1) * sizeof(char*));
    int i = 0;
    int j = 0;

    for(i=0;i < (numTokens+1);i++) {
	tokenArray[i] = NULL;
    }
    i = 0;

    //take user input, parse it and put it into tokenArray
    while(bufSize > 0) {
	tokenArray[j] = (char*)malloc(sizeof(bufSize)+1);
	//incremently copy each character in userInput into tokenArray[i]
	for(i=0; i < bufSize; i++) {
	    tokenArray[j][i] = *tempInput;
	    tempInput++;
	}
	//increment tempInput to skip space
	tempInput++;
	//recalc bufSize
	bufSize = first_unquoted_space(tempInput);
	//increment j to new array index
	j++;
    }

    //print function to print tokens in tokenArray
    /*
    for(i=0; i < numTokens; i++) {
	if(tokenArray[i] != NULL) {
	    printf("%s\n", tokenArray[i]);
	}
	//printf("%s\n", tokenArray[i]);
    }
    */
    //for normal commands, get rid of newline character at end of user input
    /*
    size_t last_char = strlen(tokenArray[numTokens-1]);
    printf("%c\n", tokenArray[numTokens-1][last_char-1]);
    tokenArray[numTokens-1][last_char-1] = '\0';
    */

    //reset bufSize and tmepInput to store user command
    bufSize = first_unquoted_space(userInput);
    tempInput = userInput;

    //write user command to .421sh history file
    write_history(tempInput);

    //check whether command is "exit", "proc", "/proc" or a normal command
    if(strncmp(tokenArray[0], "exit", 3) == 0) {
	//clean memory
	for(i=0;i < numTokens; i++) {
	    free(tokenArray[i]);
	}
	free(tokenArray);
	//return -1 to tell user_prompt_loop to exit
	return -1;
    } else if(strncmp(tokenArray[0], "/proc", 4) == 0 || strncmp(tokenArray[0], "proc", 3) == 0) {
	//for proc commands, get command after proc and get rid of newline character at end of user input
	tempInput += bufSize + 1;
	size_t last_char = strlen(tempInput);
	tempInput[last_char-1] = '\0';
	//call execute_proc_command() to execute proc commands
	execute_proc_command(tempInput);
    } else if(strncmp(tokenArray[0], "history", 6) == 0) {
	retrieve_history();
    } else {
	//execute normal command
	execute_command(tokenArray);
    }
    //clean memory
    for(i=0;i < numTokens; i++) {
	free(tokenArray[i]);
    }
    free(tokenArray);
    return 0;
}

/*
execute_command():
Execute the parsed command if the commands are neither /proc nor exit;
fork a process and execute the parsed command inside the child process
*/

void execute_command(char ** tokenArray)
{
    /*
    Functions you may need: 
        fork(), execvp(), waitpid(), and any other useful function
    */

    /*
    ENTER YOUR CODE HERE
    */
    //set filename to first command
    char* filename = tokenArray[0];
    //set up fork() process
    int status;
    pid_t pid;
    pid = fork();
    if(pid == 0) {
	//execvp the child process, then exit
	execvp(filename, tokenArray);
	exit(0);
    } else if(pid == -1) {
	//something went wrong, error
	printf("error");
    } else {
	//wait til child process finishes
	waitpid(pid, &status, 0);
    }
}

/*
execute_proc_command();
Execute any commands begining with 'proc' or '/proc';
dumping the information in the provided file
*/

void execute_proc_command(char* tempInput)
{
    //initalize path to requested proc file and open file
    size_t bufSize = first_unquoted_space(tempInput);
    char * path = malloc((bufSize+strlen("/proc"))*sizeof(char));
    sprintf(path, "/proc/%s", tempInput);
    FILE* procFile = fopen(path, "r");
    if(!procFile) {
	//failed to open file, error
	printf("Failed to open file or file doesn't exist\n");
    } else {
	//read from file and print until end of file
	char* readStr;
	size_t bufferSize = 256;
	readStr = (char*)malloc(bufferSize * sizeof(char));
	getline(&readStr, &bufferSize, procFile);
	while(!feof(procFile)) {
	    printf(readStr);
	    getline(&readStr, &bufferSize, procFile);
	}
	//clean up memory and close file
	free(path);
	free(readStr);
	fclose(procFile);
    }
}

/*
void write_history();
write user commands into the .421sh history file
*/
void write_history(char* userCommand)
{
    //initialize path and open file
    char* homeDir = getenv("HOME");
    char* path = malloc((strlen(homeDir)+strlen("/.421sh"))*sizeof(char));
    sprintf(path, "%s/.421sh", homeDir);
    FILE* hFile = fopen(path, "a+");
    if(!hFile) {
	//failed to open file, error
	hFile = fopen(path, "w");
    }
    //write command to file, clean up memory and close file
    fprintf(hFile, userCommand);
    free(path);
    fclose(hFile);
}

/*
void retrieve_history();
retireve cammands from the .421sh history file
*/
void retrieve_history()
{
    //initialize path and open file
    char* homeDir = getenv("HOME");
    char* path = malloc((strlen(homeDir)+strlen("/.421sh"))*sizeof(char));
    sprintf(path, "%s/.421sh", homeDir);
    FILE* hFile = fopen(path, "a+");
    if(!hFile) {
        //failed to open file, error
        printf("Failed to open file");
    } else {
	//read from file and count number of lines
	int count = 1;
	char* readStr;
        size_t bufferSize = 256;
        readStr = (char*)malloc(bufferSize * sizeof(char));
	getline(&readStr, &bufferSize, hFile);
	while(!feof(hFile)) {
	    count++;
	    getline(&readStr, &bufferSize, hFile);
	}
	fclose(hFile);

	//create temp file to hold history and copy relevant lines
	//delte old file and then rename the temp file
	hFile = fopen(path, "a+");
	char* path2 = malloc((strlen(homeDir)+strlen("/.421shTemp"))*sizeof(char));
        sprintf(path2, "%s/.421shTemp", homeDir);
	FILE* tempFile = fopen(path2, "w");
	if(!tempFile) {
	    //failed to open file, error
	    printf("Failed to open file");
	} else {
	    char * tempStr = (char*)malloc(bufferSize * sizeof(char));
	    //copy over the last 10 lines
	    while(!feof(hFile)) {
		getline(&tempStr, &bufferSize, hFile);
		if(count <= 10) {
		    fprintf(tempFile, tempStr);
		}
		count--;
	    }
	    //clean up memory and close files
	    free(tempStr);
	    remove(path);
	    rename(path2, path);
	    fclose(tempFile);
	    fclose(hFile);

	    //print out command history
	    hFile = fopen(path, "a+");
	    while(!feof(hFile)) {
                getline(&readStr, &bufferSize, hFile);
	        printf(readStr);
            }
	    fclose(hFile);
	}
	free(readStr);
	free(path2);
    }
    free(path);
}
