//
// Created by marc on 10/5/17.
//

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <zconf.h>
#include "command_parser.h"
#include "constants.h"
#include "variables.h"
#include "environment.h"

const char* exchange_variable(const char* command);
void split_commands(const char* command, char* parsedCommand[], int* argSize);
void split_if_expression(const char* command, char* parsedCommand[], int* argSize);
int get_type(const char* command, char* parsedCommand[], int argSize);

int parse_command (const char* command, char* parsedCommand[], int* argSize) {
    // eliminate new line
    char *newline = strchr(command, '\n');
    if (newline)
        *newline = 0;

    // check for valid length
    if (strlen(command) > 513) return INVALID_LONG_COMMAND;

    // exchange any variable with it's value
    const char* new_command = exchange_variable(command);
    split_commands(new_command, parsedCommand, argSize);
    return get_type(new_command, parsedCommand, *argSize);
}

const char* exchange_variable(const char* command) {
    int double_quotes_counter = 0;
    char* return_command = malloc(sizeof(char)*2048);
    int command_size = 0;

    for (int i = 0; command[i] != '\0'; ++i) {
//        uncomment this line to echo the between quotes like it is.
//        if (command[i] == '\"') double_quotes_counter++;

        if (command[i] == '$' && double_quotes_counter % 2 == 0) { // make sure it's between a closed quotes
            char variable[256];
            int var_size = 0;
            i++; // to advance after the ($)
            while (isalnum(command[i]) != 0 || command[i] == '_') {
                variable[var_size++] = command[i++];
            }

            variable[var_size] = '\0';
            const char* var = lookup_variable(variable);
            if (var != NULL) {
                strcat(return_command, var);
                command_size += strlen(var);
            }

            if (command[i] == '\0') break;
            else return_command[command_size++] = command[i];
        } else {
            return_command[command_size++] = command[i];
        }
    }

    return_command[command_size] = '\0';

    return return_command;
}

void split_commands(const char* command, char* parsedCommand[], int* argSize) {
    char tmpCommand[1024];
    strcpy(tmpCommand, command);

    // work with double quotes for whitespaces.
    int double_quotes = 0;
    const char *p1 = NULL;
    const char *p2 = NULL;
    char *res = NULL;
    if (strchr(tmpCommand, '\"') != NULL) {
        double_quotes = 1;
        p1 = strchr(tmpCommand, '\"') + 1;
        p2 = strchr(p1, '\"');
        size_t len = p2-p1;
        res = (char*)malloc(sizeof(char)*(len+1));
        strncpy(res, p1, len);
        res[len] = '\0';

    } // end of handling whitespaces

    // start parsing of the command.
    const char s[2] = " ";
    char *token;
    /* get the first token */
    token = strtok(tmpCommand, s);
    int index = 0;
    /* walk through other tokens */
    while (token != NULL) {
        while (token[0] == '\t') {
            token++;
        }

        if (double_quotes && token >= p1 && token <= p2) {
            ;
        } else if (double_quotes && token == p1 - 1) {
            parsedCommand[index++] = res;
        } else {
            parsedCommand[index++] = token;
        }

        token = strtok(NULL, s);
    }

    parsedCommand[index] = NULL;
    *argSize = index;

    split_if_expression(command, parsedCommand, argSize);
}

void split_if_expression(const char* command, char* parsedCommand[], int* argSize) {
    if (strchr(command, '=') != NULL) { // is expression
        const char s[2] = "=";
        char* expression;
        if (strstr(command, "export") != command) {
            expression = malloc(strlen(command));
            strcpy(expression, command);
        } else {
            expression = malloc(strlen(command)); // with export
            strcpy(expression, command + 6);
        }

        char *token;

        /* get the first token */
        token = strtok(expression, s);
        parsedCommand[0] = token;
        parsedCommand[1] = "=";
        token = strtok(NULL, s);
        if (token[0] == '\"') {
            parsedCommand[2] = malloc(strlen(token));
            memcpy(parsedCommand[2], token+1, strlen(token)-2);
        } else {
            parsedCommand[2] = token;
        }
        *argSize = 3;
    }
}

int get_type(const char* command, char* parsedCommand[], int argSize) {
    if (argSize == 0) return IS_EMPTY;

    if (parsedCommand[0][0] == '#') {
        return IS_COMMENT;
    } else if (strcmp(parsedCommand[0], "cd") == 0) {
        return IS_CD;
    } else if (strcmp(parsedCommand[0], "echo") == 0) {
        return IS_ECHO;
    } else if (strcmp(parsedCommand[0], "export") == 0) {
        return IS_EXPRESSION;
    } else if ((strstr(command, "=") != NULL)) {
        return IS_EXPRESSION;
    } else if (strlen(command) == 0) {
        return IS_EMPTY;
    }

    return EXECUTABLE;
}