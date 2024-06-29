#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../HeaderFiles/driver.h"
#include "../HeaderFiles/error_handle.h"
#include "../HeaderFiles/utils.h"
#include "../HeaderFiles/preprocessor.h"
#include "../HeaderFiles/line.h"
#include "../HeaderFiles/first_second_pass.h"

int main(int argc, char *argv[]) {
	error_handle_args(argc, argv);
	while(--argc > 0){
		struct Line *line_list = NULL;
		printf("\n\nProcessing file: %s\n", argv[argc]); /*promt for user*/
		line_list = process_lines(argv[argc]);
		pre_proccesor(&line_list, argv[argc]);
		first_second_pass(&line_list, argv[argc]);
		error_handle_has_errors(argv[argc]);
		freeLines(line_list);
		printf("\n\nDONE with file %s\n", argv[argc]); /*promt for user*/
	}
	return 1;
}

