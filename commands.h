//
// Created by marc on 10/5/17.
//

#ifndef SHELL_COMMANDS_H
#define SHELL_COMMANDS_H

/**.
 * Change directory to the given one, or print an error if not a valid path
 * @param path the path to change the current directory to
 */
void cd (const char* path);

/**.
 * @param messages the message to proceed through echo command.
 */
void echo (const char* messages[], int argSize);

/**.
 * Reset current and previous directories to their initial state.
 */
void reset_directories();

#endif //SHELL_COMMANDS_H
