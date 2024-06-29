#ifndef CLEANFILE_H
#define CLEANFILE_H

#include <stdbool.h>
#include "../HeaderFiles/line.h"

/* prototypes */
void cleanFile(struct Line **line_list);

void cleanBeginings(struct Line *line);
void cleanEnds(struct Line *line);
bool asciiCharacter(char c);

void cleanByType(struct Line **line_list, bool (*func)(struct Line *line));
bool isEmptyLine(struct Line *line);
bool isCommentLine(struct Line *line);

void convertTabToSpace(struct Line *line);
void convertManySpacesToOne(struct Line *line_list);

char *skip_spaces(char *s);

#endif
