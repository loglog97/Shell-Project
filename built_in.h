#ifndef BUILT_IN_H
#define BUILT_IN_H

void setPath(char*);
void changeDirectory(char*);
void printPath(char*);
void handleCommand(char*, char*);
void throwError();
void resetPath();
char* trimWhiteSpace(char*);

#endif