#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "../HeaderFiles/error_handle.h"
#include "../HeaderFiles/utils.h"
#include "../HeaderFiles/line.h"

bool hasErrors = false;

void raiseFlag(int buf) {
	/*printf("\nraised falg %d",buf);*/
	hasErrors = true;
}

void error_handle_has_errors(char *file_name) {
	char *file_name_am, *file_name_ob, *file_name_ent, *file_name_ext;
	file_name = strtok(file_name, ".");
	file_name_am = change_extension(file_name, EXTENTION_AM);
	file_name_ob = change_extension(file_name, EXTENTION_OB);
	file_name_ent = change_extension(file_name, EXTENTION_ENT);
	file_name_ext = change_extension(file_name, EXTENTION_EXT);
	if(hasErrors) {
		my_remove(file_name_am);
		my_remove(file_name_ob);
		my_remove(file_name_ent);
		my_remove(file_name_ext);
	}
	free(file_name_am);
	free(file_name_ob);
	free(file_name_ent);
	free(file_name_ext);
}


void error_handle_args(int argc, char *argv[]) {
	if (argc == 1) {
		printf(MSG_ARGC(argv[0]));
		raiseFlag(1);
	}
}

int error_handle_undefined_line(int number){
	printf(MSG_LINE_UNDEFINED(number));
	raiseFlag(2);
	return 1;
}

int error_handle_line_size(char *line_buffer, int number) {
	if(strlen(line_buffer) > LINE_MAX_LEN) {
		printf(MSG_LINE_SIZE(number));
		raiseFlag(3);
		return 1;
	}
	return 0;
}

int error_handle_label(char *label_buffer, int number) {
	int i;
	if(strlen(label_buffer) == 0) return 0;
	if(strlen(label_buffer) > LABEL_MAX_LEN) {
		printf(MSG_LABEL_SIZE(number));
		raiseFlag(4);
		return 1;
	}
	if(!isalpha(*label_buffer)) {
		printf(MSG_LABEL_UPPER(number));
		raiseFlag(5);
		return 1;
	}
	if(isspace(*(label_buffer + strlen(label_buffer) - 1))) {
		printf(MSG_LABEL_HAS_SPACE(number));
		raiseFlag(7);
		return 1;
	}
	for(i=0; i<strlen(label_buffer) ; i++) {
		if(!isalnum(*(label_buffer+i))) {
			printf(MSG_LABEL_REST(number));
			raiseFlag(6);
			return 1;
		}
	}
	if(is_saved_word(label_buffer)){
		printf(MSG_LABEL_SAVED(number));
		raiseFlag(8);
		return 1;
	}
	return 0;
}



int error_handle_label_expected(int number) {
	printf(MSG_LABEL_NONE(number));
	raiseFlag(9);
	return 1;
}


void error_handle_file_open(FILE *file, char *name) {
	if (file == NULL) {
		printf(MSG_FILE_OPEN(name));
		raiseFlag(10);
		exit(EXIT_FAILURE);
	}
}

void error_handle_file_empty(struct Line *head, char *file_name) {
	if (head == NULL) {
		printf(MSG_FILE_EMPTY(file_name));
		raiseFlag(11);
	}
}

void error_handle_file_remove(int status, char *file_name) {
	if (status != 0) {
		raiseFlag(12);
	}
	
}

void error_handle_malloc_char(char *dup) {
	if (dup == NULL) {
		printf(MSG_MALLOC);
		raiseFlag(13);
		exit(EXIT_FAILURE);
	}
}

void error_handle_malloc_line(struct Line *line) {
	if (line == NULL) {
		printf(MSG_MALLOC);
		raiseFlag(14);
		exit(EXIT_FAILURE);
	}
}

void error_handle_malloc_macro(struct Macro *macro) {
	if (macro == NULL) {
		printf(MSG_MALLOC);
		raiseFlag(15);
		exit(EXIT_FAILURE);
	}
}

void error_handle_malloc_binary_line(struct BinaryLine *binary_line) {
	if (binary_line == NULL) {
		printf(MSG_MALLOC);
		raiseFlag(16);
		exit(EXIT_FAILURE);
	}
}


int error_handle_define(char *buf, int line_number){
	char *temp_buffer = my_strdup(buf, BUFFER_MAX_LEN);
	char *pos, *name;
	int i;
	
	/*check value*/
	if(strstr(temp_buffer, "=") == NULL) {
		printf(MSG_DEFINE_USAGE(line_number));
		free(temp_buffer);
		raiseFlag(17);
		return 1;
	}
	strtok(temp_buffer, "=");
	pos = strtok(NULL, "=");
	if(pos == NULL) {
		printf(MSG_DEFINE_USAGE(line_number));
		free(temp_buffer);
		raiseFlag(18);
		return 1;
	}
	if(!is_number(pos)) {
		printf(MSG_DEFINE_USAGE(line_number));
		free(temp_buffer);
		raiseFlag(19);
		return 1;
	}
	if(!is_valid_define_number(atoi(pos))) {
		printf(MSG_DEFINE_NUMBER_RANGE(line_number));
		free(temp_buffer);
		raiseFlag(20);
		return 1;
	}
	free(temp_buffer);
	
	/*check name*/
	temp_buffer = my_strdup(buf, BUFFER_MAX_LEN);
	if(strstr(temp_buffer, "=") == NULL) {
		printf(MSG_DEFINE_USAGE(line_number));
		free(temp_buffer);
		raiseFlag(21);
		return 1;
	}
	pos = strtok(temp_buffer, "=");
	if(strstr(pos, " ") == NULL) {
		printf(MSG_DEFINE_USAGE(line_number));
		free(temp_buffer);
		raiseFlag(22);
		return 1;
	}
	strtok(pos, " ");
	name = strtok(NULL, " ");
	if(name == NULL) {
		printf(MSG_DEFINE_LABEL_NONE(line_number));
		free(temp_buffer);
		raiseFlag(23);
		return 1;
	}
	if(strlen(name) == 0) {
		printf(MSG_DEFINE_LABEL_NONE(line_number));
		free(temp_buffer);
		raiseFlag(24);
		return 1;
	}
	if(strlen(name) > LABEL_MAX_LEN) {
		printf(MSG_DEFINE_LABEL_SIZE(line_number));
		free(temp_buffer);
		return 1;
	}
	if(!isalpha(*name)) {
		printf(MSG_DEFINE_LABEL_UPPER(line_number));
		free(temp_buffer);
		raiseFlag(25);
		return 1;
	}
	for(i=0; i<strlen(name); i++) {
		if(!isalnum(*(name+i))) {
			printf(MSG_DEFINE_LABEL_REST(line_number));
			free(temp_buffer);
			raiseFlag(26);
			return 1;
		}
	}
	if(is_saved_word(name)){
		printf(MSG_DEFINE_LABEL_SAVED(line_number));
		free(temp_buffer);
		raiseFlag(27);
		return 1;
	}
	free(temp_buffer);
	
	return 0;
}


int error_handle_data(char *buf, int line_number, char *label, struct Symbol *symbol_list, struct Line *line_list) {
	char *temp_buffer = my_strdup(buf, BUFFER_MAX_LEN);
	char *pos;
	
	if(searchSymbolByName(symbol_list, label) != NULL) {
		printf(MSG_DUPLICATE_SYMBOLS);
		set_error_1_all(line_list);
		free(temp_buffer);
		raiseFlag(888);
		return 1;
	}
	
	/*calc binary lines*/
	pos = &temp_buffer[5];
	while(isspace(*pos) && *pos != '\0') pos++;
	if(strlen(pos) == 0) {
		printf(MSG_DATA_EMPTY(line_number));
		free(temp_buffer);
		raiseFlag(28);
		return 1;
	}
	
	if(strstr(pos, ",") == NULL){/*there is only one value*/
		struct Symbol *temp_symbol;
		/*search for symbol*/
		if (*pos == ' ') pos++;
		temp_symbol = searchSymbolByName(symbol_list, pos);
		if(temp_symbol == NULL) {
			if(!is_number(pos)){
				printf(MSG_DATA_NUMBERS_ONLY(line_number));
				free(temp_buffer);
				raiseFlag(29);
				return 1;
			}
			if(!is_valid_data_number(atoi(pos))){
				printf(MSG_DATA_NUMBER_RANGE(line_number));
				free(temp_buffer);
				raiseFlag(30);
				return 1;
			}
			free(temp_buffer);
			return 0;
		}
		free(temp_buffer);
		return 0;
	} 
	else {/*there are multipul values*/
		char *pos_end = (pos + strlen(pos) - 1);
		char *iterate_data;
		while(isspace(*pos) && *pos != '\0') pos++;
		if(*pos == ','){
			printf(MSG_DATA_COMMA_START(line_number));
			free(temp_buffer);
			raiseFlag(31);
			return 1;
		}
		if((strstr(pos, ",,") != NULL) || (strstr(pos, ", ,") != NULL)){
			printf(MSG_DATA_COMMA_CONSEC(line_number));
			free(temp_buffer);
			raiseFlag(32);
			return 1;
		}
		while(isspace(*pos_end) && pos_end > pos) pos_end--;
		if(*(pos_end) == ','){
			printf(MSG_DATA_COMMA_END(line_number));
			free(temp_buffer);
			raiseFlag(33);
			return 1;
		}
		iterate_data = strtok(pos, ",");
		while(iterate_data != NULL) {
			struct Symbol *temp_symbol;
			/*search for symbol*/
			if (*iterate_data == ' ') iterate_data++;
			temp_symbol = searchSymbolByName(symbol_list, iterate_data);
			if(temp_symbol == NULL) {
				if(!is_number(iterate_data)){
					printf(MSG_DATA_NUMBERS_ONLY(line_number));
					free(temp_buffer);
					raiseFlag(34);
					return 1;
				}
				if(!is_valid_data_number(atoi(iterate_data))){
					printf(MSG_DATA_NUMBER_RANGE(line_number));
					free(temp_buffer);
					raiseFlag(35);
					return 1;
				}
			}
			iterate_data = strtok(NULL, ",");
		}
		free(temp_buffer);
		return 0;
	}
	free(temp_buffer);
	raiseFlag(36);
	return 1;
}


int error_handle_string(char *buf, int line_number, char *label, struct Symbol *symbol_list, struct Line *line_list){
	char *temp_buffer = my_strdup(buf, BUFFER_MAX_LEN);
	char *pos = &temp_buffer[7];
	char *pos_end = (pos + strlen(pos) - 1);
	
	if(searchSymbolByName(symbol_list, label) != NULL) {
		printf(MSG_DUPLICATE_SYMBOLS);
		set_error_1_all(line_list);
		free(temp_buffer);
		raiseFlag(888);
		return 1;
	}
	
	while(isspace(*pos) && *pos != '\0') pos++;
	if(strlen(pos) == 0) {
		printf(MSG_STRING_NO_VALUE(line_number));
		free(temp_buffer);
		raiseFlag(37);
		return 1;
	}
	while(isspace(*pos_end) && pos_end > pos) pos_end--;
	if(*pos != '"'){
		printf(MSG_STRING_COMMA_START(line_number));
		free(temp_buffer);
		raiseFlag(38);
		return 1;
	}
	if(strlen(pos) == 1){
		printf(MSG_STRING_COMMA_END(line_number));
		free(temp_buffer);
		raiseFlag(39);
		return 1;
	}
	if(*pos_end != '"'){
		printf(MSG_STRING_COMMA_END(line_number));
		free(temp_buffer);
		raiseFlag(40);
		return 1;
	}
	
	free(temp_buffer);
	return 0;
}

int error_handle_cmd_second_pass(struct Line *curr_line, struct Symbol *symbol_list){
	char *temp_buffer = my_strdup(curr_line->content, BUFFER_MAX_LEN);
	char *pos_cmd_name = strtok(temp_buffer, " ");
	char *pos_operands = &temp_buffer[4];
	int line_number = curr_line->number;
	
	
	while(isspace(*pos_operands) && *pos_operands != '\0') pos_operands++;
	if(needZeroOperands(pos_cmd_name)) {
		free(temp_buffer);
		return 0;
	}
	if(needOneOperand(pos_cmd_name)){
		int target_addressing_type = ((curr_line->binary_lines)->data >> DEST_BITS_SHIFT) & 3;
		struct Symbol *temp_symbol = NULL;
		char *temp_name = NULL, *temp_pos = NULL;
		
		if(
		strncmp(pos_cmd_name, "not", CMD_LEN) == 0 ||
		strncmp(pos_cmd_name, "clr", CMD_LEN) == 0 ||
		strncmp(pos_cmd_name, "inc", CMD_LEN) == 0 ||
		strncmp(pos_cmd_name, "dec", CMD_LEN) == 0 ||
		strncmp(pos_cmd_name, "red", CMD_LEN) == 0
		) {
			if(target_addressing_type == 0) {
				printf(MSG_CMD_INVALID_TARGET_ADDRESSING_TYPE(line_number));
				free(temp_buffer);
				raiseFlag(41);
				return 1;
			}
		}
		if(
		strncmp(pos_cmd_name, "jmp", CMD_LEN) == 0 ||
		strncmp(pos_cmd_name, "bne", CMD_LEN) == 0 ||
		strncmp(pos_cmd_name, "jsr", CMD_LEN) == 0
		) {
			if(target_addressing_type == 0 || target_addressing_type == 2) {
				printf(MSG_CMD_INVALID_TARGET_ADDRESSING_TYPE(line_number));
				free(temp_buffer);
				raiseFlag(42);
				return 1;
			}
		}
		/*only prn remains as it accepts all type of adressing*/
		if (*pos_operands == ' ') pos_operands++;
		switch(target_addressing_type){

			case IMMIDITE_ADRESSING:
					pos_operands++;/* skip # */
					if(!is_number(pos_operands)){
						temp_symbol = searchSymbolByName(symbol_list, pos_operands);
						if(temp_symbol == NULL) {
							printf(MSG_CMD_INVALID_TARGET_OPERAND(line_number));
							free(temp_buffer);
							raiseFlag(43);
							return 1;
						}
						free(temp_buffer);
						return 0;
					}
					free(temp_buffer);
					return 0;
					break;

			case DIRECT_ADRESSING:
			
					temp_symbol = searchSymbolByName(symbol_list, pos_operands);
					if(temp_symbol == NULL) {
						printf(MSG_CMD_INVALID_TARGET_OPERAND(line_number));
						free(temp_buffer);
						raiseFlag(44);
						return 1;
					}
					free(temp_buffer);
					return 0;
					break;

			case CONSTANT_ADRESSING:
			
					temp_name = strtok(pos_operands, "[");
					if (*(temp_name + strlen(temp_name) - 1) == ' ') *(temp_name + strlen(temp_name) - 1) = '\0';
					temp_pos = strtok(NULL, "[");
					temp_pos[strlen(temp_pos)-1] = '\0';
					if (*(temp_pos + strlen(temp_pos) - 1) == ' ') *(temp_pos + strlen(temp_pos) - 1) = '\0';
					if (*temp_pos == ' ') temp_pos++;

					temp_symbol = searchSymbolByName(symbol_list, temp_name);
					if(temp_symbol == NULL) {
						printf(MSG_CMD_INVALID_TARGET_OPERAND(line_number));
						free(temp_buffer);
						raiseFlag(45);
						return 1;
					}
					if(!is_number(temp_pos)){
						temp_symbol = searchSymbolByName(symbol_list, temp_pos);
						if(temp_symbol == NULL) {
							printf(MSG_CMD_INVALID_TARGET_OPERAND(line_number));
							free(temp_buffer);
							raiseFlag(46);
							return 1;
						}
						free(temp_buffer);
						return 0;
					}
					free(temp_buffer);
					return 0;
					break;

			case REGISTER_ADRESSING:
					free(temp_buffer);
					return 0;
					break;
		}
		printf(MSG_CMD_INVALID_TARGET_OPERAND(line_number));
		free(temp_buffer);
		raiseFlag(47);
		return 1;
	}
	if(needTwoOperands(pos_cmd_name)){
		int target_addressing_type = ((curr_line->binary_lines)->data >> DEST_BITS_SHIFT) & 3;
		int source_addressing_type = ((curr_line->binary_lines)->data >> SOURCE_BITS_SHIFT) & 3;
		char *source_operand = NULL, *target_operand = NULL;
		struct Symbol *temp_symbol = NULL;
		char *temp_name = NULL, *temp_pos = NULL;
		int flag = 0;
		
		if(
		strncmp(pos_cmd_name, "mov", CMD_LEN) == 0 ||
		strncmp(pos_cmd_name, "add", CMD_LEN) == 0 ||
		strncmp(pos_cmd_name, "sub", CMD_LEN) == 0 ||
		strncmp(pos_cmd_name, "lea", CMD_LEN) == 0
		) {
			if(target_addressing_type == 0) {
				printf(MSG_CMD_INVALID_TARGET_ADDRESSING_TYPE(line_number));
				free(temp_buffer);
				raiseFlag(48);
				return 1;
			}
		}
		/*only cmp remains as it accepts all type of adressing to target*/
		if(strncmp(pos_cmd_name, "lea", CMD_LEN) == 0) {
			if(source_addressing_type == 0 || source_addressing_type == 3) {
				printf(MSG_CMD_INVALID_SOURCE_ADDRESSING_TYPE(line_number));
				free(temp_buffer);
				raiseFlag(49);
				return 1;
			}
		}
		source_operand = strtok(pos_operands, ",");
		target_operand = strtok(NULL, ",");
		if (*source_operand == ' ') source_operand++;
		if (*target_operand == ' ') target_operand++;
		
		switch(source_addressing_type){

			case IMMIDITE_ADRESSING:
					source_operand++;
					if(!is_number(source_operand)){
						temp_symbol = searchSymbolByName(symbol_list, source_operand);
						if(temp_symbol == NULL) {
							printf(MSG_CMD_INVALID_SOURCE_OPERAND(line_number));
							free(temp_buffer);
							raiseFlag(50);
							return 1;
						}
						flag++;
					}
					else flag++;
					break;

			case DIRECT_ADRESSING:
			
					temp_symbol = searchSymbolByName(symbol_list, source_operand);
					if(temp_symbol == NULL) {
						printf(MSG_CMD_INVALID_SOURCE_OPERAND(line_number));
						free(temp_buffer);
						raiseFlag(51);
						return 1;
					}
					flag++;
					break;

			case CONSTANT_ADRESSING:
			
					temp_name = strtok(source_operand, "[");
					if (*(temp_name + strlen(temp_name) - 1) == ' ') *(temp_name + strlen(temp_name) - 1) = '\0';
					temp_pos = strtok(NULL, "[");
					temp_pos[strlen(temp_pos)-1] = '\0';
					if (*(temp_pos + strlen(temp_pos) - 1) == ' ') *(temp_pos + strlen(temp_pos) - 1) = '\0';
					if (*temp_pos == ' ') temp_pos++;

					temp_symbol = searchSymbolByName(symbol_list, temp_name);
					if(temp_symbol == NULL) {
						printf(MSG_CMD_INVALID_SOURCE_OPERAND(line_number));
						free(temp_buffer);
						raiseFlag(52);
						return 1;
					}
					if(!is_number(temp_pos)){
						temp_symbol = searchSymbolByName(symbol_list, temp_pos);
						if(temp_symbol == NULL) {
							printf(MSG_CMD_INVALID_SOURCE_OPERAND(line_number));
							free(temp_buffer);
							raiseFlag(53);
							return 1;
						}
						flag++;
					}
					flag++;
					break;

			case REGISTER_ADRESSING:
					flag++;
					break;
		}
		switch(target_addressing_type){

			case IMMIDITE_ADRESSING:
					target_operand++;
					if(!is_number(target_operand)){
						temp_symbol = searchSymbolByName(symbol_list, target_operand);
						if(temp_symbol == NULL) {
							printf(MSG_CMD_INVALID_TARGET_OPERAND(line_number));
							free(temp_buffer);
							raiseFlag(54);
							return 1;
						}
						flag++;
					}
					else flag++;
					break;

			case DIRECT_ADRESSING:
			
					temp_symbol = searchSymbolByName(symbol_list, target_operand);
					if(temp_symbol == NULL) {
						printf(MSG_CMD_INVALID_TARGET_OPERAND(line_number));
						free(temp_buffer);
						raiseFlag(55);
						return 1;
					}
					flag++;
					break;

			case CONSTANT_ADRESSING:
			
					temp_name = strtok(target_operand, "[");
					if (*(temp_name + strlen(temp_name) - 1) == ' ') *(temp_name + strlen(temp_name) - 1) = '\0';
					temp_pos = strtok(NULL, "[");
					temp_pos[strlen(temp_pos)-1] = '\0';
					if (*(temp_pos + strlen(temp_pos) - 1) == ' ') *(temp_pos + strlen(temp_pos) - 1) = '\0';
					if (*temp_pos == ' ') temp_pos++;

					temp_symbol = searchSymbolByName(symbol_list, temp_name);
					if(temp_symbol == NULL) {
						printf(MSG_CMD_INVALID_TARGET_OPERAND(line_number));
						free(temp_buffer);
						raiseFlag(56);
						return 1;
					}
					if(!is_number(temp_pos)){
						temp_symbol = searchSymbolByName(symbol_list, temp_pos);
						if(temp_symbol == NULL) {
							printf(MSG_CMD_INVALID_TARGET_OPERAND(line_number));
							free(temp_buffer);
							raiseFlag(57);
							return 1;
						}
						flag++;
					}
					else flag++;
					break;

			case REGISTER_ADRESSING:
					flag++;
					break;
		}
		if(flag >= 2) {
			free(temp_buffer);
			return 0;
		}
		free(temp_buffer);
		raiseFlag(58);
		return 1;
	}
	free(temp_buffer);
	raiseFlag(59);
	return 1;
}

int error_handle_cmd(char *buf, int line_number, char *label, struct Symbol *symbol_list, struct Line *line_list){
	char *temp_buffer = my_strdup(buf, BUFFER_MAX_LEN);
	char *pos_cmd_name = strtok(temp_buffer, " ");
	char *pos_operands = &temp_buffer[4];
	char *pos_end = (pos_operands + strlen(pos_operands) - 1);
	while(isspace(*pos_end) && pos_end > pos_operands) pos_end--;
	
	if(searchSymbolByName(symbol_list, label) != NULL) {
		printf(MSG_DUPLICATE_SYMBOLS);
		set_error_1_all(line_list);
		free(temp_buffer);
		raiseFlag(888);
		return 1;
	}
	while(isspace(*pos_operands) && *pos_operands != '\0') pos_operands++;
	if(needZeroOperands(pos_cmd_name)) {
		if(strlen(pos_operands) != 0) {
			printf(MSG_CMD_EXTORENDOS_TEXT(line_number));
			free(temp_buffer);
			raiseFlag(60);
			return 1;
		}
		free(temp_buffer);
		return 0;
	}
	if(needOneOperand(pos_cmd_name)){
		if(strlen(pos_operands) == 0) {
			printf(MSG_CMD_MISSING_OPERAND(line_number));
			free(temp_buffer);
			raiseFlag(61);
			return 1;
		}
		if(strstr(pos_operands, ",") != NULL){
			printf(MSG_CMD_ONLY_ONE(line_number));
			free(temp_buffer);
			raiseFlag(62);
			return 1;
		}
		free(temp_buffer);
		return 0;
	}
	if(needTwoOperands(pos_cmd_name)){
		if(strlen(pos_operands) == 0) {
			printf(MSG_CMD_MISSING_OPERANDS(line_number));
			free(temp_buffer);
			raiseFlag(63);
			return 1;
		}
		if(strstr(pos_operands, ",") == NULL){
			printf(MSG_CMD_MISSING_OPERAND(line_number));
			free(temp_buffer);
			raiseFlag(64);
			return 1;
		}
		if(*pos_operands == ',' || *pos_end == ','){
			printf(MSG_CMD_MISSING_OPERAND(line_number));
			free(temp_buffer);
			raiseFlag(65);
			return 1;
		}
		free(temp_buffer);
		return 0;
	}
	printf(MSG_CMD_NAME_UNDEFINED(line_number));
	free(temp_buffer);
	raiseFlag(66);
	return 1;
}

int error_handle_entry(char *buf, int line_number, struct Symbol *symbol_list, struct Line *line_list) {
	char *temp_buffer = my_strdup(buf, BUFFER_MAX_LEN);
	char *pos_operand = &temp_buffer[6];
	struct Symbol *temp_symbol = NULL;
	
	while(isspace(*pos_operand) && *pos_operand != '\0') pos_operand++;
	if(strlen(pos_operand) == 0) {
		printf(MSG_ENTRY_EMPTY(line_number));
		free(temp_buffer);
		raiseFlag(67);
		return 1;
	}
	temp_symbol = searchSymbolByName(symbol_list, pos_operand);
	if(temp_symbol == NULL) {
		printf(MSG_ENTRY_DONT_EXIST(line_number));
		free(temp_buffer);
		raiseFlag(68);
		return 1;
	}
	if(temp_symbol->defined == 1) {
		printf(MSG_DUPLICATE_SYMBOLS);
		set_error_1_all(line_list);
		free(temp_buffer);
		raiseFlag(888);
		return 1;
	}
	free(temp_buffer);
	return 0;
}


int error_handle_extern(char *buf, int line_number, struct Symbol *symbol_list, struct Line *line_list) {
	char *temp_buffer = my_strdup(buf, BUFFER_MAX_LEN);
	char *pos_operand = &temp_buffer[8];
	int i;
	struct Symbol *temp_symbol = NULL;	
	
	while(isspace(*pos_operand) && *pos_operand != '\0') pos_operand++;
	if(strlen(pos_operand) == 0) {
		printf(MSG_EXTERN_NONE(line_number));
		free(temp_buffer);
		raiseFlag(69);
		return 1;
	}
	if(strlen(pos_operand) > LABEL_MAX_LEN) {
		printf(MSG_EXTERN_SIZE(line_number));
		free(temp_buffer);
		raiseFlag(70);
		return 1;
	}
	if(!isalpha(*pos_operand)) {
		printf(MSG_EXTERN_UPPER(line_number));
		free(temp_buffer);
		raiseFlag(71);
		return 1;
	}
	for(i=0; i<strlen(pos_operand) ; i++) {
		if(!isalnum(*(pos_operand+i))) {
			printf(MSG_EXTERN_REST(line_number));
			free(temp_buffer);
			raiseFlag(72);
			return 1;
		}
	}
	if(is_saved_word(pos_operand)){
		printf(MSG_EXTERN_SAVED(line_number));
		free(temp_buffer);
		raiseFlag(73);
		return 1;
	}
	
	temp_symbol = searchSymbolByName(symbol_list, pos_operand);
	if(temp_symbol != NULL) {
		if(temp_symbol->type != 1) {;
			printf(MSG_EXTERN_IN_FILE);
			set_error_1_all(line_list);
			free(temp_buffer);
			raiseFlag(74);
			return 1;
		}
	}
	free(temp_buffer);
	return 0;
}


int error_handle_add_symbol(struct Line *line_list, struct Symbol *symbol_list, char *name) {
	if(searchSymbolByName(symbol_list, name) != NULL) {
		printf(MSG_DUPLICATE_SYMBOLS);
		set_error_1_all(line_list);
		return 1;
	}
	printSymbols(symbol_list);
	return 0;

}





