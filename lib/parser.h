#ifndef PARSER_H_INCLUDED
#define	PARSER_H_INCLUDED

#include "../config.h"

char *openINI(char *inipath);
CONFIG *parseINI();

typedef struct parser_rules {
	char comment;
	char end_of_line;
	char equals;
	char end_of_assigment;
} RULES;

RULES loadRules_ini();


#endif