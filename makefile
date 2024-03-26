index: index.o helpfunctions.o
	gcc -g -Wall -pedantic -ansi index.o helpfunctions.o -o index

index.o: index.c index.h
	gcc -Wall -pedantic -ansi -c index.c -o index.o

helpfunctions.o: helpfunctions.c index.h
	gcc -Wall -pedantic -ansi -c helpfunctions.c -o helpfunctions.o
