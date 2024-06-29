#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../HeaderFiles/preprocessor.h"
#include "../HeaderFiles/utils.h"


/**
purpose: create the .am file with spreaded macros from .as file
there are three stages:
1) construct macro list
2) extract macros from line_list
3) spread the macros in line_list
4) make the .am file

@param line list - of the .as file
@param file name
@return void

*/
void pre_proccesor(struct Line **line_list, char *file_name){
	struct Macro *macro_list = NULL;
	char *file_name_am = change_extension(file_name, EXTENTION_AM);
	
	macro_list = process_macros(*line_list);
	extract_macros(line_list);
	spread_macros(*line_list, macro_list);
	fprintLines(*line_list, file_name_am);
	
	free(file_name_am);
	freeMacros(macro_list);
}

/**
purpose: spread the macros inside line_list

@param line list - of the .as file
@param macro list
@return void

*/
void spread_macros(struct Line *line_list, struct Macro *macro_list){
	struct Line *curr_line = line_list;
	struct Macro *curr_macro = macro_list;
	bool not_found_macro = true;
	if(line_list != NULL && line_list->next != NULL){
		while (curr_line->next != NULL) {
			while (curr_macro != NULL) {
				if (not_found_macro && (strncmp((curr_line->next)->content, curr_macro->name, strlen(curr_macro->name)) == 0)){
					struct Line *lines_to_add = NULL;
					struct Line *curr_macro_lines = curr_macro->lines;
					struct Line *line_after_decleration = (curr_line->next)->next;
					struct Line *temp_free = curr_line->next;
				
					while(curr_macro_lines != NULL) {
						addLine(&lines_to_add, curr_macro_lines->content, -1, curr_macro_lines->error);
						curr_macro_lines = curr_macro_lines->next;
					}
				
					if(lines_to_add != NULL) {
						free(lines_to_add->label);
						lines_to_add->label = my_strdup(temp_free->label, BUFFER_MAX_LEN);
						curr_line->next = lines_to_add;
						while(lines_to_add->next != NULL) lines_to_add = lines_to_add->next;
						lines_to_add->next = line_after_decleration;
					} 
					else curr_line->next = line_after_decleration;
				
					freeLine(temp_free);
				}
				if(curr_line->next == NULL) break; 
				curr_macro = curr_macro->next;
			}
			if(not_found_macro) curr_line = curr_line->next;
			not_found_macro = true;
			curr_macro = macro_list;
		}
	}
	numberTheLines(line_list);
}




