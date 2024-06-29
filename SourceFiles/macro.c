#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../HeaderFiles/macro.h"
#include "../HeaderFiles/error_handle.h"
#include "../HeaderFiles/line.h"
#include "../HeaderFiles/utils.h"

/**
purpose: construct a list of macros from a list of lines

@param line list - of the .as file
@return macro list

*/
struct Macro* process_macros(struct Line *line_list){
	struct Macro *macro_list = NULL;
	struct Line *pos = createLine("", -1, 0);
	struct Line *temp = pos;
	pos->next = line_list;
	while (pos->next != NULL) {
		if((pos->next)->error == 0) {
			if(isMCR((pos->next)->content)){
				struct Line *macro_lines = NULL;
				char macro_name[BUFFER_MAX_LEN];
				
				sscanf((pos->next)->content, "mcr %s", macro_name);
				pos->next = (pos->next)->next;
				while(!isENDMCR((pos->next)->content)) {
					addLine(&macro_lines, (pos->next)->content, (pos->next)->number, (pos->next)->error);
					pos->next = (pos->next)->next;
				}
				if (searchMacroByName(macro_list, macro_name) == NULL) addMacro(&macro_list, macro_name, macro_lines);
				else set_error_1_all(line_list);
			}
		}
		pos->next = (pos->next)->next;
	}
	freeLine(temp);
	return macro_list;
}

/**
purpose: extract macros from line_list

@param line list - of the .as file
@return void

*/
void extract_macros(struct Line **line_list){
	struct Line *pos = *line_list;
	struct Line *temp_free;
	bool found = false;
	
	
	while(isMCR((*line_list)->content)){
		temp_free = *line_list;
		*line_list = (*line_list)->next;
		freeLine(temp_free);
		while(!isENDMCR((*line_list)->content)){
			temp_free = *line_list;
			*line_list = (*line_list)->next;
			freeLine(temp_free);
		}
		temp_free = *line_list;
		*line_list = (*line_list)->next;
		freeLine(temp_free);
	}
	
	pos = *line_list;
	while (pos->next != NULL) {
		if(isMCR((pos->next)->content)){
			found = true;
			temp_free = pos->next;
			pos->next = (pos->next)->next;
			freeLine(temp_free);
			while(!isENDMCR((pos->next)->content)) {
				temp_free = pos->next;
				pos->next = (pos->next)->next;
				freeLine(temp_free);
			}
			temp_free = pos->next;
			pos->next = (pos->next)->next;
			freeLine(temp_free);
		}
		if(!found) pos = pos->next;
		found = false;
	}
}

/**
purpose: adding a macro to the end of the list

@param macro list - from the .as file
@param name - macro name
@param line list - the lines of the macro
@return void

*/
void addMacro(struct Macro **head, char *name, struct Line *lines){
	struct Macro *new_Macro = createMacro(name, lines);
	struct Macro *temp = *head;
	if (*head == NULL) {
		*head = new_Macro;
		return;
	}
	while (temp->next != NULL) temp = temp->next;
	temp->next = new_Macro;
}

/**
purpose: malloc a macro

@param name - macro name
@param line list - the lines of the macro
@return macro

*/
struct Macro* createMacro(char *name, struct Line *lines) {
	struct Macro *new_macro = (struct Macro*)malloc(sizeof(struct Macro));
	error_handle_malloc_macro(new_macro);
	new_macro->name = my_strdup(name, BUFFER_MAX_LEN);
	new_macro->lines = lines;
	new_macro->next = NULL;
	return new_macro;
}

/**
purpose: search a macro from a list of macros by his name

@param symbol list
@param name
@return symbol

*/
struct Macro* searchMacroByName(struct Macro *macro_list, char *name) {
	struct Macro *temp = macro_list;
	while(temp != NULL){
		if(strncmp(temp->name, name, strlen(temp->name)) == 0) break;
		temp = temp->next;
	}
	return temp;
}

/**
purpose: print all macros to stdout

@param macro list
@return void

*/
void printMacros(struct Macro *head) {
	printf("\n--- macros ---\n");
	if(head == NULL) printf("\t**No Macro Found**");
	while (head != NULL) {
		printMacro(head);
		head = head->next;
	}
	printf("\n--- end macros ---\n");
}

/**
purpose: print macro to stdout in the form 'macro name: \n lines of macro'

@param macro list
@return void

*/
void printMacro(struct Macro *macro) {
	printf("\nmacro name:%s\n", macro->name);
	printLines(macro->lines);
}

/**
purpose: free macro list from memmory

@param macro list
@return void

*/
void freeMacros(struct Macro *head) {
	struct Macro *temp;
	while (head != NULL) {
		temp = head;
		head = head->next;
		freeMacro(temp);
	}
}

/**
purpose: free macro from memmory

@param macro
@return void

*/
void freeMacro(struct Macro *macro) {
	free(macro->name);
	freeLines(macro->lines);
	free(macro);
}


