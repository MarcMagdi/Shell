#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>

#include "constants.h"
#include "environment.h"
#include "command_parser.h"
#include "commands.h"
#include "execute_handler.h"
#include "variables.h"
#include "file_processing.h"

typedef enum{ false = 0 , true = 1 } bool ;

void start(bool read_from_file);
void shell_loop(bool input_from_file);
static void handler(int sig);
void evaluate_expression(char* arguments[]);
void reset(char instruction[], char* arguments[], int* argSize);
void clear_input_buffer();

/**.
 * The entry point for the shell
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char *argv[])
{
    signal(SIGCHLD, handler);
    setup_environment();

    if (argc > 1) {
        open_commands_batch_file(argv[1]);
        start(true);
    } else {
        start(false);
    }

    return 0;
}

void start(bool read_from_file) {
    if(read_from_file) {
        shell_loop(true);
    } else {
        shell_loop(false);
    }
}

void shell_loop(bool input_from_file) {
    char instruction[COMMAND_MAX_SIZE + 1];
    char* arguments[25];
    int* argSize = malloc(sizeof(int));

    // check for command file
    FILE* filePtr = get_commands_batch_file();
    if (input_from_file && filePtr == NULL) {
        printf("File doesn't exist\n");
        input_from_file = false;
    }

    while (true) {
        printf("Shell>");
        // reset parameters to fetch a new command.
        reset(instruction, arguments, argSize);

        if (input_from_file) {
            //read next instruction from file
            fgets(instruction, COMMAND_MAX_SIZE + 1, filePtr);
            printf("%s", instruction); // only print with batch mode
            if (feof(filePtr)) {
                printf("EOF");
                close_commands_batch_file();
                break;
            }
        } else {
            // read next instruction from console
            fgets(instruction, COMMAND_MAX_SIZE + 1, stdin);
            if (feof(stdin)) {
                break;
            }
        }

        append_to_history(instruction);

        // parse the commands and set the arguments and argSize
        int type = parse_command(instruction, arguments, argSize);
        char* command = arguments[0];

        // command execution
        if (type == IS_EMPTY) continue;
        else if (type == INVALID_LONG_COMMAND) {
            printf("Command is too long. Max command length is 512 characters\n");
            clear_input_buffer();
        } else if (strcmp(command, "exit") == 0) break;
        else if (type == IS_COMMENT) {
            ;
        } else if (type == IS_CD) {
            cd(arguments[1]);
        }  else if (type == IS_EXPRESSION) { // assign value
            evaluate_expression(arguments);
        } else if (type == IS_ECHO) {
            echo(arguments, *argSize);
        } else if (type == EXECUTABLE) {
            execute(arguments, *argSize);
        }
    }
}

/**.
 * Used to clear input buffer when command is too long.
 */
void clear_input_buffer() {
    char tmp[256];
    while (!strchr(tmp, '\n'))
        if (!fgets(tmp,(sizeof tmp), stdin))
            break;
}

/**.
 * Used to reset all variables to fetch a new instruction
 * @param instruction
 * @param arguments
 * @param argSize
 */
void reset(char instruction[], char* arguments[], int* argSize) {
    memset(instruction, 0, sizeof instruction);
    memset(arguments, 0, sizeof arguments);
    *argSize = 0;
}

/**.
 * Used to variable assignment.
 * @param arguments
 */
void evaluate_expression(char* arguments[]) {
    set_variable(arguments[0], arguments[2]);
}

static void handler(int sig) {
    pid_t pid;
    pid = wait(NULL);
    append_to_log("Child process was terminated\n");
}
