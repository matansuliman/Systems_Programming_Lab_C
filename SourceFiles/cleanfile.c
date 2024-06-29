#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "../HeaderFiles/cleanfile.h"
#include "../HeaderFiles/utils.h"

/*
this function gets a ll and a file name
it trims the input from unnececery data
such as empty/comment lines, multiple spaces
*/
void cleanFile(struct Line **line_list){
	if(line_list != NULL) {
		cleanByType(line_list, isEmptyLine);
		cleanByType(line_list, isCommentLine);
		
		numberTheLines(*line_list);
		cleanBeginings(*line_list);
		cleanEnds(*line_list);
	
		convertTabToSpace(*line_list);
		convertManySpacesToOne(*line_list);
	}
	
}

/*
this function gets a ll and a function
it removes the lines that satisfy the function given
*/
void cleanByType(struct Line **line_list, bool (*func)(struct Line *line)){
	struct Line *curr_line = NULL;
	struct Line *temp_free = NULL;
	bool found = false;
	
	/* for the first */
	while(*line_list != NULL){ 
		if((*func)(*line_list)){
			temp_free = *line_list;
			*line_list = (*line_list)->next;
			freeLine(temp_free);
		}
		else break;
	}
	
	/* for the rest */
	curr_line = *line_list;
	while (curr_line->next != NULL) {
		
		if((*func)(curr_line->next)){
			found = true;
			temp_free = curr_line->next;
			curr_line->next = (curr_line->next)->next;
			freeLine(temp_free);
		}
		if(!found) curr_line = curr_line->next;
		found = false;
	}
}

/*this function returns true if a line is empty of content*/
bool isEmptyLine(struct Line *line){
	bool empty_flag = true;
	int i;
	for(i = 0; i < strlen(line->content); i++) if(asciiCharacter(line->content[i])) empty_flag = false;
	return empty_flag;
}

/*this function returns true if a line is a comment line*/
bool isCommentLine(struct Line *line){
	char *pos = skip_spaces(line->content);
	if (*pos == ';') return true; /*comment line begins with a ;*/
	return false;
}

/*this function cleans the begining of every line from spaces*/
void cleanBeginings(struct Line *line_list){
	struct Line *curr_line = line_list;
	char *temp;
	int i;
	while (curr_line != NULL) {
		i = 0;
		while(isspace(curr_line->content[i])) i++;
		temp = my_strdup(&curr_line->content[i], BUFFER_MAX_LEN);
		free(curr_line->content);
		curr_line->content = my_strdup(temp, BUFFER_MAX_LEN);
		free(temp);
		
		if(strlen(curr_line->label) != 0){
			i = 0;
			while(isspace(curr_line->label[i])) i++;
			temp = my_strdup(&curr_line->label[i], BUFFER_MAX_LEN);
			free(curr_line->label);
			curr_line->label = my_strdup(temp, BUFFER_MAX_LEN);
			free(temp);
		}
		
		curr_line = curr_line->next;
	}
}

/*this function cleans the end of every line from spaces*/
void cleanEnds(struct Line *line_list){
	struct Line *curr_line = line_list;
	int i;
	
	while (curr_line != NULL) {
		
		/*clean end of content*/
		i = strlen(curr_line->content) - 1;
		while(i >= 0 && !asciiCharacter(curr_line->content[i])) curr_line->content[i--] = '\0';
		
		/*clean end of label*/
		i = strlen(curr_line->label) - 1;
		while(i >= 0 && !asciiCharacter(curr_line->label[i])) curr_line->content[i--] = '\0';
		
		curr_line = curr_line->next;
	}
}





/*this function gets a ll and foreach line replacing each tab for a space*/
void convertTabToSpace(struct Line *line_list){
	int i;
	while (line_list != NULL) {
		for(i = 0; i < strlen(line_list->content); i++){
			if (line_list->content[i] == '\t') line_list->content[i] = ' ';
		}
		line_list = line_list->next;
	}
}

/*this function gets a ll and foreach line removes consecutive spaces*/
void convertManySpacesToOne(struct Line *line_list){
	char *temp_content;
	int i, j, k;
	
	while (line_list != NULL) {
		temp_content = my_malloc(BUFFER_MAX_LEN);
		temp_content[0] = line_list->content[0];
		for(j = 1, i = 1; i < strlen(line_list->content); i++){
			if (line_list->content[i] != ' ') temp_content[j++] = line_list->content[i];
			else if (line_list->content[i] == ' '){
				k = i+1;
				temp_content[j++] = ' ';
				while(line_list->content[k] == ' ') k++;
				i = k-1;
			}
		}
		free(line_list->content);
		line_list->content = my_strdup(temp_content, BUFFER_MAX_LEN);
		line_list = line_list->next;
		free(temp_content);
	}
}

/*this function gets a pointer to string and returns a pointer to the first letter*/
char *skip_spaces(char *s){
	char *p;
	for(p = s; !asciiCharacter(*p); p++) ;
	return p;
}

/*this function checks if a given char is a valid ascii letter*/
bool asciiCharacter(char c){
	return (33 <= c && c <= 126);
}


