#include "parse.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

#define MAX_CMDLINE_LEN 512 // max length of command line
#define MAX_ARGUMENTS   16  // max number of non null arguments a program can have
#define MAX_TOKEN_LEN   32  // max length of individual tokens

const char* PATH = "/usr/bin/"; // TODO: figure out how to find the $PATH variable 

/* Main command line data structure */
typedef struct parseInfo {
    int numArgs;
    char* rawCmd;
    char* progName;
    char** args;
    bool hasInputRedir;
    bool hasOutputRedir;
    bool isBackgroundProc;
} parseInfo;

/* Initialize a blank parseInfo struct */
parseInfo* initInfo() {
    // Allocate memory for parseInfo object so that it may persist beyond function
    parseInfo* info = malloc(sizeof(parseInfo));

    // When malloc doesn't have any memory to allocate, it will return NULL
    if (info == NULL) {
        return NULL;
    }

    // Initialize with all blank fields
    memset(info, 0, sizeof(parseInfo));
    return info;
}

/* Prints the shell prompt */
void printPrompt(const char* prompt) {
    printf("%s", prompt);
    return;
}

/* Determines if a token is a valid program name */
bool isValidProgName(const char* str) {
    size_t len = strlen(str);
    bool isPipeChar = strcmp(str, "|") == 0;
    bool isLeftAngle = strcmp(str, "<") == 0;
    bool isRightAngle = strcmp(str, ">") == 0;
    bool isAmpersand = strcmp(str, "&") == 0;
    if (isPipeChar || isLeftAngle || isRightAngle || isAmpersand || len == 0) {
        return false;
    } 
    return true;
}

/* Reads a single line from standard input */
void readCommandLine(char* cmdLine) {
    if (fgets(cmdLine, MAX_CMDLINE_LEN, stdin) != NULL) {
        // Remove any trailing newlines
        size_t len = strlen(cmdLine);
        if (len > 0 && cmdLine[len - 1] == '\n') {
            cmdLine[len - 1] = '\0'; // Null terminate the string at the newline position
        }
    }
    return;
}   

/* Utility method for raising errors */
void throwError(const char* errorMsg) {
    perror(errorMsg);
    exit(1);
}

/* Parses command line input and creates parsed data structure */
parseInfo* parseCommand(const char* command) {
    // Initialize buffer
    char buf[MAX_CMDLINE_LEN];
    strcpy(buf, command);
    parseInfo* info = initInfo();

    // NOTE: strtok() is not thread-safe. Consider using strtok_r() instead
    // NOTE: what if the first token is ""? is this even possible?
    char* token = strtok(buf, " ");
    int numArgs = 0;
    info->args = (char**) malloc(sizeof(char*) * MAX_ARGUMENTS);
    // Check for malloc() failure
    if (info->args == NULL) {
        free(info);
        throwError("malloc() failure");
        exit(1);
    }
    while (token) {
        if (numArgs == 0) {
            // First program token *must* be the filename or path to the executable
            if (isValidProgName(token)) {
                // Allocate memory for first program argument
                size_t tokenLen = strlen(token);
                char* progName = (char*) malloc(sizeof(char) * (tokenLen + 1));
                strcpy(progName, token);
                info->progName = progName;
                info->args[numArgs] = progName;
                ++numArgs;
            } else {
                freeInfo(info);
                throwError("Invalid program name");
                exit(1);
            }
        } else if (isValidProgName(token)) {
            size_t tokenLen = strlen(token);
            char* arg = (char*) malloc(sizeof(char) * (tokenLen + 1));
            // Check for malloc() failure
            if (arg == NULL) {
                freeInfo(info);
                throwError("malloc() failure");
                exit(1);
            }
            strcpy(arg, token);
            info->args[numArgs] = arg;
            ++numArgs;
        } else if (strcmp(token, ">") == 0) {
            info->hasOutputRedir = true;
        } else if (strcmp(token, "<") == 0) {
            info->hasInputRedir = true;
        } else if (strcmp(token, "&") == 0) {
            info->isBackgroundProc = true;
        }
        // Get next token
        token = strtok(NULL, " ");
    }
    info->numArgs = numArgs;
    return info;
}   

void printInfo(parseInfo* info) {
    // Print all arguments
    int numArgs = info->numArgs;
    for (int i = 0; i < numArgs; i++) {
        printf("arg%d: %s\n", i + 1, info->args[i]);
    }
    // Print options
    char* inputRedirStr = (info->hasInputRedir) ? "yes" : "no";
    char* outputRedirStr = (info->hasOutputRedir) ? "yes" : "no";
    char* backgroundProcStr = (info->isBackgroundProc) ? "yes" : "no";
    printf("inputRedir: %s\n", inputRedirStr);
    printf("outputRedir: %s\n", outputRedirStr);
    printf("background: %s\n", backgroundProcStr);
    return;
}  

void freeInfo(parseInfo* info) {
    int numArgs = info->numArgs;
    for (int i = 0; i < numArgs; i++) {
        free(info->args[i]);
    }
    free(info->args);
    free(info);
    return;
}