#ifndef BINARYLINE_H
#define BINARYLINE_H

#include <stdbool.h>
#include "../HeaderFiles/symbol.h"


#define OPCODE_BITS_SHIFT 6 /* number of shift left needed to reach the opcode LSB 5-8 */
#define SOURCE_BITS_SHIFT 4 /* number of shift left needed to reach the source operand LSB 9-11 */
#define DEST_BITS_SHIFT 2 /* number of shift left needed to reach the dest operand LSB 2-4 */
#define ARE_SHIFT 2
#define SOURCE_REGIESTER_SHIFT 5
#define DEST_REGIESTER_SHIFT 2

#define IMMIDITE_ADRESSING 0
#define DIRECT_ADRESSING 1
#define CONSTANT_ADRESSING 2
#define REGISTER_ADRESSING 3

#define LEN_BINARY 14
#define LEN_ENCODED 7
#define STR_ZERO_BIN "00"
#define STR_ONE_BIN "01"
#define STR_TWO_BIN "10"
#define STR_THREE_BIN "11"
#define STR_LEN_BIN_TO_ENCODE 2

#define ZERO 0
#define ONE 1
#define CHAR_ZERO '0'
#define CHAR_ONE '1'
#define CHAR_STAR '*'
#define CHAR_HASH '#'
#define CHAR_PERCENT '%'
#define CHAR_EXCLAMATION '!'



/* Structure to represent a single line of assembly code */
typedef struct BinaryLine {
	int adress;
	int data;
	char *label;
	struct BinaryLine *next;
} BinaryLine;

/* prototypes */


void addBinaryLines(struct BinaryLine **head, struct BinaryLine *pos);
void addBinaryLine(struct BinaryLine **head, int adress, int data, char *label);
struct BinaryLine* createBinaryLine(int adress, int data, char *label);

void printBinaryLines(struct BinaryLine *head);
void fprintEncodedLines(struct BinaryLine *head, char *file_name);

void fprintENTLines(struct Symbol *symbol_list, struct BinaryLine *binary_line_list, char *file_name);
void fprintEXTLines(struct Symbol *symbol_list, struct BinaryLine *binary_line_list, char *file_name);

char* int_to_bin(int num);
char* bin_to_encode(char *bin);
void reverse_str(char *str);

void fixAdresses(struct BinaryLine *head);

void freeBinaryLines(struct BinaryLine *head);
void freeBinaryLine(struct BinaryLine *binary_line);



#endif
