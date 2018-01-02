//
// Created by marc on 10/11/17.
//

#ifndef SHELL_FILE_PROCESSING_H
#define SHELL_FILE_PROCESSING_H


void append_to_history(char* txt);
void print_history();

/**.
 * Append the given txt to the log file.
 * @param txt the text to be appended to the log file.
 */
void append_to_log(char* txt);

void open_commands_batch_file(char* path);
FILE* get_commands_batch_file();
void close_commands_batch_file();

#endif //SHELL_FILE_PROCESSING_H
