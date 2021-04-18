#include <stdio.h>
#include <stdlib.h>
#include "codegen.h"
#include "parser.h"


// generate code function
instruction* generate_code(lexeme *list, symbol *table)
{
	cx = 0; 
	// initialize code array to all 0s
	blockCode(list,table,0);
	emit(9,0,0,3);
	return code;
}

void blockCode(lexeme *list, symbol *table, int lexlevel)
{
	// main code block
	tokencount = 0;
	numsymbols = 0;
	int numvars = 0;
	// if token is a const
	if (list[tokencount].kind == constsym)
	{
		do
		{
			tokencount++;
			numsymbols++;
			unmark(table,1,list[tokencount].name,lexlevel);
			tokencount += 3;
		} while (list[tokencount].kind == commasym);
		tokencount++;
	}
	// if token is a variable
	if (list[tokencount].kind == varsym)
	{
		do
		{
			tokencount++;
			numvars++;
			numsymbols++;
			unmark(table,2,list[tokencount].name,lexlevel);
			tokencount++;
		} while (list[tokencount].kind == commasym);
		tokencount++;
	}
	// if token is a procedure
	if (list[tokencount].kind == procsym)
	{
		
		do
		{
			tokencount++;
			numsymbols++;
			unmark(table,3,list[tokencount].name,lexlevel);
			tokencount += 2;
			block(list,table,lexlevel+1);
			emit(2,0,0,0);
			tokencount++;
		} while (list[tokencount].kind == procsym);
	}
	table[numsymbols].addr = cx;
	emit(6,0,0,3+numvars);
	statementCode(list,table,lexlevel);
	for (int i = symbolcount; i > numsymbols; i--)
	{
		table[i].mark = 0;
	}
}


void statementCode(lexeme *list, symbol *table, int lexlevel)
{ 
	printf("yo\n");
	// statement code
	int indexNeeded;
	int jumpIndex;
	int jpcIndex;
	if (list[tokencount].kind == identsym)
	{
		// finds closest var symbol with closest lex level
		for (int i = symbolcount; i > 0; i--)
		{
			if (table[i].kind == 2 && table[i].mark == 0)
			{
				indexNeeded = i;
			}
		}
		tokencount+=2;
		expressionCode(list,table,0,lexlevel);
		emit(4,0,table[indexNeeded].level, lexlevel);
	}
	if (list[tokencount].kind == callsym)
	{
		tokencount++;
		// finds closest proc symbol with closest lex level
		for (int i = symbolcount; i > 0; i--)
		{
			if (table[i].kind == 3 && table[i].mark == 0)
			{
				indexNeeded = i;
			}
		}
		emit(5,0,table[indexNeeded].level, table[indexNeeded].addr);
		tokencount++;
	}
	// if token is begin
	if (list[tokencount].kind == beginsym)
	{
		tokencount++;
		statementCode(list,table,lexlevel);
		while (list[tokencount].kind == semicolonsym)
		{
			tokencount++;
			statementCode(list,table,lexlevel);
		}
		tokencount++;
	}
	// if token is if
	if (list[tokencount].kind == ifsym)
	{
		tokencount++;
		conditionCode(list,table,lexlevel);
		jpcIndex = cx;
		emit(8,0,0,0);
		tokencount++;
		statementCode(list,table,lexlevel);
		if (list[tokencount].kind == elsesym)
		{
			tokencount++;
			jumpIndex = cx;
			emit(7,0,0,0);
			code[jpcIndex].m = cx;
			statementCode(list,table,lexlevel);
			code[jumpIndex].m = cx;
		}
	}
	else
	{
		code[jpcIndex].m = cx;
	}
	// if token is while
	if (list[tokencount].kind == whilesym)
	{
		tokencount++;
		jpcIndex = cx;
		conditionCode(list,table,lexlevel);
		tokencount++;
		jumpIndex = cx;
		emit(8,0,0,0);
		statementCode(list,table,lexlevel);
		emit(7,0,0,jpcIndex);
		code[jumpIndex].m = cx;
	}
	// if token is read
	if (list[tokencount].kind == readsym)
	{
		tokencount++;
		for (int i = symbolcount; i > 0; i--)
		{
			if (table[i].kind == 2 && table[i].mark == 0)
			{
				indexNeeded = i;
			}
		}
		tokencount++;
		emit(9,0,0,2);
		emit(4,0,table[indexNeeded].level,table[indexNeeded].addr);
	}
	// if token is write
	if (list[tokencount].kind == writesym)
	{
		tokencount++;
		expressionCode(list,table,0,lexlevel);
		emit(9,0,0,1);
	}
}

void conditionCode(lexeme *list, symbol *table, int lexlevel)
{
	// if token is odd
	if (list[tokencount].kind == oddsym)
	{
		tokencount++;
		expressionCode(list,table,0,lexlevel);
		emit(15,0,0,0);
	}
	else
	{
		expressionCode(list,table,0,lexlevel);
		if (list[tokencount].kind == eqsym)
		{
			tokencount++;
			expressionCode(list,table,1,lexlevel);
			emit(17,0,0,1);
		}
		if (list[tokencount].kind == neqsym)
		{
			tokencount++;
			expressionCode(list,table,1,lexlevel);
			emit(18,0,0,1);
		}
		if (list[tokencount].kind == lessym)
		{
			tokencount++;
			expressionCode(list,table,1,lexlevel);
			emit(19,0,0,1);
		}
		if (list[tokencount].kind == leqsym)
		{
			tokencount++;
			expressionCode(list,table,1,lexlevel);
			emit(20,0,0,1);
		}
		if (list[tokencount].kind == gtrsym)
		{
			tokencount++;
			expressionCode(list,table,1,lexlevel);
			emit(21,0,0,1);
		}
		if (list[tokencount].kind == geqsym)
		{
			tokencount++;
			expressionCode(list,table,1,lexlevel);
			emit(22,0,0,1);
		}	
	}
	
	
}

void expressionCode(lexeme *list, symbol *table, int regToEnd, int lexlevel)
{
	// if token is plus
	if (list[tokencount].kind == plussym)
	{
		tokencount++;
	}
	// if token is minus
	if (list[tokencount].kind == minussym)
	{
		tokencount++;
		termCode(list,table,regToEnd, lexlevel);
		emit(10,regToEnd,0,0);
		while (list[tokencount].kind == plussym || list[tokencount].kind == minussym)
		{
			if (list[tokencount].kind == plussym)
			{
				tokencount++;
				termCode(list,table,regToEnd+1,lexlevel);
				emit(11,regToEnd,regToEnd,regToEnd+1);
			}
			if (list[tokencount].kind == minussym)
			{
				tokencount++;
				termCode(list,table,regToEnd+1,lexlevel);
				emit(12,regToEnd,regToEnd,regToEnd+1);
			}
			
		}
		return;
	}
	// run term function
	termCode(list,table,regToEnd,lexlevel);
	while (list[tokencount].kind == plussym || list[tokencount].kind == minussym)
	{
		if (list[tokencount].kind == plussym)
		{
			tokencount++;
			termCode(list,table,regToEnd+1,lexlevel);
			emit(11,regToEnd,regToEnd,regToEnd+1);
		}
		if (list[tokencount].kind == minussym)
		{
			tokencount++;
			termCode(list,table,regToEnd+1, lexlevel);
			emit(12,regToEnd,regToEnd,regToEnd+1);
		}
	}	
}

void termCode(lexeme *list, symbol *table, int regToEnd, int lexlevel)
{
	// main term code
	factorCode(list,table,regToEnd,lexlevel);
	while (list[tokencount].kind == multsym || list[tokencount].kind == slashsym)
	{
		if (list[tokencount].kind == multsym)
		{
			tokencount++;
			factorCode(list,table,regToEnd+1,lexlevel);
			emit(13,regToEnd,regToEnd, regToEnd+1);
		}
		if (list[tokencount].kind == slashsym)
		{
			tokencount++;
			factorCode(list,table,regToEnd+1, lexlevel);
			emit(14,regToEnd,regToEnd,regToEnd+1);
		}
	}
}

void factorCode(lexeme *list, symbol *table, int regToEnd, int lexlevel)
{
	// factor code
	int indexNeeded;
	if (list[tokencount].kind == identsym)
	{
		// find closes var or const closest
		for (int i = symbolcount; i > 0; i--)
		{
			if ((table[i].kind == 2 || table[i].kind == 1) && table[i].mark == 0)
			{
				indexNeeded = i;
			}
		}
		if (table[indexNeeded].kind == 1)
		{
			emit(1,regToEnd,0,table[indexNeeded].val);
		}
		if (table[indexNeeded].kind == 2)
		{
			emit(3,regToEnd,table[indexNeeded].level,table[indexNeeded].addr);
		}
		tokencount++;
	}
	else if (list[tokencount].kind == numbersym)
	{
		emit(1,regToEnd,0, list[tokencount].val);
		tokencount++;
	}
	else
	{
		tokencount++;
		expressionCode(list,table,regToEnd,lexlevel);
		tokencount++;
	}
	
}

// emit function
void emit(int op, int r, int l, int m)
{
	if (cx > CODE_SIZE)
	{
		//error
	}
	else
	{
		code[cx].op = op;
		code[cx].r = r;
		code[cx].l = l;
		code[cx].m = m;
		cx++;
	}
}

// unmark function
void unmark(symbol *table, int type, char* name, int lexlevel)
{
	for (int i = 0; i < symbolcount; i++)
	{
		if (strcmp(name,table[i].name) == 0 && table[i].kind == type && table[i].level == lexlevel)
		{
			table[i].mark = 0;
			currentProcedure = &table[i];
		}
	}
}