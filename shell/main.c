//
//  main.c
//  shell
//
//  Created by Grant Miller on 9/8/17.
//  Copyright © 2017 Grant Miller. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "command.h"

#define ARGS_MAX 10
#define ARGLENGTH_MAX 16
#define COMMAND_MAX 160

char * appendNullTerminator(char * input) {
    if (*(input + strlen(input) - 1) == '\n' ) {
        *(input + strlen(input) - 1) = 0;
    } else {
        *(input + strlen(input)) = 0;
    }
    return input;
}

struct Command * parseInput(char * input){
    struct Command * base = malloc(sizeof(base));
    char argsPlaceHolder[ARGS_MAX][ARGLENGTH_MAX];
    char * token;
    int i = 0;
    
    token = strtok(input, " ");
    
    while (token != NULL){
        strcpy(argsPlaceHolder[i], token);
        appendNullTerminator(argsPlaceHolder[i]);
        i++;
        token = strtok(NULL, " ");
    }
    
    base->argc = i;
    base->args=malloc(base->argc * sizeof(char *));

    for( int j = 0; j < base->argc; j++) {
        base->args[j] = malloc((strlen(argsPlaceHolder[j]) + 1) * sizeof(char));
        strcpy(base->args[j],argsPlaceHolder[j]);
        appendNullTerminator(base->args[j]);
        printf("%s\n", base->args[j]);
    }
    return base;
}

int main(int argc, const char * argv[], char** envp) {
    // insert code here...
    char * prompt = "thsh> ";
    char * inputCommand = malloc(COMMAND_MAX);
    size_t commandLength;
    int pid;
    
    while(1){
        write(1, prompt, strlen(prompt));
        commandLength = read(0, inputCommand, COMMAND_MAX);
        inputCommand = appendNullTerminator(inputCommand);
        
        if (strcmp(inputCommand, "exit") == 0) {
            exit(EXIT_SUCCESS);
        }
        
        struct Command * command = parseInput(inputCommand);
        
        pid = fork();
        
        if(pid==0){
            //In child
            int errno = execvp(command->args[0], command->args + 1);
            printf("Error during exec: %d\n",errno);
            exit(0);
        } else {
            waitpid(-1, &pid, 0);
        }
        
        //free resources
        for(int i=0; i < command->argc; i++) {
            free(command->args[i]);
        }
        free(command->args);
        free(command);
        
    }
    return 0;
}


