#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H

#include "../HeaderFiles/line.h"
#include "../HeaderFiles/macro.h"

/* prototypes */
void pre_proccesor(struct Line **line_list, char *file_name);
void spread_macros(struct Line *line_list, struct Macro *macro_list);

#endif
