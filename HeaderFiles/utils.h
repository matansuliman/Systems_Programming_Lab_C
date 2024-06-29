#ifndef UTILS_H
#define UTILS_H

#include "../HeaderFiles/line.h"

#define BUFFER_MAX_LEN 250

#define WRITE "w"
#define READ "r"

#define EXTENTION_AM "am"
#define EXTENTION_OB "ob"
#define EXTENTION_ENT "ent"
#define EXTENTION_EXT "ext"

#define MCR_LEN 3
#define END_MCR_LEN 6

#define CMD_LEN 3
#define NUMBER_OF_TOTAL_INSTRUCTIONS 16
#define NUMBER_OF_ZERO_OPERAND_INSTRUCTIONS 2
#define NUMBER_OF_ONE_OPERAND_INSTRUCTIONS 9
#define NUMBER_OF_TWO_OPERAND_INSTRUCTIONS 5

/* prototypes */
FILE* my_fopen(char *file_name, char *type);
void my_remove(char *file_name);

char* my_strdup(const char *src, int size);
char* my_malloc(int size);

char *change_extension(char *input_file_name, char *new_extension);


bool isDefine(char *buf);
bool hasLabel(struct Line *line);
bool isData(char *buf);
bool isString(char *buf);
bool isEntry(char *buf);
bool isExtern(char *buf);
bool isCommand(char *buf);

bool isMCR(char *buf);
bool isENDMCR(char *buf);

bool needZeroOperands(char *cmd_name);
bool needOneOperand(char *cmd_name);
bool needTwoOperands(char *cmd_name);

int is_saved_word(char* name);

int is_number(char *buf);
int is_valid_define_number(int num);
int is_valid_data_number(int num);

int is_valid_operand(char *buf, int line_number, struct Symbol *symbol_list);
int is_valid_immidiate_addressing(char *buf, int line_number);
int is_valid_direct_addressing(char *buf, int line_number, struct Symbol *symbol_list);
int is_valid_constant_addressing(char *buf, int line_number, struct Symbol *symbol_list);
int is_valid_register_addressing(char *buf, int line_number);

#endif
