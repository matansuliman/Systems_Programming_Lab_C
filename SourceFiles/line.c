#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../HeaderFiles/line.h"
#include "../HeaderFiles/error_handle.h"
#include "../HeaderFiles/cleanfile.h"
#include "../HeaderFiles/binary_line.h"
#include "../HeaderFiles/symbol.h"
#include "../HeaderFiles/utils.h"


/**
purpose: construct a list of lines of a file

@param file_name
@return list of lines

*/
struct Line* process_lines(char *file_name){
	FILE *input_file;
	struct Line *line_list = NULL;/* this is the list of lines from the file */
	char line_buffer[BUFFER_MAX_LEN]; /* this is a buffer to hold tha line content*/
	int error = 0, number = 0; /* error=0 indicates there is no error, error=1 indicates there is error, number is the line number */
	
	input_file = my_fopen(file_name, READ);
	while (fgets(line_buffer, BUFFER_MAX_LEN, input_file) != NULL) {
		number++;
		error = error_handle_line_size(line_buffer, number);
		addLine(&line_list, line_buffer, number, error);
    	}
	fclose(input_file);
	cleanFile(&line_list);
	proccess_labels(line_list);
	cleanFile(&line_list);
	error_handle_file_empty(line_list, file_name);
	return line_list;
}


/**
purpose: seperate the lables from the context

@param line list
@return void

*/
void proccess_labels(struct Line *line_list){
	struct Line *curr_line = line_list;
	char *temp_buffer, *label_pos, *line_pos;
	int error;
	
	while (curr_line != NULL){
		error = 0;
		if(curr_line->error == 0) {
			temp_buffer = my_strdup(curr_line->content, BUFFER_MAX_LEN);
			label_pos = NULL;
			line_pos = temp_buffer;
			if(strstr(line_pos, ":") != NULL) {/*has label declaration*/
				if(*line_pos == ':') {/*label is empty*/
					error = error_handle_label_expected(curr_line->number);
					set_error(curr_line, error);
				} else {/*label is non empty*/
					label_pos = strtok(temp_buffer, ":");
					error = error_handle_label(label_pos, curr_line->number);
					set_error(curr_line, error);
					line_pos = strtok(NULL, ":");
				}
				free(curr_line->label);			
				free(curr_line->content);
				curr_line->label = my_strdup(label_pos, BUFFER_MAX_LEN);
				curr_line->content = my_strdup(line_pos, BUFFER_MAX_LEN);
			}
			free(temp_buffer);
		}
		curr_line = curr_line->next;
	}
}

/**
purpose: adding a line to the end of the list

@param line list
@param buffer - the content of the line
@param number - the index of the line in the file
@param error - indicates an invalid line
@return void

*/
void addLine(struct Line **line_list, char *line_buffer, int number, int error){
	struct Line *new_line = createLine(line_buffer, number, error);
	struct Line *temp = *line_list;
	if (*line_list == NULL) {
		*line_list = new_line; 
		return;
	}
	while (temp->next != NULL)temp = temp->next;
	temp->next = new_line;
}

/**
purpose: malloc a line

@param buffer - the content of the line
@param number - the index of the line in the file
@param error - indicates an invalid line
@return line

*/
struct Line* createLine(char *line_buffer, int number, int error) {
	struct Line *new_line = (struct Line*)malloc(sizeof(struct Line));
	/*init properties*/
	new_line->number = number;
	new_line->error = error;
	new_line->label = my_malloc(BUFFER_MAX_LEN);
	new_line->content = my_strdup(line_buffer, BUFFER_MAX_LEN);
	new_line->binary_lines = NULL;
	new_line->next = NULL;
	
	return new_line;
}

/**
purpose: print all lines to stdout in the format 'label:content'

@param line list
@return void

*/
void printLines(struct Line *line_list) {
	printf("\nLines of code:");
	printf("\n------------------------------------------------------------------");
	while (line_list != NULL) {
		printf("\nnumber:%-3d | error:%-1d | label:%-10s | content:%s ", line_list->number, line_list->error, line_list->label, line_list->content);
		line_list = line_list->next;
	}
	printf("\n------------------------------------------------------------------");
}

/**
purpose: print all lines to stdout in the form 'label:content \n binarylines'

@param line list
@return void

*/
void printLinesAndBinaryLines(struct Line *line_list) {
	printf("\nLines and Binary lines of code:");
	printf("\n------------------------------------------------------------------");
	while (line_list != NULL) {
		printf("\nnumber:%-3d | error:%-1d | label:%-10s | content:%s ", line_list->number, line_list->error, line_list->label, line_list->content);
		printBinaryLines(line_list->binary_lines);
		printf("\n------------------------------------------------------------------");
		line_list = line_list->next;
	}
}

/**
purpose: print all lines to stdout in the form 'binarylines'

@param line list
@return void

*/
void printOnlyBinaryLines(struct Line *line_list) {
	printf("\nBinary lines of code:");
	while (line_list != NULL) {
		printBinaryLines(line_list->binary_lines);
		line_list = line_list->next;
	}
}

/**
purpose: print all lines to file in the form 'label:content'

@param line list
@param file name
@return void

*/
void fprintLines(struct Line *line_list, char *file_name) {
	FILE *file = my_fopen(file_name, WRITE);
	while (line_list != NULL) {
		if(strlen(line_list->label) != 0) fprintf(file, "%s:%s\n", line_list->label, line_list->content);
		else fprintf(file, "%s\n", line_list->content);
		line_list = line_list->next;
	}
	fclose(file);
}

/**
purpose: number the line list starting from 1

@param line list
@return void

*/
void numberTheLines(struct Line *line_list) {
	int number = 1;
	while (line_list != NULL) {
		line_list->number = number++;
		line_list = line_list->next;
	}
}

/**
purpose: update the error of the line

@param line
@param status - value to update error
@return void

*/
void set_error_1_all(struct Line *line_list) {
	while (line_list != NULL) {
		set_error(line_list, 1);
		line_list = line_list->next;
	}
}

/**
purpose: update the error of the line

@param line
@param status - value to update error
@return void

*/
void set_error(struct Line *line, int status) {
	line->error = status;
	if(status == 1) raiseFlag(999);
}



/**
purpose: free line list from memmory

@param line list
@return void

*/
void freeLines(struct Line *line_list) {
	struct Line *temp;
	while (line_list != NULL) {
		temp = line_list;
		line_list = line_list->next;
		freeLine(temp);
	}
}

/**
purpose: free line from memmory

@param line
@return void

*/
void freeLine(struct Line *line) {
	free(line->label);
	free(line->content);
	freeBinaryLines(line->binary_lines);
	free(line);
}
