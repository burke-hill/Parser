#ifndef __PARSER_H
#define __PARSER_H
#include <stdio.h>
#include <stdlib.h>



typedef struct symbol
{
	int kind;
	char name[12];
	int val;
	int level;
	int addr;
	int mark;
	
} symbol;

typedef struct lexeme
{
	int kind;
	char name[12];
	int val;
	
} lexeme;

int errors[500];
int numErrors;
symbol *currentProcedure;

typedef enum {
nulsym = 1, identsym = 2, numbersym = 3, plussym = 4, minussym = 5, multsym = 6,  slashsym = 7, oddsym = 8,
eqsym = 9, neqsym = 10, lessym = 11, leqsym = 12, gtrsym = 13, geqsym = 14,
lparentsym = 15, rparentsym = 16, commasym = 17, semicolonsym = 18, periodsym = 19,
becomessym = 20, beginsym = 21, endsym = 22, ifsym = 23, thensym = 24, whilesym = 25, dosym = 26,
callsym = 27, constsym = 28, varsym = 29, procsym = 30, writesym = 31, readsym = 32, elsesym = 33, colonsym = 34
} token_type;

int numsymbols;
int cx;
int tokencount;
int input_len;
int lexPrint;
int codePrint;
int halt;
int lexlevel;
int tableIndex;
int symbolcount;

lexeme *lex_analyze(FILE *ifp); 
symbol* parse(lexeme *list);
char *tokenToString(int token);
int checkTable(symbol* table, char* name);
int backCheck(symbol *table, char* name);
void block(lexeme *list, symbol *table, int lexlevel);
int constDec(lexeme *list, symbol *table, int lexlevel);
int varDec(lexeme *list, symbol *table, int lexlevel);
int procDec(lexeme *list, symbol *table, int lexlevel);
void statement(lexeme *list, symbol *table, int lexlevel);
void condition(lexeme *list, symbol *table, int lexlevel);
void expression(lexeme *list, symbol *table, int lexlevel);
void term(lexeme *list, symbol *table, int lexlevel);
void factor(lexeme *list, symbol *table, int lexlevel);
static void addError(int errNum); 
#endif