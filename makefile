get_name: get_name.o
	gcc -Wall -pedantic -ansi get_name.o -o get_name

get_name.o: get_name.c data.h
	gcc -Wall -pedantic -ansi -c get_name.c -o get_name.o
