#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "codegen.h"
#include "vm.h"

// string for input
char in[20000];

// main function
int main(int argc, char **argv)
{
    // if file isnt used as command argument
	FILE *ifp = NULL;
    ifp = fopen(argv[1], "r");
    if (ifp == NULL)
    {
        printf("error : please include the file name");
    }
    // initialize number of errors to 0
	numErrors = 0;
    // functions to create list, table, and code
	lexeme *list = lex_analyze(ifp);
    // stops program if list fails to form
    if (list == NULL)
    {
        printf("Error: ");
        switch (errors[numErrors])
		{
			case 1: 
				printf("Use = instead of :=.\n");
				break;
			case 2: 
				printf("= must be followed by a number.\n");
				break;
			case 3: 
				printf("Identifier must be followed by =.\n");
				break;
			case 4: 
				printf("const, var, procedure must be followed by identifier.\n");
				break;
			case 5: 
				printf("Semicolon or comma missing.\n");
				break;	
			case 6: 
				printf("Incorrect symbol after procedure declaration.\n");
				break;
			case 7: 
				printf("Statement expected.\n");
				break;
			case 8: 
				printf("Incorrect symbol after statement part in block.\n");
				break;
			case 9:
				printf("Period expected.\n");
				break;
			case 10: 
				printf("Semicolon between statements missing.\n");
				break;
			case 11: 
				printf("Undeclared identifier\n");
				break;
			case 12: 
				printf("Assignment to constant or procedure is not allowed\n");
				break;
			case 13: 
				printf("Assignment operator expected\n");
				break;
			case 14: 
				printf("call must be followed by an identifier.\n");
				break;
			case 15: 
				printf("Call of a constant or variable is meaningless.\n");
				break;
			case 16: 
				printf("then expected.\n");
				break;
			case 17: 
				printf("Semicolon or } expected\n");
				break;
			case 18: 
				printf("do expected.\n");
				break;
			case 19: 
				printf("Incorrect symbol following statement.\n");
				break;
			case 20: 
				printf("Relational operator expected.\n");
				break;
			case 21: 
				printf("Expression must not contain a procedure identifier.\n");
				break;
			case 22: 
				printf("Right parenthesis missing.\n");
				break;
			case 23: 
				printf("The preceding factor cannot begin with this symbol.\n");
				break;
			case 24: 
				printf("An expression cannot begin with this symbol.\n");
				break;
			case 25: 
				printf("This number is too large.\n");
				break;
			case 26: 
				printf("End expected\n");
				break;
			
		}
        return 0;
    }
	//symbol *table = parse(list);
	//instruction *code = generate_code(list,table);
    //prints errors if needed
    if (numErrors == 0)
    {
        printf("No Errors Found\n\n");
    }
    else
    {
    
	for (int i = 0; i < numErrors;i++)
	{
		switch (errors[i])
		{
			case 1: 
				printf("Use = instead of :=.\n");
				break;
			case 2: 
				printf("= must be followed by a number.\n");
				break;
			case 3: 
				printf("Identifier must be followed by =.\n");
				break;
			case 4: 
				printf("const, var, procedure must be followed by identifier.\n");
				break;
			case 5: 
				printf("Semicolon or comma missing.\n");
				break;	
			case 6: 
				printf("Incorrect symbol after procedure declaration.\n");
				break;
			case 7: 
				printf("Statement expected.\n");
				break;
			case 8: 
				printf("Incorrect symbol after statement part in block.\n");
				break;
			case 9: 
				printf("Period expected.\n");
				break;
			case 10: 
				printf("Semicolon between statements missing.\n");
				break;
			case 11: 
				printf("Undeclared identifier\n");
				break;
			case 12: 
				printf("Assignment to constant or procedure is not allowed\n");
				break;
			case 13: 
				printf("Assignment operator expected\n");
				break;
			case 14: 
				printf("call must be followed by an identifier.\n");
				break;
			case 15: 
				printf("Call of a constant or variable is meaningless.\n");
				break;
			case 16: 
				printf("then expected.\n");
				break;
			case 17: 
				printf("Semicolon or } expected\n");
				break;
			case 18: 
				printf("do expected.\n");
				break;
			case 19: 
				printf("Incorrect symbol following statement.\n");
				break;
			case 20: 
				printf("Relational operator expected.\n");
				break;
			case 21: 
				printf("Expression must not contain a procedure identifier.\n");
				break;
			case 22: 
				printf("Right parenthesis missing.\n");
				break;
			case 23: 
				printf("The preceding factor cannot begin with this symbol.\n");
				break;
			case 24: 
				printf("An expression cannot begin with this symbol.\n");
				break;
			case 25: 
				printf("This number is too large.\n");
				break;
			case 26: 
				printf("End expected\n");
				break;
			
		}
	}
    }

    // loops through and prints requested output
    for (int i = 2; i < argc; i++)
    {
        // prints lex table
        if (strcmp(argv[i],"-l") == 0)
        {
            for (int i = 0; i < lexPrint; i++)
            {
            if (list[i].kind == identsym)
            {
            printf("%d %s ", list[i].kind, list[i].name);
            }
            else if (list[i].kind == numbersym)
            {
                printf("%d %d ", list[i].kind, list[i].val);
            }
            else
            {
                printf("%d ", list[i].kind);
            }
        }
        printf("\n\n");

            for (int i = 0; i < lexPrint; i++)
            {
                if (list[i].kind == identsym)
                {
                    printf("%s %s ", tokenToString(list[i].kind), list[i].name);
                }
                else if (list[i].kind == numbersym)
                {
                    printf("%s %d ", tokenToString(list[i].kind), list[i].val);
                }
                else
                {
                    printf("%s ", tokenToString(list[i].kind));
                }
            }
            printf("\n");
            printf("\n");
        }
        // prints codegen output
        else if (strcmp(argv[i],"-a") == 0)
        {
            for (int i = 0; i < cx; i++)
        {
            printf("%d %d %d %d\n",code[i].op,code[i].r,code[i].l,code[i].m);
        }
        printf("\n");

        }

        // runs vm on generated code
        else if (strcmp(argv[i],"-v") == 0)
        {
           virtual_machine(code); 
        }
    }
}

// turns token into string
char *tokenToString(int token)
{
    switch(token)
    {
        case 1:
        return "nulsym";
        break;

        case 2:
        return "identsym";
        break;

        case 3:
        return "numbersym";
        break;

        case 4:
        return "plussym";
        break;

        case 5:
        return "minussym";
        break;

        case 6:
        return "multsym";
        break;
        
        case 7:
        return "slashsym";
        break;

        case 8:
        return "oddsym";
        break;

        case 9:
        return "eqsym";
        break;

        case 10:
        return "neqsym";
        break;

        case 11:
        return "lessym";
        break;

        case 12:
        return "leqsym";
        break;

        case 13:
        return "gtrsym";
        break;

        case 14:
        return "geqsym";
        break;

        case 15:
        return "lparentsym";
        break;

        case 16:
        return "rparentsym";
        break;

        case 17:
        return "commasym";
        break;

        case 18:
        return "semicolonsym";
        break;

        case 19:
        return "periodsym";
        break;

        case 20:
        return "becomessym";
        break;

        case 21:
        return "beginsym";
        break;

        case 22:
        return "endsym";
        break;

        case 23:
        return "ifsym";
        break;

        case 24:
        return "thensym";
        break;

        case 25:
        return "whilesym";
        break;

        case 26:
        return "dosym";
        break;

        case 27:
        return "callsym";
        break;
        
        case 28:
        return "constsym";
        break;

        case 29:
        return "varsym";
        break;

        case 30:
        return "procsym";
        break;

        case 31:
        return "writesym";
        break;

        case 32:
        return "readsym";
        break;

        case 33:
        return "elsesym";
        break;

        case 34:
        return "colonsym";
        break;

        default:
        return "null";
    }
}