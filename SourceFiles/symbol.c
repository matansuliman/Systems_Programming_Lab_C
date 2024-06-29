#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../HeaderFiles/symbol.h"
#include "../HeaderFiles/line.h"
#include "../HeaderFiles/utils.h"

/**
purpose: adding a symbol to the end of the list

@param symbol list
@param name, data, feature, type, defeined
@return void

*/
void addSymbol(struct Symbol **head, char *name, int data, int feature, int type, int defined) {
	struct Symbol *new_symbol = createSymbol(name, data, feature, type, defined);
	struct Symbol *temp = *head;
	if (*head == NULL) {
		*head = new_symbol;
		return;
	}
	while (temp->next != NULL) temp = temp->next;
	temp->next = new_symbol;
}

/**
purpose: malloc a symbol

@param name, data, feature, type, defeined
@return symbol

*/
struct Symbol* createSymbol(char *name, int data, int feature, int type, int defined) {
	struct Symbol *new_symbol = (struct Symbol*)malloc(sizeof(struct Symbol));
	if (new_symbol == NULL) {
		printf("Memory allocation failed\n");
		exit(EXIT_FAILURE);
	}
	new_symbol->name = my_strdup(name, BUFFER_MAX_LEN);
	new_symbol->data = data;
	new_symbol->feature = feature;
	new_symbol->type = type;
	new_symbol->defined = defined;
	new_symbol->next = NULL;
	return new_symbol;
}

/**
purpose: set the type of a symbol

@param symbol list
@param name
@param type
@return void

*/
void setType(struct Symbol *symbol_list, char *name, int type) {
	struct Symbol *temp = searchSymbolByName(symbol_list, name);
	if(temp != NULL) temp->type = type;
}

/**
purpose: set the define of a symbol

@param symbol list
@param name
@param type
@return void

*/
void setDefined(struct Symbol *symbol_list, char *name, int defined) {
	struct Symbol *temp = searchSymbolByName(symbol_list, name);
	if(temp != NULL) temp->defined = defined;
}


/**
purpose: search a symbol from a list of symbols by his name

@param symbol list
@param name
@return symbol

*/
struct Symbol* searchSymbolByName(struct Symbol *symbol_list, char *name) {
	struct Symbol *temp = symbol_list;
	if (strlen(name) == 0) return NULL;
	while(temp != NULL){
		if(strncmp(temp->name, name, strlen(temp->name)) == 0) return temp;
		temp = temp->next;
	}
	return temp;
}

/**
purpose: add the IC to all symbols that are in the data segment of the file
1)feature is data
2)type is not external

@param symbol list
@param IC - instruction count
@return void

*/
void addIC(struct Symbol *symbol_list, int IC) {
	while(symbol_list != NULL){
		if(symbol_list->feature == DATA && symbol_list->type != EXTERNAL) symbol_list->data += IC;
		symbol_list = symbol_list->next;
	}
}

/**
purpose: print all symbols to stdout in thr format 'name:feature:type:defined:data'

@param symbol list
@return void

*/
void printSymbols(struct Symbol *head) {
	char *feature = NULL, *type = NULL, *defined = NULL;
	
	printf("\nSymbols Table:");
	printf("\n\t-------------------------------------------------------------------------");
	
	while (head != NULL) {
		feature = my_malloc(BUFFER_MAX_LEN);
		type = my_malloc(BUFFER_MAX_LEN);
		defined = my_malloc(BUFFER_MAX_LEN);
		
		switch(head->feature){
			case MDEFINE: strcpy(feature, "mdefine"); break;
			case DATA: strcpy(feature, "data"); break;
			case CODE: strcpy(feature, "code"); break;
			default: printf("\nERROR\n");
		}
		switch(head->type){
			case ABSOLUTE: strcpy(type, "absolute"); break;
			case EXTERNAL: strcpy(type, "external"); break;
			case RELOCATABLE: strcpy(type, "relocatable"); break;
			default: printf("\nERROR\n");
		}
		switch(head->defined){
			case 0: strcpy(defined, "no"); break;
			case 1: strcpy(defined, "entry"); break;
			case 2: strcpy(defined, "extern"); break;
			default: printf("\nERROR\n");
		}
		printf("\n\t|  name:%-7s  |  feature:%-7s  |  type:%-11s  |  defined:%-11s  | data:%-5d  |", head->name, feature, type, defined, head->data);
		
		free(feature);
		free(type);
		free(defined);
		head = head->next;
	}
	printf("\n\t-------------------------------------------------------------------------");
}

/**
purpose: free symbol list from memmory

@param symbol list
@return void

*/
void freeSymbols(struct Symbol *head) {
	struct Symbol *temp;
	while (head != NULL) {
		temp = head;
		head = head->next;
		freeSymbol(temp);
	}
}

/**
purpose: free symbol from memmory

@param symbol
@return void

*/
void freeSymbol(struct Symbol *symbol) {
	free(symbol->name);
	free(symbol);
}



