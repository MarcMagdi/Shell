
shell: main.o command_parser.o file_processing.o commands.o variables.o environment.o constants.o execute_handler.o
	gcc main.o command_parser.o commands.o file_processing.o variables.o environment.o constants.o execute_handler.o -o shell

main.o: main.c
	gcc -c main.c

command_parser.o: command_parser.c command_parser.h
	gcc -c command_parser.c

commands.o: commands.c commands.h
	gcc -c commands.c

file_processing.o: file_processing.c file_processing.h
	gcc -c file_processing.c

execute_handler.o: execute_handler.c execute_handler.h
	gcc -c execute_handler.c

variables.o: variables.c variables.h
	gcc -c variables.c

environment.o: environment.c environment.h
	gcc -c environment.c

constants.o: constants.c constants.h
	gcc -c constants.c

clean:
	rm *.o shell
