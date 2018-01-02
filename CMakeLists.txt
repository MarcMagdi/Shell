cmake_minimum_required(VERSION 3.8)
project(shell)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c constants.c constants.h environment.h environment.c variables.h variables.c commands.h commands.c command_parser.h command_parser.c execute_handler.h execute_handler.c file_processing.h file_processing.c)
add_executable(shell ${SOURCE_FILES})