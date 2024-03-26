#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LENGTH 100 /* Assuming each word is no longerr then 100 letters */
#define MAX_WORDS 1000 
#define MAX_LINES 1000 

/* Structure to hold a word and its count */
struct WordInfo {
    char word[MAX_WORD_LENGTH];
    int lines[MAX_LINES];
    int num_lines;
};

/* Function prototypes */
int compare(const void *a, const void *b);
void Error_handel(int argc);
int Read_and_Analyze(FILE *file, char word[], int num_words, struct WordInfo word_info[]);
void Print(int num_words, struct WordInfo word_info[]);
