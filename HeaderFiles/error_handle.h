#ifndef ERROR_HENDLE_H
#define ERROR_HENDLE_H


#define MSG_ARGC(MSG) 		 "\nError in Usage: %s <assembly_file_1> <assembly_file_2> ...", MSG
#define MSG_LINE_SIZE(NUM)       "\nError in line %d: Excceded max size of 80 letters", NUM
#define MSG_LINE_UNDEFINED(NUM)  "\nError in line %d: Undefined line structure", NUM

#define MSG_LABEL_SIZE(NUM)      "\nError in line %d: label excceded max size of 32 letters", NUM
#define MSG_LABEL_NONE(NUM)      "\nError in line %d: label expected", NUM
#define MSG_LABEL_UPPER(NUM)     "\nError in line %d: label can only start with an alphabetic letter", NUM
#define MSG_LABEL_REST(NUM)      "\nError in line %d: label can be letters and digits only", NUM
#define MSG_LABEL_HAS_SPACE(NUM) "\nError in line %d: Illigal space between the label name and : sign", NUM
#define MSG_LABEL_SAVED(NUM)     "\nError in line %d: label cant be a saved name in assembly", NUM

#define MSG_FILE_OPEN(NAME)      "\nError in file %s: Unable to open", NAME
#define MSG_FILE_EMPTY(NAME)     "\nErorr in file %s: Empty", NAME
#define MSG_FILE_REMOVE(NAME)	 "\nErorr in file %s: Unable to remove", NAME

#define MSG_MALLOC 		 "\nError: Memory allocation failed"

#define MSG_DEFINE_USAGE(NUM) 	 "\nError in line %d: define Usage: .define NAME = NUMBER", NUM
#define MSG_DEFINE_NUMBER_RANGE(NUM) 	"\nError in line %d: define number must be in range [-2048, 2047]", NUM

#define MSG_DEFINE_LABEL_SIZE(NUM)      "\nError in line %d: define label excceded max size of 32 letters", NUM
#define MSG_DEFINE_LABEL_NONE(NUM)      "\nError in line %d: define Expected label", NUM
#define MSG_DEFINE_LABEL_UPPER(NUM)     "\nError in line %d: define label needs to start in an alphabetic letter", NUM
#define MSG_DEFINE_LABEL_REST(NUM)      "\nError in line %d: define label needs to be only letters and digits", NUM
#define MSG_DEFINE_LABEL_SAVED(NUM)     "\nError in line %d: define label is a saved name in assembly", NUM

#define MSG_DATA_EMPTY(NUM)		"\nError in line %d: data has no values to store", NUM
#define MSG_DATA_NUMBERS_ONLY(NUM) 	"\nError in line %d: data can store integers only", NUM
#define MSG_DATA_NUMBER_RANGE(NUM) 	"\nError in line %d: data number must be in range [-8192, 8191]", NUM 
#define MSG_DATA_COMMA_START(NUM) 	"\nError in line %d: data cant start with comma", NUM 
#define MSG_DATA_COMMA_CONSEC(NUM) 	"\nError in line %d: data has consecutive commas", NUM
#define MSG_DATA_COMMA_END(NUM) 	"\nError in line %d: data cant end with comma", NUM 

#define MSG_STRING_NO_VALUE(NUM) 	"\nError in line %d: string has no value to store", NUM 
#define MSG_STRING_COMMA_START(NUM) 	"\nError in line %d: string has no starting comma", NUM 
#define MSG_STRING_COMMA_END(NUM) 	"\nError in line %d: string has no ending comma", NUM 

#define MSG_CMD_NO_VALUE(NUM) 		"\nError in line %d: command has no ", NUM 
#define MSG_CMD_NAME_UNDEFINED(NUM) 	"\nError in line %d: command name undefined", NUM 
#define MSG_CMD_EXTORENDOS_TEXT(NUM) 	"\nError in line %d: command has extorendos text", NUM 
#define MSG_CMD_MISSING_OPERAND(NUM) 	"\nError in line %d: command missing one operand", NUM 
#define MSG_CMD_MISSING_OPERANDS(NUM) 	"\nError in line %d: command missing two operands", NUM 
#define MSG_CMD_ONLY_ONE(NUM) 		"\nError in line %d: command gets only one operand (Illigal comma)", NUM 
#define MSG_CMD_INVALID_OPERAND(NUM) 	"\nError in line %d: command has invalid operand", NUM 

#define MSG_CMD_IMMIDIATE_OPERAND_NUMBER(NUM) 	"\nError in line %d: command has invalid immidiate addressing", NUM 
#define MSG_CMD_IMMIDIATE_OPERAND_RANGE(NUM) 	"\nError in line %d: command immidiate addressing must be in range [-8192, 8191]", NUM
 
#define MSG_CMD_DIRECT_OPERAND_NUMBER(NUM) 	"\nError in line %d: command has invalid direct addressing", NUM 
#define MSG_CMD_DIRECT_OPERAND_RANGE(NUM) 	"\nError in line %d: command direct addressing has r0-r7 registers only", NUM 

#define MSG_ENTRY_EMPTY(NUM) 			"\nError in line %d: entry has no label", NUM 
#define MSG_ENTRY_DONT_EXIST(NUM) 		"\nError in line %d: entry declaring label that dont exist in file", NUM 

#define MSG_EXTERN_SIZE(NUM)      "\nError in line %d: extern label excceded max size of 32 letters", NUM
#define MSG_EXTERN_NONE(NUM)      "\nError in line %d: extern expected a label", NUM
#define MSG_EXTERN_UPPER(NUM)     "\nError in line %d: extern label needs to start in an alphabetic letter", NUM
#define MSG_EXTERN_REST(NUM)      "\nError in line %d: extern label can be letters and digits only", NUM
#define MSG_EXTERN_SAVED(NUM)     "\nError in line %d: extern label is a saved name in assembly", NUM

#define MSG_CMD_INVALID_TARGET_ADDRESSING_TYPE(NUM)     "\nError in line %d: Invalid adressing type for target operand", NUM
#define MSG_CMD_INVALID_SOURCE_ADDRESSING_TYPE(NUM)     "\nError in line %d: Invalid adressing type for source operand", NUM 
#define MSG_CMD_INVALID_TARGET_OPERAND(NUM)    	       "\nError in line %d: Invalid target operand", NUM
#define MSG_CMD_INVALID_SOURCE_OPERAND(NUM)    	       "\nError in line %d: Invalid source operand", NUM

#define MSG_DUPLICATE_SYMBOLS   	       "\nFATAL ERROR: dulicate names of labels found in code"
#define MSG_EXTERN_IN_FILE   	       		"\nFATAL ERROR: .extern defined a label the exist in file"

#include "../HeaderFiles/line.h"
#include "../HeaderFiles/macro.h"
#include "../HeaderFiles/binary_line.h"


/* prototypes */
void raiseFlag(int buf);

void error_handle_has_errors(char *file_name);

void error_handle_args(int argc, char *argv[]);

int error_handle_line_size(char *line_buffer, int number);
int error_handle_undefined_line(int number);
int error_handle_label(char *label_buffer, int number);
int error_handle_label_expected(int number);

void error_handle_file_open(FILE *file, char *name);
void error_handle_file_empty(struct Line *head, char *file_name);
void error_handle_file_remove(int status, char *file_name);

void error_handle_malloc_char(char *dup);
void error_handle_malloc_line(struct Line *line);
void error_handle_malloc_macro(struct Macro *macro);
void error_handle_malloc_binary_line(struct BinaryLine *binary_line);

int error_handle_define(char *buf, int line_number);
int error_handle_data(char *buf, int line_number, char *label, struct Symbol *symbol_list, struct Line *line_list);
int error_handle_string(char *buf, int line_number, char *label, struct Symbol *symbol_list, struct Line *line_list);
int error_handle_cmd(char *buf, int line_number, char *label, struct Symbol *symbol_list, struct Line *line_list);
int error_handle_cmd_second_pass(struct Line *curr_line, struct Symbol *symbol_list);

int error_handle_entry(char *buf, int line_number, struct Symbol *symbol_list, struct Line *line_list);
int error_handle_extern(char *buf, int line_number, struct Symbol *symbol_list, struct Line *line_list);

int error_handle_add_symbol(struct Line *line_list, struct Symbol *symbol_list, char *name);


#endif
