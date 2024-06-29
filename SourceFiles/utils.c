#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <math.h>
#include "../HeaderFiles/utils.h"
#include "../HeaderFiles/error_handle.h"
#include "../HeaderFiles/line.h"

/**
purpose: custom fopen

@param file name
@param type - open types are: 'r'-read, 'w'-write
@return FILE pointer

*/
FILE* my_fopen(char *file_name, char *type){
	FILE *file = fopen(file_name, type);
	error_handle_file_open(file, file_name);
	return file;
}

/**
purpose: custom remove

@param file name
@return void

*/
void my_remove(char *file_name){
	int status = remove(file_name);
	error_handle_file_remove(status, file_name);
}

/**
purpose: custom strdup

@param source
@param size
@return string

*/
char* my_strdup(const char *src, int size) {
	char *dup = my_malloc(size);
	if(src == NULL) return dup;
	return strncpy(dup, src, size);
}

/**
purpose: custom mallloc

@param size
@return string

*/
char* my_malloc(int size){
	char *dup = (char*)malloc(size);
	error_handle_malloc_char(dup);
	memset(dup, '\0', size);
	return dup;
}

/**
purpose: custom mallloc

@param file name
@param new extention
@return string

*/
char* change_extension(char *file_name, char *new_extension) {
	/* Find the position of the last dot in the filename */
	const char *last_dot = strrchr(file_name, '.');
	char *output_file_name = my_malloc(BUFFER_MAX_LEN);
	if (last_dot != NULL) {
		size_t file_name_Length = last_dot - file_name;
		strncpy(output_file_name, file_name, file_name_Length);
		/* Append the new extension */
		strcat(output_file_name, ".");
		strcat(output_file_name, new_extension);
	} else {
		/* If no dot is found, just append the new extension to the original filename */
		strcpy(output_file_name, file_name);
		strcat(output_file_name, ".");
		strcat(output_file_name, new_extension);
	}
	return output_file_name;
}

/**
purpose: determine if a define command

@param buffer
@return true if and only if buf is a define command

*/
bool isDefine(char *buf){
	return (strncmp(buf, ".define", strlen(".define")) == 0 && buf[strlen(".define")] == ' ');
}

/**
purpose: determine if a line has a label

@param line
@return true if and only if the line has label

*/
bool hasLabel(struct Line *line){
	return (strlen(line->label) != 0);
}

/**
purpose: determine if a data command

@param buffer
@return true if and only if buf is a data command

*/
bool isData(char *buf){
	int status = 0;
	if(strlen(buf) < strlen(".data")) status = 0;
	else if(strlen(buf) == strlen(".data")) {
		if(strncmp(buf, ".data", strlen(".data")) == 0) status = 1;
		else status = 0;
	}
	else status = (strncmp(buf, ".data", strlen(".data")) == 0 && buf[strlen(".data")] == ' ');
	return status;
}

/**
purpose: determine if a string command

@param buffer
@return true if and only if buf is a string command

*/
bool isString(char *buf){
	int status = 0;
	if(strlen(buf) < strlen(".string")) status = 0;
	else if(strlen(buf) == strlen(".string")) {
		if(strncmp(buf, ".string", strlen(".string")) == 0) status = 1;
		else status = 0;
	}
	else status = (strncmp(buf, ".string", strlen(".string")) == 0 && buf[strlen(".string")] == ' ');
	return status;
}

/**
purpose: determine if a entry command

@param buffer
@return true if and only if buf is a entry command

*/
bool isEntry(char *buf){
	int status = 0;
	if(strlen(buf) < strlen(".entry")) status = 0;
	else if(strlen(buf) == strlen(".entry")) {
		if(strncmp(buf, ".entry", strlen(".entry")) == 0) status = 1;
		else status = 0;
	}
	else status = (strncmp(buf, ".entry", strlen(".entry")) == 0 && buf[strlen(".entry")] == ' ');
	return status;
}

/**
purpose: determine if a extern command

@param buffer
@return true if and only if buf is a extern command

*/
bool isExtern(char *buf){
	int status = 0;
	if(strlen(buf) < strlen(".extern")) status = 0;
	else if(strlen(buf) == strlen(".extern")) {
		if(strncmp(buf, ".extern", strlen(".extern")) == 0) status = 1;
		else status = 0;
	}
	else status = (strncmp(buf, ".extern", strlen(".extern")) == 0 && buf[strlen(".extern")] == ' ');
	return status;
}

/**
purpose: determine if a its a command

@param buffer
@return true if and only if buf is a command

*/
bool isCommand(char *buf){
	char *temp_buffer = my_strdup(buf, BUFFER_MAX_LEN);
	char *cmd_name = strtok(temp_buffer, " ");
	int i;
	for (i = 0; i < NUMBER_OF_TOTAL_INSTRUCTIONS; i++) {
		if (needZeroOperands(cmd_name) || needOneOperand(cmd_name) || needTwoOperands(cmd_name)){
			free(temp_buffer);
			return true;
		}
	}
	free(temp_buffer);
	return false;
}

/**
purpose: determine if it is a start of macro

@param buffer
@return true if and only if buf is a marco line

*/
bool isMCR(char *buf){
	return (strncmp(buf, "mcr", MCR_LEN) == 0 && buf[MCR_LEN] == ' ');
}

/**
purpose: determine if it is a end of macro line

@param buffer
@return true if and only if buf is end of marco

*/
bool isENDMCR(char *buf){
	return (strncmp(buf, "endmcr", END_MCR_LEN) == 0 && strlen(buf) == END_MCR_LEN);
}

/**
purpose: determine if command need zero operands

@param cmd name
@return true if and only if command need zero operands

*/
bool needZeroOperands(char *cmd_name){
	char *zero_operand_instructions_list[] = {"rts", "hlt"};
	int i;
	for (i = 0; i < NUMBER_OF_ZERO_OPERAND_INSTRUCTIONS; i++) {
		if (strncmp(cmd_name, zero_operand_instructions_list[i], CMD_LEN) == 0 && !isprint(cmd_name[CMD_LEN])) return true;
	}
	return false;
}

/**
purpose: determine if command need one operands

@param cmd name
@return true if and only if command need one operands

*/
bool needOneOperand(char *cmd_name){
	char *single_operand_instructions_list[] = {"not", "clr", "inc", "dec", "jmp", "bne", "red", "prn", "jsr"};
	int i;
	for (i = 0; i < NUMBER_OF_ONE_OPERAND_INSTRUCTIONS; i++) {
		if (strncmp(cmd_name, single_operand_instructions_list[i], CMD_LEN) == 0 && !isprint(cmd_name[CMD_LEN])) return true;
	}
	return false;
}

/**
purpose: determine if command need two operands

@param cmd name
@return true if and only if command need two operands

*/
bool needTwoOperands(char *cmd_name){
	char *two_operand_instructions_list[] = {"mov", "cmp", "add", "sub", "lea"};
	int i;
	for (i = 0; i < NUMBER_OF_TWO_OPERAND_INSTRUCTIONS; i++) {
		if (strncmp(cmd_name, two_operand_instructions_list[i], CMD_LEN) == 0 && !isprint(cmd_name[CMD_LEN])) return true;
	}
	return false;
}

/**
purpose: determine if name is a saved word in assembly
a saved word:
1) assembly commands - "rts", "hlt", "not", "clr", "inc", "dec", "jmp", "bne", "red", "prn", "jsr", "mov", "cmp", "add", "sub", "lea"
2) assembly defenitions - ".data", ".define", ".entry", ".extern"
3) mcr declaration - "mcr", endmcr"

@param name
@return true if and only if name is a saved word

*/
int is_saved_word(char* name){
	if(needZeroOperands(name) || needOneOperand(name) || needTwoOperands(name)) return 1;
	if(isData(name) || isString(name) || isEntry(name) || isExtern(name)) return 1;
	if(isMCR(name) || isENDMCR(name)) return 1;
	return 0;
}

/**
purpose: determine if buf is a natural number
1) can contain a +/- at start
2) after that only digits
3) cant contain a dot

@param buf
@return int - 1 if buf is a natural number, else 0

*/
int is_number(char *buf) {
	int i;
	if(buf == NULL) return 0;
	if(*buf == '\0') return 0;
	
	while (isspace(*buf) && *buf != '\0') buf++;
	if (*buf == '+' || *buf == '-') buf++;
	if(*buf == '\0') return 0;
	
	for(i=0; i<strlen(buf); i++) {
		if(!isdigit(*(buf+i)) ) return 0;
	}
	
	return 1;

}

/**
purpose: determine if num can be in define defenition

@param num
@return int - 1 if true

*/
int is_valid_define_number(int num) {
	return (-2048 <= num && num <= 2047);
}

/**
purpose: determine if num can be in data defenition

@param num
@return int - 1 if true

*/
int is_valid_data_number(int num) {
	return (-8192 <= num && num <= 8191);
}

/**
purpose: determine if buf is a valid operand

@param buf
@param line number
@param symbol list
@return int - 0 if true

*/
int is_valid_operand(char *buf, int line_number, struct Symbol *symbol_list){
	if(*buf == '#') return is_valid_immidiate_addressing(buf, line_number);
	else if(*buf == 'r') return is_valid_register_addressing(buf, line_number);
	else if(strstr(buf, "[") != NULL && strstr(buf, "]") != NULL) return is_valid_constant_addressing(buf, line_number, symbol_list);
	else return 1;
}

/**
purpose: determine if buf is a valid immidiate addressing

@param buf
@param line number
@return int - 1 if true

*/
int is_valid_immidiate_addressing(char *buf, int line_number){
	if(*buf == '#'){
		buf++;
		if(!is_number(buf)) {
			printf(MSG_CMD_IMMIDIATE_OPERAND_NUMBER(line_number));
			return 1;
		}
		if(!is_valid_data_number(atoi(buf))) {
			printf(MSG_CMD_IMMIDIATE_OPERAND_RANGE(line_number));
			return 1;
		}
		return 0;
	}
	return 0;
}

/**
purpose: determine if buf is a valid register addressing

@param buf
@param line number
@return int - 1 if true

*/
int is_valid_register_addressing(char *buf, int line_number){
	if(*buf == 'r'){
		buf++;
		if(!is_number(buf)) {
			printf(MSG_CMD_DIRECT_OPERAND_NUMBER(line_number));
			return 0;
		}
		if(atoi(buf) < 0 || 7 < atoi(buf))  {
			printf(MSG_CMD_DIRECT_OPERAND_RANGE(line_number));
			return 0;
		}
		return 1;
	}
	return 0;
}

/**
purpose: determine if buf is a valid constant addressing

@param buf
@param line number
@return int - 1 if true

*/
int is_valid_constant_addressing(char *buf, int line_number, struct Symbol *symbol_list){
	
	return 1;
}

/**
purpose: determine if buf is a valid direct addressing

@param buf
@param line number
@return int - 1 if true

*/
int is_valid_direct_addressing(char *buf, int line_number, struct Symbol *symbol_list){
	
	return 1;
}





