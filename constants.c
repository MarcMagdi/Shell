//
// Created by marc on 10/4/17.
//

const int COMMAND_MAX_SIZE = 514; // +1 for new line character
const int IS_COMMENT = 1;
const int IS_CD = 2;
const int IS_ECHO = 3;
const int IS_EXPRESSION = 4;
const int EXECUTABLE = 5;
const int IS_EMPTY = 6;
const int INVALID_LONG_COMMAND = 7;
const char* HISTORY_FILE = "/tmp/shell.history";
const char* LOG_FILE = "/tmp/shell.log";