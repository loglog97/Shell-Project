#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "commandParser.h"
#include "built_in.h"

/**
 * Shell made for CMPE320
 * Tests 1-7: Passed on 2/21/2022
 * Tests 1-15: Passed on 3/27/2022
 * Tests 1-22: Passed on 3/28/2022
 * @author Logan Cole
 */

int main(int argc, char *argv[]){
    char *input = malloc(255*sizeof(char)); //pointer for stdin
    size_t buffer = 255; //buffer to be used when reading a file
    setPath("/bin");
    if(argc <= 1){
        printf("Wish> ");
        while(getline(&input, &buffer, stdin)){
            if(strchr(input, '&') != NULL){
                if(strcmp(input, "&\n")){
                    runParallel(strsep(&input, "\n"));
                }
            }else{
                if(strcmp(input, "\n") != 0){
                    parseLine(input);
                }
            }
            printf("Wish> ");
        }
        exit(0);
    } else{
        if(argc > 2){
            throwError();
            exit(1);
        }
        FILE *fp = fopen(argv[1], "r");
        if(fp != NULL){
            while(getline(&input, &buffer, fp) != EOF){
                    if(input[0] == ' '){
                        char *str = trimWhiteSpace(input);
                        if(strcmp(str, "\0")){
                            parseLine(str);
                        }
                    }else{
                        if(strchr(input, '&') != NULL){
                            if(strcmp(input, "&\n")){
                                runParallel(strsep(&input, "\n"));
                            }
                        }else{
                            if(strcmp(input, "\n") != 0){
                                parseLine(input);
                            }
                        }
                    }
            }
        }else{
            throwError();
            exit(1);
        }
        fclose(fp);
        exit(0);
    }
}