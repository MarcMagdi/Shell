//
// Created by marc on 10/11/17.
//

#include <stdio.h>
#include <string.h>
#include "file_processing.h"
#include "environment.h"
#include "constants.h"

void append_to_file(char* txt, char* path) {
    FILE* filePtr = fopen(path, "ab+");
    fprintf(filePtr, txt);
    fclose(filePtr);
}

void append_to_history(char* txt) {
    append_to_file(txt, HISTORY_FILE);
}

void append_to_log(char* txt) {
    append_to_file(txt, LOG_FILE);
}

void print_history() {
    FILE* filePtr = fopen(HISTORY_FILE, "ab+");
    if (filePtr != NULL) {
        char inst[COMMAND_MAX_SIZE + 1];
        while (feof(filePtr) == 0) {
            fgets(inst, COMMAND_MAX_SIZE+1, filePtr);
            fputs(inst, stdout);
        }

        fclose(filePtr);
    }
}

FILE *commandsFilePtr;

/*
	CommandsBatch file section
*/
void open_commands_batch_file(char* path) {
    commandsFilePtr = fopen(path, "r");
}

FILE* get_commands_batch_file() {
    return commandsFilePtr;
}

void close_commands_batch_file() {
    if (commandsFilePtr != NULL)
        fclose(commandsFilePtr);
}