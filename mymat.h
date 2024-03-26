/*include*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>



/*define*/
#define ROWS 4
#define COLS 4
#define MAX_LINE 10000
#define IS_VALID(E) (isalpha(E) || ispunct(E) || isdigit(E) || E == '.') && (E != ',')
#define IS_BLANK(P) P == '\t' || P == ' '



/*structers*/
typedef struct mat{
	float arr[ROWS][COLS];
} mat;



/*prototypes*/

/*
purpose: init array to null before getting new line
input: pointer to start of line
output: void
*/
void init_line(char*);
/*
purpose: get next command from user while printing raw input
input: pointer to start of line
output: 0 for error, else 1
*/
int get_line(char*);
/*
purpose: getting next word from the input
input: pointer to some position on the input line
output: pointer to start of word
*/
char *get_word(char*);
/*
purpose: getting next word length from the input
input: pointer to some position on the input line
output: length of the next word
*/
int get_word_len(char*);
/*
purpose: skiping spaces from the input
input: pointer to some position on the input line
output: pointer to position in input after the spaces 
*/
char *skip_spaces(char*);
/*
purpose: identifing matrix
input: pointer to start of word and length of word
output: 0 for error, 1-6 for matrcies A-F
*/
int matrix_nametonum(char*, int);
/*
purpose: when wanting to detect a comma and print error msg
input: pointer to some position on the input line
output: 1 for error found, else 0
*/
int illegal_comma(char*);
/*
purpose: when wanting to detect a missing comma and print error msg
input: pointer to some position on the input line
output: 1 for error found, else 0
*/
int missing_comma(char*);
/*
purpose: print error msg
input: void
output: always 0, from convenient purposes
*/
int missing_argument(void);
/*
purpose: print error msg
input: pointer to start of word and length of word
output: always 0, from convenient purposes
*/
int undefined_matrix(char*, int);
/*
purpose: detect if a string is a valid number
input: pointer to start of word and length of word
output: 0 for error, else 1
*/
double is_number(char*, int);
/*
purpose: detect extraneous text after a string and prints error msg
input: pointer to end position of command on the input line
output: 0 for error, else 1
*/
int extraneous_text(char*);




/*
purpose: checking if name of given word from position is equal to command name given
input: pointer to start of word and length of word, and a cmd string
output: 1 for equal, else 0
*/
int cmd_check_name(char*, const char*, int);
/*
purpose: check valid stop cmd and execute it
input: pointer to start of word and length of word
output: 1 for error
*/
int cmd_stop(char*, int);
/*
purpose: check valid read mat cmd and execute it
input: pointer to start of word and length of word, and mat array
output: 1 for error, else 0
*/
int cmd_read_mat(char*, int, mat*[]);
/*
purpose: check valid print cmd and execute it
input: pointer to start of word and length of word, and mat array
output: 1 for error, else 0
*/
int cmd_print_mat(char*, int, mat*[]);
/*
purpose: check valid add/sub/mul cmd and execute it
input: pointer to start of word and length of word, and mat array, and function to applay
output: 1 for error, else 0
*/
int cmd_AddMulSub_mat(char*, int, mat*[], void(*f)(mat *, mat *, mat *));
/*
purpose: check valid mat scalar mul cmd and execute it
input: pointer to start of word and length of word, and mat array
output: 1 for error, else 0
*/
int cmd_scalar_mat(char*, int, mat*[]);
/*
purpose: check valid trans mat cmd and execute it
input: pointer to start of word and length of word, and mat array
output: 1 for error, else 0
*/
int cmd_trans_mat(char*, int, mat*[]);
/*
purpose: copy source mat values to destenation mat
input: 2 mats
output: void
*/
void copy_mat(mat*, mat*);
/*
purpose: a+b=c when a,b,c are mats
input: 3 mats
output: void
*/
void add_mat(mat*, mat*, mat*);
/*
purpose: a-b=c when a,b,c are mats
input: 3 mats
output: void
*/
void sub_mat(mat*, mat*, mat*);
/*
purpose: a*b=c when a,b,c are mats
input: 3 mats
output: void
*/
void mul_mat(mat*, mat*, mat*);
/*
purpose: a*(number)=c when a,c are mats
input: 2 mats and 1 number
output: void
*/
void mul_scalar(mat*, float, mat*);
/*
purpose: transpose source mat and store in destanation mat
input: 2 mats
output: void
*/
void trans_mat(mat*, mat*);
/*
purpose: init mat values to 0
input: mat
output: void
*/
void init_mat_zero(mat*);
/*
purpose: print mat values to user in a structure
input: mat
output: void
*/
void print_mat(mat*);



