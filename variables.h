//
// Created by marc on 10/5/17.
//

#ifndef SHELL_VARIABLES_H
#define SHELL_VARIABLES_H

const char* lookup_variable (const char* key);

void set_variable(const char* key , const char* value);

void print_all_variables( void );

#endif //SHELL_VARIABLES_H
