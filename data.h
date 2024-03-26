/*include*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

/*define*/
#define MAX_WORDS 30
#define MAX_LETTERS 21 /*includeing the '\0'*/
#define LOOP 10 /*constant from the question*/
#define ERROR_MSG(I,J) \
	printf("\nError, %d and %d are duplicates, please do not enter any name twice\n",I , J)
	/*macro error message*/

/*prototypes*/
void init_array(void);
void get_input(void);
int check_input(void);
char *get_name(void);
int word_count(void);

/*global variable*/
char array[MAX_WORDS][MAX_LETTERS];
