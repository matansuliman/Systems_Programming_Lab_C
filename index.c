#include "index.h"

int main(int argc, char* argv[]) {
	FILE *file;
	char word[MAX_WORD_LENGTH];
	struct WordInfo word_info[MAX_WORDS];
	int num_words = 0;
	
	/* Error handeling */
	Error_handel(argc);
	
	file = fopen(argv[1], "r");
	if (file == NULL) {
		printf("Error: Could not open file: %s\n", argv[1]);
		return 1;
	}

	/* Read words from the file and count occurrences */
	num_words = Read_and_Analyze(file, word, num_words, word_info);
	fclose(file);

	/* Sort word_info array in alphabetical order */
	qsort(word_info, num_words, sizeof(struct WordInfo), compare);

	/* Print the results */
	Print(num_words, word_info);
	
	return 0;
}
