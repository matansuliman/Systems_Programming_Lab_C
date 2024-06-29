#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../HeaderFiles/binary_line.h"
#include "../HeaderFiles/error_handle.h"
#include "../HeaderFiles/symbol.h"
#include "../HeaderFiles/utils.h"

/**
purpose: adding a binary line list to the end of the list

@param bianry line list
@param bianry line
@return void

*/
void addBinaryLines(struct BinaryLine **head, struct BinaryLine *pos){
	while (pos != NULL) {
		addBinaryLine(head, pos->adress, pos->data, pos->label);
		pos = pos->next;
	}
}

/**
purpose: adding a binary line list to the end of the list

@param bianry line list
@param adress
@param data
@param label
@return void

*/
void addBinaryLine(struct BinaryLine **head, int adress, int data, char *label){
	struct BinaryLine *new_binary_line = createBinaryLine(adress, data, label);
	struct BinaryLine *temp = *head;
	if (*head == NULL) {
		*head = new_binary_line;
		return;
	}
	while (temp->next != NULL) temp = temp->next;
	temp->next = new_binary_line;
}

/**
purpose: malloc a bianry line

@param address
@param data
@param label 
@return bianry line

*/
struct BinaryLine* createBinaryLine(int adress, int data, char *label) {
	struct BinaryLine *new_binary_line = (struct BinaryLine*)malloc(sizeof(struct BinaryLine));
	error_handle_malloc_binary_line(new_binary_line);
	new_binary_line->adress = adress;
	new_binary_line->data = data;
	new_binary_line->label = my_strdup(label, BUFFER_MAX_LEN);
	new_binary_line->next = NULL;
	return new_binary_line;
}

/**
purpose: print all bianry lines to stdout in the format 'label:address:data'

@param bianry line list
@return void

*/
void printBinaryLines(struct BinaryLine *head) {
	char *bin = NULL;
	while (head != NULL) {
		bin = int_to_bin(head->data);
		printf("\n\tlabel:%-10s | adress:%-3d %s ", head->label, head->adress, bin);
		free(bin);
		head = head->next;
	}
}


/**
purpose:  create .ob file

@param bianry line list
@param file name
@return void

*/
void fprintEncodedLines(struct BinaryLine *head, char *file_name) {
	FILE *file = my_fopen(file_name, "w");
	char *bin = NULL, *encode = NULL;
	while (head != NULL) {
		bin = int_to_bin(head->data);
		encode = bin_to_encode(bin);
		fprintf(file, "%4d\t%s\n ", head->adress, encode);
		free(bin);
		free(encode);
		head = head->next;
	}
	fclose(file);
}

/**
purpose: create .ent file

@param symbol list
@param bianry line list
@param file name
@return void

*/
void fprintENTLines(struct Symbol *symbol_list, struct BinaryLine *binary_line_list, char *file_name) {
	
	FILE *file = my_fopen(file_name, "w");
	struct BinaryLine *curr_binary_line = NULL;
	int empty = true;
	while (symbol_list != NULL) {
		if(symbol_list->defined == 1){
			curr_binary_line = binary_line_list;
			while (curr_binary_line != NULL) {
				if(strlen(curr_binary_line->label) != 0){
					if(strncmp(curr_binary_line->label, symbol_list->name, strlen(symbol_list->name)) == 0){
						fprintf(file, "%s\t%-4d\n", symbol_list->name, curr_binary_line->adress);
						empty = false;
					}
				}
				curr_binary_line = curr_binary_line->next;
			}
		}
		symbol_list = symbol_list->next;
	}
	fclose(file);
	if(empty) my_remove(file_name);
}

/**
purpose: create .ext file

@param symbol list
@param bianry line list
@param file name
@return void

*/
void fprintEXTLines(struct Symbol *symbol_list, struct BinaryLine *binary_line_list, char *file_name) {
	FILE *file = my_fopen(file_name, "w");
	struct BinaryLine *curr_binary_line = NULL;
	int empty = true;
	while (symbol_list != NULL) {
		if(symbol_list->defined == 2){
			curr_binary_line = binary_line_list;
			while (curr_binary_line != NULL) {
				if(strlen(curr_binary_line->label) != 0){
					if(strncmp(curr_binary_line->label, symbol_list->name, strlen(symbol_list->name)) == 0){
						fprintf(file, "%s\t%-4d\n", symbol_list->name, curr_binary_line->adress);
						empty = false;
					}
				}
				curr_binary_line = curr_binary_line->next;
			}
		}
		symbol_list = symbol_list->next;
	}
	fclose(file);
	if(empty) my_remove(file_name);
}

/**
purpose: translate decimal number into binary in complements 2

@param string bin data
@return string encoded

*/
char* bin_to_encode(char *bin){
	char *pos, *dup = my_malloc(LEN_ENCODED + 1);	
	int i;
	for(i = 0; i < LEN_ENCODED; i++){
		pos = bin + (2*i);
		if(strncmp(pos, STR_ZERO_BIN,  STR_LEN_BIN_TO_ENCODE) == 0) dup[i] = CHAR_STAR;
		if(strncmp(pos, STR_ONE_BIN,   STR_LEN_BIN_TO_ENCODE) == 0) dup[i] = CHAR_HASH;
		if(strncmp(pos, STR_TWO_BIN,   STR_LEN_BIN_TO_ENCODE) == 0) dup[i] = CHAR_PERCENT;
		if(strncmp(pos, STR_THREE_BIN, STR_LEN_BIN_TO_ENCODE) == 0) dup[i] = CHAR_EXCLAMATION;
	}
	return dup;
}

/**
purpose: translate integer decimal number into binary in complements 2

@param string integer decimal
@return string binary

*/
char* int_to_bin(int num){
	char *bin = my_malloc(LEN_BINARY + 1);
	int i = 0, j = 0;
	bool isNeg = (num < 0);
	num = abs(num);
	while (num) {
		if (num & ONE) bin[i] = CHAR_ONE;
		else bin[i] = CHAR_ZERO;
		i++;
		num >>= ONE;
	}
	for(j = i; j < LEN_BINARY; j++) bin[j] = CHAR_ZERO;
	if(isNeg){
		int carry;
		for(i = 0; i < LEN_BINARY; i++){
			if(bin[i] == CHAR_ZERO) bin[i] = CHAR_ONE;
			else bin[i] = CHAR_ZERO;
		}
		carry = ONE;
		for(i = 0; i < LEN_BINARY; i++){
			if(carry == ZERO) break;
			else if(bin[i] == CHAR_ZERO){
				bin[i] = CHAR_ONE;
				break;
			}
			else if(bin[i] == CHAR_ONE) bin[i] = CHAR_ZERO;
		}
	}
	reverse_str(bin);
	return bin;
}

/**
purpose: reverse a string in place

@param source
@return void

*/
void reverse_str(char *str){
	int i, length = strlen(str);
	char temp;
	for (i = 0; i < length / 2; i++) {
		temp = str[i];
		str[i] = str[length - i - 1];
		str[length - i - 1] = temp;
	}
}

/**
purpose: number the address of the binary lines

@param binary line list
@return void

*/
void fixAdresses(struct BinaryLine *head){
	int num;
	if (head == NULL) return;
	num = head->adress;
	while (head != NULL) {
		head->adress = num++;
		head = head->next;
	}
}

/**
purpose: free bianry line list from memmory

@param bianry line list
@return void

*/
void freeBinaryLines(struct BinaryLine *head) {
	struct BinaryLine *temp;
	while (head != NULL) {
		temp = head;
		head = head->next;
		freeBinaryLine(temp);
	}
}

/**
purpose: free bianry line from memmory

@param bianry line
@return void

*/
void freeBinaryLine(struct BinaryLine *binary_line) {
	free(binary_line->label);
	free(binary_line);
}

