#include "data.h"

/*
This is the main function.
it handels input from user and output.
input: list of names with prerequisites
output: 10 random names
return: 1 or 0 (int)
*/
int main(void){
	int i;
	time_t t;
	srand(time(&t));
	
	/*input*/
	init_array();
	get_input();
	if(check_input()) return 0;
	
	/* output */
	printf("\nThe Output:\n");
	for(i=0; i<LOOP; i++){
		printf("%s\n", get_name());
	}
	return 1;
}

/*
This function initializes the array to null
input: void
output: void
return: void
*/
void init_array(void){
	int i, j;
	for(i=0; i<MAX_WORDS; i++){
		for(j=0; j<MAX_LETTERS; j++)
			array[i][j] = '\0';
	}
}

/*
This function gets the input from the user and insert it to the global array.
each name end in '\0' like a string.
input: void
output: raw input
return: void
*/
void get_input(void){
	int i=0, j=0;/*i iterate names, j iterate letters*/
	char c;/*current char read*/
	/*input msg for user*/
	printf("Please enter 30 names seperated by single white spcace or single newline ");
	printf(", name is max 20 letters:\n\n");
	while((c = getchar()) != EOF){
		printf("%c", c); /*print raw input*/
		if(c == ' ' || c == '\n'){/*end of name*/
			array[i++][j] = '\0';
			j=0;
		} else { array[i][j++] = c; }
	}
}

/*
This function checks the input.
if there are duplicate names then it prints error msg and returns 1 for faliure.
in order to be non case sensetive i used tolower() function.
input: void
output: appropriate error msg if need
return: 1 for faliure, 0 for success (int)
*/
int check_input(void){
	int i, j, k; /*i and j iteraring couples, k iterating letters*/
	int wc = word_count();
	for(i=0; i<wc; i++){
		for(j=i+1; j<wc; j++){
			for(k=0; k<MAX_LETTERS; k++){
				if(tolower(array[i][k]) != tolower(array[j][k])) break;
				/*ended the name without break then they are equal*/
				if(array[i][k] == '\0'){ 
					ERROR_MSG(i+1, j+1);
					return 1;
				}
			}
		}
	}
	return 0;
}

/*
This function picks a random name from the input and returns a copy of it.
in order to be inside array bounds i used mod of word_count.
input: void
output: void
return: chosen name (char array)
*/
char *get_name(void){
	int pick = rand() % word_count(); /*mod*/
	static char des[MAX_LETTERS];
	strncpy(des, array[pick], MAX_LETTERS);
	return des;
}

/*
This function returns the number of words in array
if there is a word then the first position diffrent from '\0'
otherwise there is no word
input: void
output: void
return: words in array (int)
*/
int word_count(void){
	int i;
	for(i=0; i<MAX_WORDS; i++)
		if(array[i][0] == '\0') break;
	return i;
}







