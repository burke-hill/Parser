#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
//#include "codegen.h"
//#include "vm.h"

// parses list and makes symbol table
symbol* parse(lexeme *list)
{
	symbolcount = 0;
	tokencount = 0;
	tableIndex = 0;
	symbol *table = malloc(500 * sizeof(symbol));
	table[symbolcount].kind = 3;
	strcpy(table[symbolcount].name,"main");
	table[symbolcount].val = 0;
	table[symbolcount].level = 0;
	table[symbolcount].addr = 0;
	table[symbolcount].mark = 0;
	symbolcount++;
	block(list,table,0);
	if (list[tokencount].kind != periodsym)
	{
		addError(9);
	}
	return table;
}

// block function
void block(lexeme *list, symbol *table, int lexlevel)
{
	int numsymbols = 0;
	numsymbols += constDec(list,table,lexlevel);
	numsymbols += varDec(list,table,lexlevel);
	numsymbols += procDec(list,table,lexlevel+1);
	statement(list,table,lexlevel);
	for (int i = (symbolcount-numsymbols); i < symbolcount; i++)
	{
		table[i].mark = 1;
	}
}

int constDec(lexeme *list, symbol *table, int lexlevel)
{
	int numconsts;
	char name[12];
	if (list[tokencount].kind == constsym)
	{
		do
		{
			numconsts++;
			tokencount++;
			if (list[tokencount].kind != identsym)
			{
				addError(4);
			}
			strcpy(name,list[tokencount].name);
			if (checkTable(table,name) && table[tableIndex].level == lexlevel && table[tableIndex].mark == 0)
			{
				addError(24);
			}
			tokencount++;
			if (list[tokencount].kind != eqsym)
			{
				addError(3);
			}
			tokencount++;
			if (list[tokencount].kind != numbersym)
			{
				addError(2);
			}
			table[symbolcount].kind = 1;
			strcpy(table[symbolcount].name,name);
			table[symbolcount].val = list[tokencount].val;
			table[symbolcount].level = lexlevel;
			table[symbolcount].addr = 0;
			table[symbolcount].mark = 0;
			symbolcount++;
			tokencount++;
		} while (list[tokencount].kind == commasym);
		if (list[tokencount].kind != semicolonsym)
		{
			addError(12);
		}
		tokencount++;
	}
	return numconsts;
}

int varDec(lexeme *list, symbol *table, int lexlevel)
{
	int numvars;
	if (list[tokencount].kind == varsym)
	{
		numvars = 0;
		do
		{
			numvars++;
			tokencount++;
			if (list[tokencount].kind != identsym)
			{
				addError(4);
			}
			if (checkTable(table,list[tokencount].name) && table[tableIndex].level == lexlevel && table[tableIndex].mark == 0)
			{
				addError(24);
			}
			table[symbolcount].kind = 2;
			strcpy(table[symbolcount].name,list[tokencount].name);
			table[symbolcount].val = 0;
			table[symbolcount].level = lexlevel;
			table[symbolcount].addr = numvars+2;
			table[symbolcount].mark = 0;
			symbolcount++;
			tokencount++;
		} while (list[tokencount].kind == commasym);
		if (list[tokencount].kind != semicolonsym)
		{
			addError(10);
		}
		tokencount++;
	}
	return numvars;
}

int procDec(lexeme *list, symbol *table, int lexlevel)
{
	int numprocs;
	if (list[tokencount].kind == procsym)
	{
		numprocs = 0;
		do
		{
			numprocs++;
			tokencount++;
			if (list[tokencount].kind != identsym)
			{
				addError(4);
			}
			if (checkTable(table,list[tokencount].name) && table[tableIndex].level == lexlevel && table[tableIndex].mark == 0)
			{
				addError(24);
			}
			table[symbolcount].kind = 3;
			strcpy(table[symbolcount].name,list[tokencount].name);
			table[symbolcount].val = 0;
			table[symbolcount].level = lexlevel;
			table[symbolcount].addr = 0;
			table[symbolcount].mark = 0;
			tokencount++;
			symbolcount++;
			if (list[tokencount].kind != semicolonsym)
			{
				addError(10);
			}
			tokencount++;
			block(list,table,lexlevel);
			if (list[tokencount].kind != semicolonsym)
			{
				addError(10);
			}
			tokencount++;
		} while (list[tokencount].kind == procsym);		
	}
	return numprocs;
}

void statement(lexeme *list, symbol *table, int lexlevel)
{
	if (list[tokencount].kind == identsym)
	{
		if (backCheck(table,list[tokencount].name) && table[tableIndex].mark == 0 && table[tableIndex].kind == 2)
		{
			//good
		}
		else
		{
			addError(24);
		}
		tokencount++;
		if (list[tokencount].kind != becomessym)
		{
			addError(3);
		}
		tokencount++;
		expression(list,table,lexlevel);
		return;
	}
	if (list[tokencount].kind == callsym)
	{
		tokencount++;
		if (backCheck(table,list[tokencount].name) && table[tableIndex].mark == 0 && table[tableIndex].kind == 3)
		{
			//good
		}
		else
		{
			addError(24);
		}
		tokencount++;
		return;
	}
	if (list[tokencount].kind == beginsym)
	{
		tokencount++;
		statement(list,table,lexlevel);
		while (list[tokencount].kind == semicolonsym)
		{
			tokencount++;
			statement(list,table,lexlevel);
		}
		if (list[tokencount].kind != endsym)
		{
			addError(17);
		}
		tokencount++;
		return;
	}
	if (list[tokencount].kind == ifsym)
	{
		tokencount++;
		condition(list,table,lexlevel);
		if (list[tokencount].kind != thensym)
		{
			addError(16);
		}
		tokencount++;
		statement(list,table,lexlevel);
		if (list[tokencount].kind == elsesym)
		{
			tokencount++;
			statement(list,table,lexlevel);
		}
		return;
	}
	if (list[tokencount].kind == whilesym)
	{
		tokencount++;
		condition(list,table,lexlevel);
		if (list[tokencount].kind != dosym)
		{
			addError(18);
		}
		tokencount++;
		statement(list,table,lexlevel);
		return;
	}
	if (list[tokencount].kind == readsym)
	{
		tokencount++;
		if (backCheck(table,list[tokencount].name) && table[tableIndex].mark == 0 && table[tableIndex].kind == 2)
		{
			//good
		}
		else
		{
			addError(24);
		}
		tokencount++;
		return;
	}
	if (list[tokencount].kind == writesym)
	{
		tokencount++;
		expression(list,table,lexlevel);
		return;
	}
	return;
}

void condition(lexeme *list, symbol *table, int lexlevel)
{
	if (list[tokencount].kind == oddsym)
	{
		tokencount++;
		expression(list,table,lexlevel);
	}
	else
	{
		expression(list,table,lexlevel);
		if (list[tokencount].kind != eqsym || list[tokencount].kind != neqsym || list[tokencount].kind != lessym
		|| list[tokencount].kind != leqsym || list[tokencount].kind != gtrsym || list[tokencount].kind != geqsym)
		{
			addError(23);
		}
		tokencount++;
		expression(list,table,lexlevel);
	}
}

void expression(lexeme *list, symbol *table, int lexlevel)
{
	if (list[tokencount].kind == plussym || list[tokencount].kind == minussym)
	{
		tokencount++;
	}
	term(list,table,lexlevel);
	while (list[tokencount].kind == plussym || list[tokencount].kind == minussym)
	{
		tokencount++;
		term(list,table,lexlevel);
	}
}

void term(lexeme *list, symbol *table, int lexlevel)
{
	factor(list,table,lexlevel);
	while (list[tokencount].kind == multsym || list[tokencount].kind == slashsym)
	{
		tokencount++;
		factor(list,table,lexlevel);
	}
}

void factor (lexeme *list, symbol *table, int lexlevel)
{
	if (list[tokencount].kind == identsym)
	{
		if (backCheck(table,list[tokencount].name) && table[tableIndex].mark == 0 && (table[tableIndex].kind == 2 || table[tableIndex].kind == 1))
		{
			//good
		}
		else
		{
			addError(24);
		}
		tokencount++;
	}
	else if (list[tokencount].kind == numbersym)
	{
		tokencount++;
	}
	else if (list[tokencount].kind == lparentsym)
	{
		tokencount++;
		expression(list,table,lexlevel);
		if (list[tokencount].kind != rparentsym)
		{
			//error
		}
		tokencount++;
	}
	else
	{
		//error
	}
	
}

// checks if symbol is already in table
int checkTable(symbol* table, char* name)
{
	for (int i = 0; i < symbolcount; i++)
	{
		if (strcmp(name,table[i].name) == 0)
		{
			tableIndex = i;
			return 1;
		}
	}
	return 0;
}

int backCheck(symbol* table, char* name)
{
	for (int i = symbolcount; i > 0; i--)
	{
		if (strcmp(name,table[i].name) == 0)
		{
			tableIndex = i;
			return 1;
		}
	}
	return 0;
}

// adds error
void addError(int errNum)
{
    errors[numErrors] = errNum;
    numErrors++;
} 