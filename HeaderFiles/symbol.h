#ifndef SYMBOL_H
#define SYMBOL_H

#define MDEFINE 1
#define DATA 2
#define CODE 3

#define ABSOLUTE 0
#define EXTERNAL 1
#define RELOCATABLE 2

/* Structure to represent a macro */
typedef struct Symbol {
	char *name;
	int data; /* IC or value */
	int feature; /* mdefine=1, data=2, code=3 */
	int type; /*absolute=0, external=1, relocatable=2 */
	int defined; /* no=0, entry=1, extern=2;*/
	struct Symbol *next;
} Symbol;

/* prototypes */

void addSymbol(struct Symbol **head, char *name, int data, int feature, int type, int defined);
struct Symbol* createSymbol(char *name, int data, int feature, int type, int defined);

void setType(struct Symbol *symbol_list, char *name, int type);
void setDefined(struct Symbol *symbol_list, char *name, int defined);

struct Symbol* searchSymbolByName(struct Symbol *symbol_list, char *name);
void addIC(struct Symbol *symbol_list, int IC);

void printSymbols(struct Symbol *head);

void freeSymbols(struct Symbol *head);
void freeSymbol(struct Symbol *symbol);

#endif
