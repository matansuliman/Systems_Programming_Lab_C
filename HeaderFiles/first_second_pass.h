#ifndef FIRSTPASS_H
#define FIRSTPASS_H

#include "../HeaderFiles/line.h"
#include "../HeaderFiles/symbol.h"


typedef enum CMD_VALUES {
	MOV = 0,
	CMP,
	ADD,
	SUB,
	NOT,
	CLR,
	LEA,
	INC,
	DEC,
	JMP,
	BNE,
	RED,
	PRN,
	JSR,
	RTS,
	HLT
} CMD_VALUES;

/* prototypes */
void first_second_pass(struct Line **line_list, char *file_name);
void firstPass(struct Line **line_list, char *file_name, struct Symbol **symbol_list);
void secondPass(struct Line **line_list, char *file_name, struct Symbol **symbol_list);
void entext(struct Symbol *symbol_list, struct BinaryLine *binary_line_list, char *file_name);


bool cmdcmp(char *cmd, char *compareTo);

void resetLabel(struct Line *line);

int adressing_type(char *operand);

void fprintMachineCode(struct Line *line_list, struct Symbol *symbol_list, char *file_name);


#endif
