#ifndef LINE_H
#define LINE_H

#define LINE_MAX_LEN 81
#define LABEL_MAX_LEN 32 /* 31 plus the '\0' */


#include "../HeaderFiles/binary_line.h"
#include "../HeaderFiles/symbol.h"

/* 
Structure to represent a single line of assembly code
each line has a number - index
label - optional
content - cmd <operand> <operand>
binary lines - machine code
next - thr next line
 */
typedef struct Line {
	int number;
	int error;
	char *label;
	char *content;
	struct BinaryLine *binary_lines;
	struct Line *next;
} Line;

/* prototypes */
Line* process_lines(char *file_name);
void proccess_labels(struct Line *line_list);
void numberTheLines(struct Line *line_list);

void addLine(struct Line **line_list, char *line_buffer, int number, int error);
struct Line* createLine(char *line_buffer, int number, int error);

void set_error_1_all(struct Line *line_list);
void set_error(struct Line *line, int status);

void printLines(struct Line *line_list);
void printLinesAndBinaryLines(struct Line *line_list);
void printOnlyBinaryLines(struct Line *line_list);
void fprintLines(struct Line *line_list, char *file_name);

/*void fixAdresses(struct Line *line_list, struct Symbol *symbol_list);*/

void freeLines(struct Line *line_list);
void freeLine(struct Line *line);

#endif
