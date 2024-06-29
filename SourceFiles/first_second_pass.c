#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "../HeaderFiles/first_second_pass.h"
#include "../HeaderFiles/error_handle.h"
#include "../HeaderFiles/line.h"
#include "../HeaderFiles/binary_line.h"
#include "../HeaderFiles/utils.h"
#include "../HeaderFiles/symbol.h"


/**
purpose: preform first and second pass:
1) first pass
2) second pass
3) make the .ob file
4) make the .ent .ext files

@param line list - of the .am file
@param file name
@return void

*/
void first_second_pass(struct Line **line_list, char *file_name){
	struct Symbol *symbol_list = NULL;
	char *file_name_am = change_extension(file_name, EXTENTION_AM);
	char *file_name_ob = change_extension(file_name, EXTENTION_OB);
	
	firstPass(line_list, file_name_am, &symbol_list);
	secondPass(line_list, file_name_am, &symbol_list);
	fprintMachineCode(*line_list, symbol_list, file_name_ob);
	
	free(file_name_am);
	free(file_name_ob);
	freeSymbols(symbol_list);
}

/**
purpose: make the .ent .ext files

@param symbol list
@param binaryline list
@param file name
@return void

*/
void entext(struct Symbol *symbol_list, struct BinaryLine *binary_line_list, char *file_name){
	char *file_name_ent = change_extension(file_name, EXTENTION_ENT);
	char *file_name_ext = change_extension(file_name, EXTENTION_EXT);
	
	fprintENTLines(symbol_list, binary_line_list, file_name_ent);
	fprintEXTLines(symbol_list, binary_line_list, file_name_ext);
	
	free(file_name_ent);
	free(file_name_ext);
}

/**
purpose: make the .ob file and .ent .ext files

@param line list
@param symbol list
@param file name
@return void

*/
void fprintMachineCode(struct Line *line_list, struct Symbol *symbol_list, char *file_name){
	FILE *file = my_fopen(file_name, WRITE);
	struct Line *curr_line = NULL;
	struct Symbol *curr_symbol = NULL;
	struct BinaryLine *binary_line_list = NULL;
	
	curr_symbol = symbol_list;
	while (curr_symbol != NULL) {
		if(curr_symbol->feature == CODE) {
			bool flag = true;
			curr_line = line_list;
			while (curr_line != NULL && flag) {
				if(strncmp(curr_line->label, curr_symbol->name, strlen(curr_symbol->name)) == 0) flag = false;	
				else curr_line = curr_line->next;
			}
			flag = true;
			if(curr_line->error == 0) addBinaryLines(&binary_line_list, curr_line->binary_lines);
			curr_line = curr_line->next;
			while(curr_line != NULL && flag){
				
				if(strlen(curr_line->label) != 0) flag = false;
				else if(curr_line->error == 0) addBinaryLines(&binary_line_list, curr_line->binary_lines);
				curr_line = curr_line->next;
			}
		}
		curr_symbol = curr_symbol->next;
	}
	
	curr_symbol = symbol_list;
	while (curr_symbol != NULL) {
		if(curr_symbol->feature == DATA && curr_symbol->type == RELOCATABLE) {
			bool flag = true;
			curr_line = line_list;
			while (curr_line != NULL && flag) {
				if(strncmp(curr_line->label, curr_symbol->name, strlen(curr_symbol->name)) == 0) flag = false;
				else curr_line = curr_line->next;
			}
			flag = true;
			if(curr_line->error == 0)addBinaryLines(&binary_line_list, curr_line->binary_lines);
		}
		curr_symbol = curr_symbol->next;
	}
	fixAdresses(binary_line_list);
	fprintEncodedLines(binary_line_list, file_name);
	entext(symbol_list, binary_line_list, file_name);
	
	freeBinaryLines(binary_line_list);
	fclose(file);
}

/**
purpose: perform the second pass

@param line list
@param symbol list
@param file name
@return void

*/
void secondPass(struct Line **line_list, char *file_name, struct Symbol **symbol_list){
	int IC = 0;
	struct Line *curr_line = *line_list;
	while (curr_line != NULL) {
		char *temp_buffer = my_malloc(BUFFER_MAX_LEN);
		int bin_data = 0, bin_data_extra = 0, error = 0;
		/*helper vars*/
		char *temp_name, *temp_pos;
		int pos;
		
		if(isCommand(curr_line->content)){/*handle valid cmd*/
			if(curr_line->error == 0) {
				char *cmd_name = NULL, *operands = NULL, *source_operand = NULL, *target_operand = NULL;
				int leap = 1;
				int source_addressing_type = 0, target_addressing_type = 0;
				struct Symbol *temp_symbol = NULL;
			
				error = error_handle_cmd_second_pass(curr_line, *symbol_list);
				if(error == 0){/*error*/
					/*add extra binary lines*/
					strcpy(temp_buffer, curr_line->content);
					cmd_name = strtok(temp_buffer, " ");
					/*clac leap and source/target adressing types*/
					operands = &temp_buffer[4];
					if(needOneOperand(cmd_name)){/* one operand*/

						target_operand = operands;
						if (*target_operand == ' ') target_operand++;
						target_addressing_type = adressing_type(target_operand);
						switch(target_addressing_type){

							case IMMIDITE_ADRESSING:
									target_operand++;/* skip # */
									temp_symbol = searchSymbolByName(*symbol_list, target_operand);
									if(temp_symbol == NULL){
										bin_data += atoi(target_operand);
										bin_data = bin_data << ARE_SHIFT;
									} else {
										bin_data += temp_symbol->data;
										bin_data = bin_data << ARE_SHIFT;
										bin_data += temp_symbol->type;
									}
									leap++;
									break;

							case DIRECT_ADRESSING:
							
									temp_symbol = searchSymbolByName(*symbol_list, target_operand);
									bin_data += temp_symbol->data;
									bin_data = bin_data << ARE_SHIFT;
									bin_data += temp_symbol->type;
									leap++;
							
									break;

							case CONSTANT_ADRESSING:
							
									temp_name = strtok(target_operand, "[");
									temp_pos = strtok(NULL, "[");
									temp_pos[strlen(temp_pos)-1] = '\0';
									temp_symbol = searchSymbolByName(*symbol_list, temp_name);
									bin_data += temp_symbol->data;
									bin_data = bin_data << ARE_SHIFT;
									bin_data += temp_symbol->type;
									temp_symbol = searchSymbolByName(*symbol_list, temp_pos);
									if(temp_symbol != NULL) bin_data_extra += temp_symbol->data;
									else {
										pos = atoi(temp_pos);
										bin_data_extra += pos;
									}
									bin_data_extra = bin_data_extra << ARE_SHIFT;
									leap+=2;
									break;

							case REGISTER_ADRESSING:
							
									leap++;
									break;
						}
						if(temp_symbol == NULL){
							addBinaryLine(&curr_line->binary_lines, IC, bin_data, NULL);/*add binary line*/
							if(bin_data_extra != 0) {
								addBinaryLine(&curr_line->binary_lines, IC, bin_data_extra, NULL);/*add binary line*/
							}
						} else {
							if(temp_symbol->type == EXTERNAL){
								addBinaryLine(&curr_line->binary_lines, IC, bin_data, temp_symbol->name);/*add binary line*/
								if(bin_data_extra != 0) {
									addBinaryLine(&curr_line->binary_lines, IC, bin_data_extra, temp_symbol->name);/*add binary line*/
								}
							} else {
								addBinaryLine(&curr_line->binary_lines, IC, bin_data, NULL);/*add binary line*/
								if(bin_data_extra != 0) {
									addBinaryLine(&curr_line->binary_lines, IC, bin_data_extra, NULL);/*add binary line*/
								}
							}
						}
					}
					else if(needTwoOperands(cmd_name)){/* two operands */

						source_operand = strtok(operands, ",");
						target_operand = strtok(NULL, ",");

						if (*source_operand == ' ') source_operand++;
						source_addressing_type = adressing_type(source_operand);
						if (*target_operand == ' ') target_operand++;
						target_addressing_type = adressing_type(target_operand);

						if(source_addressing_type == REGISTER_ADRESSING && target_addressing_type == REGISTER_ADRESSING){
							leap++;
							source_operand++;
							pos = atoi(source_operand);
							pos = pos << SOURCE_REGIESTER_SHIFT;
							bin_data += pos;
							target_operand++;
							pos = atoi(target_operand);
							pos = pos << DEST_REGIESTER_SHIFT;
							bin_data += pos;
							addBinaryLine(&curr_line->binary_lines, IC, bin_data, NULL);/*add binary line*/
						} else {
							switch(source_addressing_type){

								case IMMIDITE_ADRESSING:
										source_operand++;/* skip # */
										temp_symbol = searchSymbolByName(*symbol_list, source_operand);
										if(temp_symbol == NULL){
											bin_data += atoi(source_operand);
											bin_data = bin_data << ARE_SHIFT;
										} else {
											bin_data += temp_symbol->data;
											bin_data = bin_data << ARE_SHIFT;
											bin_data += temp_symbol->type;
										}
										leap++;
										break;

								case DIRECT_ADRESSING:
										temp_symbol = searchSymbolByName(*symbol_list, source_operand); 
										bin_data += temp_symbol->data;
										bin_data = bin_data << ARE_SHIFT;
										bin_data += temp_symbol->type;
										leap++;
										break;
					
								case CONSTANT_ADRESSING:
										temp_name = strtok(source_operand, "[");
										temp_pos = strtok(NULL, "[");
										temp_pos[strlen(temp_pos)-1] = '\0';
										temp_symbol = searchSymbolByName(*symbol_list, temp_name);
										bin_data += temp_symbol->data;
										bin_data = bin_data << ARE_SHIFT;
										bin_data += temp_symbol->type;
										temp_symbol = searchSymbolByName(*symbol_list, temp_pos);
										if(temp_symbol != NULL) bin_data_extra += temp_symbol->data;
										else {
											pos = atoi(temp_pos);
											bin_data_extra += pos;
										}
										bin_data_extra = bin_data_extra << ARE_SHIFT;
										leap+=2;
										break;

								case REGISTER_ADRESSING:
										source_operand++;
										pos = atoi(source_operand);
										bin_data += pos;
										bin_data = bin_data << SOURCE_REGIESTER_SHIFT;
										leap++;
										break;
							}/*end switch*/
							addBinaryLine(&curr_line->binary_lines, IC, bin_data, NULL);/*add binary line*/
							if(bin_data_extra != 0) {
								if(temp_symbol != NULL) addBinaryLine(&curr_line->binary_lines, IC, bin_data_extra, temp_symbol->name);/*add binary line*/
								else addBinaryLine(&curr_line->binary_lines, IC, bin_data_extra, NULL);/*add binary line*/
							}
				
							bin_data = 0;
							bin_data_extra = 0;
							temp_symbol = NULL;
					
				
				
								
							switch(target_addressing_type){

								case IMMIDITE_ADRESSING:
										target_operand++;/* skip # */
										temp_symbol = searchSymbolByName(*symbol_list, target_operand);
										if(temp_symbol == NULL){
											bin_data += atoi(target_operand);
											bin_data = bin_data << ARE_SHIFT;
										} else {
											bin_data += temp_symbol->data;
											bin_data = bin_data << ARE_SHIFT;
											bin_data += temp_symbol->type;
										}
										leap++;
										break;

								case DIRECT_ADRESSING:
										temp_symbol = searchSymbolByName(*symbol_list, target_operand);
										bin_data += temp_symbol->data;
										bin_data = bin_data << ARE_SHIFT;
										bin_data += temp_symbol->type;
										leap++;
										break;

								case CONSTANT_ADRESSING:
										temp_name = strtok(target_operand, "[");
										temp_pos = strtok(NULL, "[");
										temp_pos[strlen(temp_pos)-1] = '\0';
										temp_symbol = searchSymbolByName(*symbol_list, temp_name);
										bin_data += temp_symbol->data;
										bin_data = bin_data << ARE_SHIFT;
										bin_data += temp_symbol->type;
										temp_symbol = searchSymbolByName(*symbol_list, temp_pos);
										if(temp_symbol != NULL) bin_data_extra += temp_symbol->data;
										else {
											pos = atoi(temp_pos);
											bin_data_extra += pos;
										}
										bin_data_extra = bin_data_extra << ARE_SHIFT;
										leap+=2;
										break;

								case REGISTER_ADRESSING:
										target_operand++;
										pos = atoi(target_operand);
										bin_data += pos;
										bin_data = bin_data << DEST_REGIESTER_SHIFT;
										leap++;
										break;
							}/*end switch*/
							addBinaryLine(&curr_line->binary_lines, IC, bin_data, NULL);/*add binary line*/
							if(bin_data_extra != 0) {
								if(temp_symbol != NULL) addBinaryLine(&curr_line->binary_lines, IC, bin_data_extra, temp_symbol->name);/*add binary line*/
								else addBinaryLine(&curr_line->binary_lines, IC, bin_data_extra, NULL);/*add binary line*/
							}
						}/*end else*/
				
					}
					IC += leap;/*end is cmd*/
				}
				set_error(curr_line, error);
			}
		}
		free(temp_buffer);
		curr_line = curr_line->next;/*end while*/
	}
}


/**
purpose: perform the first pass

@param line list
@param symbol list
@param file name
@return void

*/
void firstPass(struct Line **line_list, char *file_name, struct Symbol **symbol_list){
	struct Line *curr_line = *line_list; /*used to iterate line list*/
	int bin_data, error;/*the bin data to construct, and error*/
	int IC = 100, DC = 0;/*IC = instruction count, DC = data count*/
	char *temp_buffer, *temp_label, *temp_name;/*temp buffers to hold data*/
	
	/*handle .define, .data, .string, command and construct symbol list*/
	while (curr_line != NULL) {
		error = 0;
		if(curr_line->error == 0) {
			/*init*/
			temp_buffer = my_malloc(BUFFER_MAX_LEN);
			temp_label = my_malloc(BUFFER_MAX_LEN);
			bin_data = 0;/*init binary data*/
			error = 0;/*init error*/
			
			if(isDefine(curr_line->content)){/*handle define*/
				char *pos, *name;
				int data = 0;
				/*get data*/
				error = error_handle_define(curr_line->content, curr_line->number);
				if(error == 0){/*error*/
					strcpy(temp_buffer, curr_line->content);
					strtok(temp_buffer, "=");
					pos = strtok(NULL, "=");
					data = atoi(pos);
					/*get name*/
					strcpy(temp_label, curr_line->content);
					strtok(temp_buffer, " ");
					pos = strtok(NULL, " ");
					name = strtok(pos, "=");
					/*add symbol*/
					addSymbol(symbol_list, name, data, MDEFINE, ABSOLUTE, 0);
				}
				set_error(curr_line, error);
			}
		
			else if(isData(curr_line->content)){/*handle data*/
				char* pos = NULL;
				
				error = error_handle_data(curr_line->content, curr_line->number, curr_line->label, *symbol_list, *line_list);
				if(error == 0){/*error*/
					
					/*get label*/
					if(hasLabel(curr_line)) {
						strcpy(temp_label, curr_line->label);
						/*add symbol*/
						addSymbol(symbol_list, temp_label, DC, DATA, RELOCATABLE, 0);
					}
				
					/*calc binary lines*/
					strcpy(temp_buffer, curr_line->content);
					pos = &temp_buffer[5];
					if(strstr(pos, ",") == NULL){/*there is only one value*/
						/*search for symbol*/
						struct Symbol *temp_symbol = searchSymbolByName(*symbol_list, pos);
						if(temp_symbol == NULL) bin_data = atoi(pos);
						else bin_data = temp_symbol->data;
						addBinaryLine(&curr_line->binary_lines, DC++, bin_data, curr_line->label);/*add binary line*/
					} else {/*there are multipul values*/
						bool first = true;
						char * iterate_data = strtok(pos, ",");
						while(iterate_data != NULL) {
							/*search for symbol*/
							struct Symbol *temp_symbol;
							if (*iterate_data == ' ') iterate_data++;
							temp_symbol = searchSymbolByName(*symbol_list, iterate_data);
							if(temp_symbol == NULL) bin_data = atoi(iterate_data);
							else bin_data = temp_symbol->data;
					
							if(first){
								addBinaryLine(&curr_line->binary_lines, DC++, bin_data, curr_line->label);/*add binary line*/
								first = false;
							} else addBinaryLine(&curr_line->binary_lines, DC++, bin_data, NULL);/*add binary line*/
							iterate_data = strtok(NULL, ",");
						}
					}
				}
				set_error(curr_line, error);
			}
		
			else if(isString(curr_line->content)){/*handle string*/
				char* pos = NULL;
				int i = 1;

				error = error_handle_string(curr_line->content, curr_line->number, curr_line->label, *symbol_list, *line_list);
				if(error == 0){/*error*/
					
					bool first;
					char *pos_end;
					
					/*get label*/
					if(hasLabel(curr_line)) {
						strcpy(temp_label, curr_line->label);
						/*add symbol*/
						addSymbol(symbol_list, temp_label, DC, DATA, RELOCATABLE, 0);
					}
					/*calc binary lines*/
					strcpy(temp_buffer, curr_line->content);
					strtok(temp_buffer, " ");
					pos = strtok(NULL, " ");
					first = true;
					pos_end = (pos + strlen(pos) - 1);
					while(isspace(*pos_end) && pos_end > pos) pos_end--;
					while((pos + i) < pos_end){
						bin_data = pos[i];
						if(first){
							addBinaryLine(&curr_line->binary_lines, DC++, bin_data, curr_line->label);/*add binary line*/
							first = false;
						} else addBinaryLine(&curr_line->binary_lines, DC++, bin_data, NULL);/*add binary line*/
						i++;
					}
					bin_data = '\0';
					addBinaryLine(&curr_line->binary_lines, DC++, bin_data, NULL);/*add closer binary line*/
				}
				set_error(curr_line, error);
			}
		
			else if(isCommand(curr_line->content)){/*handle cmd*/

				error = error_handle_cmd(curr_line->content, curr_line->number, curr_line->label, *symbol_list, *line_list);
				if(error == 0){/*error*/
					char *cmd_name = NULL, *operands = NULL, *source_operand = NULL, *target_operand = NULL;
					int leap = 1, opcode_bin = 0;
					
				
					/*get label*/
					if(hasLabel(curr_line)) {
						strcpy(temp_label, curr_line->label);
						/*add symbol*/
						addSymbol(symbol_list, temp_label, IC, CODE, RELOCATABLE, 0);
					}
					/*add binary line*/
					strcpy(temp_buffer, curr_line->content);
					cmd_name = strtok(temp_buffer, " ");
		
					if(cmdcmp(cmd_name, "hlt")) opcode_bin += HLT;
					else if(cmdcmp(cmd_name, "rts")) opcode_bin += RTS;
					else if(cmdcmp(cmd_name, "not")) opcode_bin += NOT;
					else if(cmdcmp(cmd_name, "clr")) opcode_bin += CLR;
					else if(cmdcmp(cmd_name, "inc")) opcode_bin += INC;
					else if(cmdcmp(cmd_name, "dec")) opcode_bin += DEC;
					else if(cmdcmp(cmd_name, "jmp")) opcode_bin += JMP;
					else if(cmdcmp(cmd_name, "bne")) opcode_bin += BNE;
					else if(cmdcmp(cmd_name, "red")) opcode_bin += RED;
					else if(cmdcmp(cmd_name, "prn")) opcode_bin += PRN;
					else if(cmdcmp(cmd_name, "jsr")) opcode_bin += JSR;
					else if(cmdcmp(cmd_name, "mov")) opcode_bin += MOV;
					else if(cmdcmp(cmd_name, "cmp")) opcode_bin += CMP;
					else if(cmdcmp(cmd_name, "add")) opcode_bin += ADD;
					else if(cmdcmp(cmd_name, "sub")) opcode_bin += SUB;
					else if(cmdcmp(cmd_name, "lea")) opcode_bin += LEA;
					bin_data = opcode_bin << OPCODE_BITS_SHIFT;
		

					/*clac leap and source/target adressing types*/
					operands = &temp_buffer[4];
					if(needOneOperand(cmd_name)){
						int target_addressing_type = 0;
						target_operand = operands;
						if (*target_operand == ' ') target_operand++;
						target_addressing_type = adressing_type(target_operand);
						bin_data += target_addressing_type << DEST_BITS_SHIFT;
						switch(target_addressing_type){
							case IMMIDITE_ADRESSING: leap++;  break;
							case DIRECT_ADRESSING:   leap++;  break;
							case CONSTANT_ADRESSING: leap+=2; break;
							case REGISTER_ADRESSING: leap++;  break;
						}
					}
					else if(needTwoOperands(cmd_name)){
						int source_addressing_type = 0, target_addressing_type = 0;
						source_operand = strtok(operands, ",");
						target_operand = strtok(NULL, ",");
						if (*source_operand == ' ') source_operand++;
						if (*target_operand == ' ') target_operand++;
						source_addressing_type = adressing_type(source_operand);
						target_addressing_type = adressing_type(target_operand);
						bin_data += source_addressing_type << SOURCE_BITS_SHIFT;
						bin_data += target_addressing_type << DEST_BITS_SHIFT;
						switch(source_addressing_type){
							case IMMIDITE_ADRESSING: leap++;  break;
							case DIRECT_ADRESSING:   leap++;  break;
							case CONSTANT_ADRESSING: leap+=2; break;
							case REGISTER_ADRESSING: leap++;  break;
						}
						switch(target_addressing_type){
							case IMMIDITE_ADRESSING: leap++;  break;
							case DIRECT_ADRESSING:   leap++;  break;
							case CONSTANT_ADRESSING: leap+=2; break;
							case REGISTER_ADRESSING: leap++;  break;
						}
						if(source_addressing_type == REGISTER_ADRESSING && target_addressing_type == REGISTER_ADRESSING) leap--;
					}
					addBinaryLine(&curr_line->binary_lines, IC, bin_data, curr_line->label);/*add binary line*/
					IC += leap;
				}
				set_error(curr_line, error);
			}
			else if(!isExtern(curr_line->content) && !isEntry(curr_line->content)){
				error = error_handle_undefined_line(curr_line->number);
				set_error(curr_line, error);
			}
			free(temp_buffer);
			free(temp_label);
		}
		
		curr_line = curr_line->next;
	}
	
	/*after finding all symbols, we idetefy them as entry or extern*/
	/*handle .entry */
	curr_line = *line_list;
	while (curr_line != NULL) {
		error = 0;
		if(curr_line->error == 0) {
			temp_buffer = my_malloc(BUFFER_MAX_LEN);
			temp_label = my_malloc(BUFFER_MAX_LEN);
			
			if(isEntry(curr_line->content)){/*handle entry*/
				strcpy(temp_buffer, curr_line->content);
				
				error = error_handle_entry(temp_buffer, curr_line->number, *symbol_list, *line_list);
				if(error == 0){/*error*/
					temp_name = strtok(temp_buffer, " ");
					temp_name = strtok(NULL, " ");
					setType(*symbol_list, temp_name, RELOCATABLE);
					setDefined(*symbol_list, temp_name, 1);
					resetLabel(curr_line);/* page 25: the asembler ignores the label at an entry line */
				}
				set_error(curr_line, error);
			}
			free(temp_buffer);
			free(temp_label);
		}
		curr_line = curr_line->next;
	}
	/*handle .extern */
	curr_line = *line_list;
	while (curr_line != NULL) {
		error = 0;
		if(curr_line->error == 0) {
			temp_buffer = my_malloc(BUFFER_MAX_LEN);
			temp_label = my_malloc(BUFFER_MAX_LEN);
			
			if(isExtern(curr_line->content)){/*handle extern*/
				strcpy(temp_buffer, curr_line->content);
				
				error = error_handle_extern(temp_buffer, curr_line->number, *symbol_list, *line_list);
				if(error == 0){/*error*/
					temp_name = strtok(temp_buffer, " ");
					temp_name = strtok(NULL, " ");
					/*add symbol*/
					addSymbol(symbol_list, temp_name, 0, DATA, EXTERNAL, 2);
					resetLabel(curr_line);/* page 26: the asembler ignores the label at an extern line */
				}
				set_error(curr_line, error);
			}
			free(temp_buffer);
			free(temp_label);
		}
		curr_line = curr_line->next;
	}
	addIC(*symbol_list, IC);
}


/**
purpose: compare between two cmd names

@param cmd_name
@param compareTo_name
@return true if and only if equal

*/
bool cmdcmp(char *cmd_name, char *compareTo_name){
	return (strncmp(cmd_name, compareTo_name, CMD_LEN) == 0);
}

/**
purpose: reset the label of a line

@param line
@return void

*/
void resetLabel(struct Line *line){
	free(line->label);
	line->label = my_malloc(LABEL_MAX_LEN);
}

/**
purpose: check the addresing type of the given operand

@param operand
@return int - addressing type

*/
int adressing_type(char *operand){
	if(*operand == '#') return IMMIDITE_ADRESSING;
	if(*operand == 'r') {
		operand++;
		if(is_number(operand)) {
			if(0 <= atoi(operand) && atoi(operand) <= 7) return REGISTER_ADRESSING;
		}
	}
	if(strstr(operand, "[") != NULL && strstr(operand, "]")) return CONSTANT_ADRESSING;
	return DIRECT_ADRESSING;
}


