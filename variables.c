//
// Created by marc on 10/5/17.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "variables.h"

char* table[100][2]; // first cell key, second cell value
int current = 0;
int exist(const char* key);

const char* lookup_variable (const char* key) {
    int id = exist(key);
    if (id != -1) return table[id][1];

    return getenv(key);
}

void set_variable (const char* key , const char* value) {
    int id = exist(key);
    if (id != -1) {
        strcpy(table[id][1], value);
    } else if (current < 100) {
        table[current][0] = malloc(strlen(key));
        table[current][1] = malloc(256);
        strcpy(table[current][0], key);
        strcpy(table[current][1], value);
        current++;
    }
}

int exist(const char* key) {
    for (int i = 0; i < current; ++i) {
        if (strcmp(table[i][0], key) == 0) { // equal
            return i;
        }
    }

    return -1;
}

void print_all_variables (void) {
    for (int i = 0; i < current; ++i) {
        printf("Key:%s Value:%s\n", table[i][0], table[i][1]);
    }
}