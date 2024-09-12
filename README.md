[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-24ddc0f5d75046c5622901739e7c5dd533143b0c8e959d652212380cedb1ea36.svg)](https://classroom.github.com/a/xR-cYv8r)
# project1
Answer these questions please:
Tell me what this project is about?
Tell me how your thought process for completing the project?
Any issues you came across?


This project creates a simple shell mimicing functions found in a shell like bash.
worked on and created several functions:
	main()
	user_prompt_loop()
	get_user_command()
	parse_command()
	execute_command()
	execute_proc_command()
	write_history()
	retrieve_history()

main():
	checks number of arguments, if greater than 1, error
	otherwise call user_propmt_loop()

user_prompt_loop():
	calls a loop to print the prompt chearacter '$', then call get_user_command() to get the user input
	then parses the user input by calling parse_command(). if parse_command() returns -1
	then the 'exit' command was inputed and the shell terminates

get_user_command():
	gets the user command through getline and saves it into a char pointer

parse_command():
	uses first_unquoted_space() to get number of tokens (numTokens) in the user input, 
	then creates a char* array (tokenArray) of size numTokens and initializes it to NULL

	uses first_unquoted_space() to help parse the user input into individual tokens and saves it in the tokenArray
	then calls write_history() to update the .421sh file with the inputed command

	then compares the first command inputed to "exit", "proc" and "/proc", and "history"
	if any of the commands match, execute a specific function, otherwise execute a normal command

	if exit is called, clean memory and return -1, telling the shell to terminate

	otherwise call helper function, then clean memory and return to user_prompt_loop() to reprompt user

execute_command():
	utilizes fork() and execvp() functions to do a system call and execute inputed commands

execute_proc_command():
	creates a path to the user provided path to read from
	then attempts to open the file, if unsuccessful, pritn error
	on successful file open, read from file and print to terminal until end of file

write_history():
	creates a path to the .421sh file in the user home directory and attempts to open file
	if file fails to open, create file
	then take user input and write it to the file
	then clean memory and close the file

retrieve_history():
	creates a path to the .421sh file in the suer home directory and attempts to open the file
	if file opens, create temp file and write recent commands to the temp file
	then remove old .421sh file and rename the temp file to .421sh
	then read from .421sh file and print the comand history
