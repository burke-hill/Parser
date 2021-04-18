#ifndef __CODEGEN_H
#define __CODEGEN_H

#include "parser.h"
#include <string.h>

#define CODE_SIZE 500


typedef struct instruction
{
	int op;
	int r;
	int l;
	int m;
	
} instruction;

instruction code[CODE_SIZE];
void emit(int op, int r, int l, int m);
void blockCode(lexeme *list, symbol *table, int lexlevel);
void statementCode(lexeme *list, symbol *table, int lexlevel);
void conditionCode(lexeme *list, symbol *table, int lexlevel);
void expressionCode(lexeme *list,symbol *table, int regToEnd, int lexlevel);
void termCode(lexeme *list,symbol *table, int regToEnd, int lexlevel);
void factorCode(lexeme *list,symbol *table, int regToEnd, int lexlevel);
instruction* generate_code(lexeme *list, symbol *table);
void unmark(symbol *table, int type, char *name, int lexlevel);

#endif