//
// Created by marc on 10/7/17.
//

#include <signal.h>
#include <zconf.h>
#include <stdio.h>
#include <string.h>
#include <wait.h>
#include <stdlib.h>
#include "variables.h"
#include "environment.h"
#include "file_processing.h"

char* expand_argument(char* argument) {
    if (strchr(argument, '~')) {
        char tmpArg[256];
        strcpy(tmpArg, argument);
        argument = malloc(sizeof(char) * 4096);
        int argSize = 0;
        for (int i = 0; tmpArg[i] != '\0'; ++i) {
            if (tmpArg[i] == '~') {
                strcat(argument, lookup_variable("HOME"));
                argSize += strlen(lookup_variable("HOME"));
            } else {
                argument[argSize++] = tmpArg[i];
            }
        }

        argument[argSize] = '\0';
    }
    return argument;
}

void expanded_arguments(char* arguments[], int argSize) {
    for (int i = 0; i < argSize; ++i) {
        arguments[i] = expand_argument(arguments[i]);
    }
}

void execute_parent(char* arguments[], int argSize, int pid) {
    int returnStatus;
    if (arguments[0][strlen(arguments[0]) - 1] == '&' || strcmp(arguments[argSize-1], "&") == 0);
    else {
        do {
            waitpid(pid, &returnStatus, WUNTRACED);
        } while(!WIFEXITED(returnStatus) && !WIFSIGNALED(returnStatus));
    }
}

void execute(char* arguments[], int argSize) {
    pid_t pid = fork();
    expanded_arguments(arguments, argSize);

    if (pid == 0) { // child process
        int found = 0;

        char inst[513];
        strcpy(inst, arguments[0]);

        // remove & the indicator of running in the background.
        if (inst[strlen(inst) - 1] == '&') { // remove the & from inst
            arguments[0][strlen(inst) - 1] = '\0';
            inst[strlen(inst) - 1] = '\0';
        } else if (strcmp(arguments[argSize-1], "&") == 0) {
            arguments[argSize-1] = NULL;
        }

        // no relative path
        if (arguments[0][0] == '/') {
            if (access(inst, F_OK) != -1) {
                // file exists
                execv(inst, arguments);
                found = 1;
            }
        } else {
            char command[512];
            memset(command, 0, sizeof command); // reset the command
            const char s[2] = ":";
            char *token;
            const char *paths = lookup_variable("PATH");
            char *tmpPaths = malloc(strlen(paths));
            strcpy(tmpPaths, paths);
            token = strtok(tmpPaths, s);

            while (token != NULL) {
                memset(command, 0, sizeof command); // reset the command
                strcat(command, token);
                strcat(command, "/");
                strcat(command, arguments[0]);
                if (access(command, F_OK) != -1) {
                    // file exists
                    execv(command, arguments);
                    found = 1;
                    break;
                }

                token = strtok(NULL, s);
            }

            if (strcmp(arguments[0], "history") == 0) {
                found = 1;
                print_history();
            }
        }

        if (found == 0) {
            // file doesn't exist
            printf("No command %s found.\n", arguments[0]);
        }

        _exit(0);
    } else if (pid > 0) { // parent process
        execute_parent(arguments, argSize, pid);
    } else { // error
        perror("fork failed");
        _exit(3);
    }
}