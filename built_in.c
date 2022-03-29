#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <ctype.h>
#include "built_in.h"
#include "commandParser.h"


char currentPath[20][255];
int pathCount = 0;

/**
 * @brief Called when there is an error somewhere in the command given
 */
void throwError(){
    char error_message[30] = "An error has occurred\n";
    fprintf(stderr, error_message, strlen(error_message));
}

/**
 * @brief Should trim any white space given before the command
 * 
 * @param input the line of input given to the command line
 * @return str - the string with no leading or trailing spaces
 * 
 * @note this is not my code, but I understand what it is doing.
 * aquired from:
 * https://stackoverflow.com/questions/122616/how-do-i-trim-leading-trailing-whitespace-in-a-standard-way
 */
char* trimWhiteSpace(char *str){
    char *end;

  // Trim leading space
  while(isspace((unsigned char)*str)) str++;

  if(*str == 0)  // All spaces?
    return str;

  // Trim trailing space
  end = str + strlen(str) - 1;
  while(end > str && isspace((unsigned char)*end)) end--;

  // Write new null terminator character
  end[1] = '\0';

  return str;
}

/**
 * @brief Tries to search and execute a command
 * 
 * @param command the command to handle
 * @param args the arguments associated with the command
 */
void handleCommand(char *command, char* args){
    char *newDirectory;
    char *trimmedArgs;
    int redirectionFP = 0;

    if(args != NULL && strcmp(args, ">") == 0){ //checks if there is a redirection symbol, but no output file
        throwError(); 
        return;
    }else if(args != NULL && strchr(args, '>') != NULL){
        char *directory = strsep(&args, ">");
        args = trimWhiteSpace(args);
        if(strchr(args, ' ') != NULL){
            throwError();
            return;
        }
        redirectionFP = open(args, O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
        directory = trimWhiteSpace(directory); //should get a NULL pointer if there is no directory

        if(directory != NULL){
            args = directory;
        }
    }

    char *arguments[] = {command, strsep(&args, " "), args, NULL};
    char *path = malloc(255*sizeof(char));

    int i = 0;
    int done = 0; //flag for whether we executed a command

    while(i < pathCount && done == 0 && command != NULL){
        strcpy(path, currentPath[i]);
        strcat(path, "/");
        strcat(path, command);

        if(access(path, F_OK) == 0){
            pid_t pid = fork();
            int status;
            if(pid == 0){
                if(redirectionFP != 0){
                    dup2(redirectionFP, STDOUT_FILENO);
                    close(redirectionFP);
                    execv(path, arguments);
                    printf("execv failiure: %s\n", command);
                }else{
                    execv(path, arguments);
                    printf("execv failiure: %s\n", command);
                }
                exit(0);
            }
            waitpid(pid, &status, 0);
            done = 1;
            free(path);
        }
        i++;
    }

    // This should only run if we have went through all the paths
    // AND we didn't execute a command from those paths
    if(i == pathCount && done == 0){
        throwError();
        free(path);
    }
}

/**
 * @brief Handles changing the directiory
 * 
 * @param path the directory the user wants to go to
 */
void changeDirectory(char *path){
    char *givenPath = strsep(&path, " ");
    //char directory[256];
    if(givenPath != NULL){
        if(chdir(strsep(&givenPath, "\n")) != 0){
            char error_message[30] = "An error has occurred\n";
            fprintf(stderr, error_message, strlen(error_message)); 
            return;
        }
    }
}

/**
 * @brief Set the Path object
 * 
 * @param givenPath the path given from user input
 */
void setPath(char* givenPath){
    if(givenPath != NULL){
        while(givenPath != NULL){
            char *path = strsep(&givenPath, " ");
            strcpy(currentPath[pathCount], path);
            pathCount++;
        }
    }
    pathCount++;
}

/**
 * @brief Called anytime the path command is given
 */
void resetPath(){
    memset(currentPath, 0, sizeof(currentPath[0][0]) * 20 * 255);
    pathCount = 0;
}