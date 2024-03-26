mainmat: mainmat.o mymat.o helpfunctions.o
	gcc -g -Wall -pedantic -ansi mainmat.o mymat.o helpfunctions.o -o mainmat

mainmat.o: mainmat.c mymat.h
	gcc -Wall -pedantic -ansi -c mainmat.c -o mainmat.o

mymat.o: mymat.c mymat.h
	gcc -Wall -pedantic -ansi -c mymat.c -o mymat.o

helpfunctions.o: helpfunctions.c mymat.h
	gcc -Wall -pedantic -ansi -c helpfunctions.c -o helpfunctions.o
