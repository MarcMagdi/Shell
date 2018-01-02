//
// Created by marc on 10/5/17.
//

#ifndef SHELL_COMMAND_PARSER_H
#define SHELL_COMMAND_PARSER_H

/**.
 * parse the given command, split its arguments and set argument's size and return its type
 * @param command to parse
 * @param parsedCommand an array containing the split input command
 * @param argSize the size of the given arguments
 * @return the type of the argument "comment, cd, echo, expression - x=5 -, else
 */
int parse_command (const char* command, char* parsedCommand[], int* argSize);

#endif //SHELL_COMMAND_PARSER_H
