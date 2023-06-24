#include <stdio.h>
#include <stdlib.h>
#include "parse.h"

const char* SHELL_PROMPT = "$ ";
#define MAX_CMDLINE_LEN 512 // max length of command line


int main() {
    char* cmdLine;
    parseInfo* info;
    while (1) {
        cmdLine = (char*) malloc(sizeof(char) * MAX_CMDLINE_LEN);                      
        printPrompt(SHELL_PROMPT);      
        readCommandLine(cmdLine);
        info = parseCommand(cmdLine);                          
        printInfo(info);
        freeInfo(info);
        free(cmdLine);                                                  
    }
}