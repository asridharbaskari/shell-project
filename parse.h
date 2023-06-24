#ifndef PARSE_H
#define PARSE_H
#include <stdbool.h>

typedef struct parseInfo parseInfo;
parseInfo* initInfo();
void printPrompt(const char* prompt);
bool isValidProgName(const char* str);                                  // determines if a string is a valid program name
void readCommandLine(char* cmdLine);                                 // reads raw shell input into a string
parseInfo* parseCommand(const char* command);                               // parses raw shell input from a string
// parseInfo* parse(char* cmdLine);
void throwError(const char* errorMsg);
void printInfo(parseInfo* info);                    
void freeInfo(parseInfo* info);




#endif // PARSE_H