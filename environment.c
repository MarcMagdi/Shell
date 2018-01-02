//
// Created by marc on 10/5/17.
//

#include "environment.h"
#include "variables.h"
#include "commands.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* current_director;

void setup_environment(void) {
    // set paths to be more efficient
    set_variable("PATH", getenv("PATH"));
    set_variable("HOME", getenv("HOME"));
    current_director = malloc(sizeof(char) * 4096);
    reset_directories();
    cd(""); // let shell starts from home
}

void set_current_directory(char* directory) {
    strcpy(current_director, directory);
}

const char* get_current_directory() {
    return current_director;
}