#ifndef MACRO_H
#define MACRO_H

#include "../HeaderFiles/line.h"

/* Structure to represent a macro */
typedef struct Macro {
	char *name;
	Line *lines;
	struct Macro *next;
} Macro;

/* prototypes */
struct Macro* process_macros(struct Line *line_list);
void extract_macros(struct Line **line_list);

void addMacro(struct Macro **head, char *name, struct Line *lines);
struct Macro* createMacro(char *name, struct Line *lines);

struct Macro* searchMacroByName(struct Macro *macro_list, char *name);

void printMacros(struct Macro *head);
void printMacro(struct Macro *head);

void freeMacros(struct Macro *head);
void freeMacro(struct Macro *head);

#endif
