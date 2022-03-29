#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include "commandParser.h"
#include "built_in.h"

/**
 * @brief Handles running commands in parallel
 * 
 * @param input the whole line of commands
 */
void runParallel(char *input){
    char commands[10][255];
    int cmdCount = 0;
    while(strchr(input, '&') != NULL){
        char *cmd = strsep(&input, "&");
        cmd = trimWhiteSpace(cmd);
        strcpy(commands[cmdCount], cmd);
        cmdCount++;
    }

    input = trimWhiteSpace(input);
    if(strcmp(input, "\0") != 0){
        strcpy(commands[cmdCount], input); //this should only run if input is left with a command
        cmdCount++; 
    }

    /* runs the individual commands in parallel */
    for(int i = 0; i < cmdCount; i++){
        if(fork() == 0){
            parseLine(commands[i]);
            exit(0);
        }
    }
    /* waits for childs to finish */
    for(int i = 0; i < cmdCount; i++){
        wait(NULL);
    }
}

/**
 * @brief Parses the command and attempts to run it
 * 
 * @param input the command to parse and run
 */
void parseLine(char *input){
    if(input != NULL){
        char *cmd = strsep(&input, " ");
        if(cmd != NULL){
            if(strcmp("cd", cmd) == 0){
                changeDirectory(input);
            }else if(strcmp(cmd, "path") == 0 || strcmp(cmd, "path\n") == 0){
                resetPath();
                setPath(strsep(&input, "\n"));
            }else if(strcmp(cmd, "exit\n") == 0){
                exit(0);
            }else{
                handleCommand(strsep(&cmd, "\n"), strsep(&input, "\n"));
            }
        }  
    }
}