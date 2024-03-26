#include "index.h"

/* Comparison function for qsort */
int compare(const void *a, const void *b) {
	const struct WordInfo *wordA = (const struct WordInfo *)a;
	const struct WordInfo *wordB = (const struct WordInfo *)b;
	return strcmp(wordA->word, wordB->word);
}

/* hadel error with args */
void Error_handel(int argc) {
	if(argc < 2){
		printf("Error: Specifiy file name in command\n");
		exit(1);
	}

	if(argc > 2){
		printf("Error: Specifiy only 1 file name in command\n");
		exit(1);
	}
}

/* Read words from the file and count occurrences */
int Read_and_Analyze(FILE *file, char word[], int num_words, struct WordInfo word_info[]) {
	int found, line_number = 1, i;
	while (fscanf(file, "%s", word) != EOF) {

		/* Check if the word already exists in word_counts */
		found = 0;
		for (i = 0; i < num_words; i++) {
			if (strcmp(word_info[i].word, word) == 0) {
				word_info[i].lines[word_info[i].num_lines] = line_number;
                		word_info[i].num_lines++;
				found = 1;
				break;
			}
		}

		/* If the word is not found, add it to word_counts */
		if (!found) {
			strcpy(word_info[num_words].word, word);
			word_info[num_words].lines[0] = line_number;
			word_info[num_words].num_lines = 1;
			num_words++;
		}
		
		/* Check for end of line */
		if (fgetc(file) == '\n') {
			line_number++;
		}
	
		/* Move back one character to prevent skipping the first character of the next word */
		fseek(file, -1, SEEK_CUR);		
	}
	return num_words;
}

/* Print the results */
void Print(int num_words, struct WordInfo word_info[]) {
	int i, j;
	for (i = 0; i < num_words; i++) {
		if(word_info[i].num_lines == 1) printf("%-20s appares in line ", word_info[i].word);
		else printf("%-20s appares in lines ", word_info[i].word);	
		for (j = 0; j < word_info[i].num_lines; j++) {
			if(j == word_info[i].num_lines - 1) printf("%d", word_info[i].lines[j]);
			else printf("%d,", word_info[i].lines[j]);
		}
		printf("\n");
	}
}

