//
// Created by marc on 10/5/17.
//

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <zconf.h>
#include <dirent.h>
#include "commands.h"
#include "variables.h"
#include "environment.h"

char* currentDir;
char* prevDirectory;

int swapPrev(const char* path);

void reset_directories() {
    currentDir = malloc(sizeof(char) * 4096);
    prevDirectory = malloc(sizeof(char) * 4096);

    currentDir[0] = '#';
    prevDirectory[0] = '#';
}

void cd (const char* path) {
    if (strlen(path) == 1 && path[0] == '.') return;

    if (swapPrev(path)) {
        if (strlen(prevDirectory) == 1 && prevDirectory[0] == '#') {
            printf("bash: cd: OLDPWD not set\n");
        } else {
            char* tmp = currentDir;
            currentDir = prevDirectory;
            prevDirectory = tmp;
            chdir(currentDir);
            printf("%s\n", currentDir);
        }

        return;
    }

    int path_changed = 0;
    if (strlen(path) == 0 || (strlen(path) == 1 && path[0] == '~')) { // go home
        path_changed = chdir(lookup_variable("HOME"));
    } else if (strlen(path) > 1 && path[0] == '~') { // go to user folder
        char testDir[2048];
        strcpy(testDir, lookup_variable("HOME"));
        if (path[1] != '/') { // check if it's a user
            strcat(testDir, "/../"); // the home directory is the current user directory
        }

        strcat(testDir, &path[1]);
        path_changed = chdir(testDir);
    } else {
        path_changed = chdir(path);
    }

    if (path_changed != 0) {
        printf("bash: cd: %s: No such file or directory\n", path);
    } else {
        strcpy(prevDirectory, currentDir);
        getcwd(currentDir, 4096);
        set_current_directory(currentDir);
        if (prevDirectory[0] != '#') printf("%s\n", currentDir);
    }
}

int swapPrev(const char* path) {
    return strlen(path) == 1 && path[0] == '-';
}

void echo (const char* messages[], int argSize) {
    for (int i = 1; i < argSize; ++i) {
        printf("%s ", messages[i]);
    }

    printf("\n");
}