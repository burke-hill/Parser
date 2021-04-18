/* 
    Charles McCampbell-Hill
    COP3402 - Montagne
    10/14/2020
*/




#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "parser.h"
/* #include "codegen.h"
#include "vm.h" */

//Declare program constraint constants.
#define MAX_IDENTIFIER_LENGTH   11
#define MAX_NUM_LENGTH          5


    // struct for tokens

// List of tokens
lexeme token_list[500];






    //Initialize input file character array and keep track of it's length.
    char in[20000];

    //Declare input and output files
    FILE *ifp = NULL;

    //Declare necessary arrays for tokens, numbers, and identifiers
    int tokens[20000];
    int token_cnt =  0;

    int num[20000];
    int num_cnt = 0;
    int num_print= 0;

    char id_list[20000][MAX_IDENTIFIER_LENGTH];
    int  id_ints[20000];
    int  id_cnt = 0;
    int  id_total_number = 1;

//Declare functions for converting strings to tokens and tokens to strings.
int characterToToken(char sym);
char* tokenToCharacter(int token);
    //Main Function
lexeme*  lex_analyze(FILE *ifp){
        int print = 0;

        // Opens file via command line
        //ifp =   fopen(argv[1], "r");

    
        // Reads file and prints source program
        
        char tokenId;
        int i = 0;

        while(fscanf(ifp,"%c", &tokenId) != EOF) {
            in[i]= tokenId;
            i++;
            input_len++;
    }

    // Loops through all input
    for (i=0; i< input_len; i++)
    {
        // Ignores commends in form /*
        if (in[i] == '/' && in[i+1] == '*')
        {
            // skips past commented indicies
            int j=i+2;

            // Loop until other end of comment is reached
            while (!(in[j] == '*' && in[j+1] == '/'))
            {
                j++;
            }
            //Continue from after the comment.
            i=j+2;
            continue;
        }

        //Ignore new lines
        if (isspace(in[i]))
            continue;
        if (iscntrl(in[i]))
            continue;

        // Check if reserved word
        if (isalpha(in[i]))
        {
            if (in[i] == 'b' && in[i+1] == 'e' && in[i+2] == 'g' && in[i+3] == 'i' && in[i+4] == 'n')
            {
                i+=5;
                token_list[token_cnt].kind = beginsym;
                token_cnt++;
            }
            else if (in[i] == 'e' && in[i+1] == 'n' && in[i+2] == 'd')
            {
                i+=3;
                token_list[token_cnt].kind = endsym;
                token_cnt++;
            }
            else if (in[i] == 'e' && in[i+1] == 'l' && in[i+2] == 's' && in[i+3] == 'e')
            {
                i+=4;
                token_list[token_cnt].kind = elsesym;
                token_cnt++;
            }
            else if (in[i] == 'i' && in[i+1] == 'f')
            {
                i+=2;
                token_list[token_cnt].kind = ifsym;
                token_cnt++;
            }
            else if (in[i] == 't' && in[i+1] == 'h' && in[i+2] == 'e' && in[i+3] == 'n')
            {
                i+=4;
                token_list[token_cnt].kind = thensym;
                token_cnt++;
            }
            else if (in[i] == 'w' && in[i+1] == 'h' && in[i+2] == 'i' && in[i+3] == 'l' && in[i+4] == 'e')
            {
                i+=5;
                token_list[token_cnt].kind = whilesym;
                token_cnt++;
            }
            else if (in[i] == 'w' && in[i+1] == 'r' && in[i+2] == 'i' && in[i+3] == 't' && in[i+4] == 'e')
            {
                i+=5;
                token_list[token_cnt].kind = writesym;
                token_cnt++;
            }
            else if (in[i] == 'd' && in[i+1] == 'o')
            {
                i+=2;
                token_list[token_cnt].kind = dosym;
                token_cnt++;
            }
            else if (in[i] == 'c' && in[i+1] == 'a' && in[i+2] == 'l' && in[i+3] == 'l')
            {
                i+=4;
                token_list[token_cnt].kind = callsym;
                token_cnt++;
            }
            else if (in[i] == 'c' && in[i+1] == 'o' && in[i+2] == 'n' && in[i+3] == 's' && in[i+4] == 't')
            {
                i+=5;
                token_list[token_cnt].kind = constsym;
                token_cnt++;
            }
            else if (in[i] == 'o' && in[i+1] == 'd' && in[i+2] == 'd')
            {
                i+=3;
                token_list[token_cnt].kind = oddsym;
                token_cnt++;
            }
            else if (in[i] == 'p' && in[i+1] == 'r' && in[i+2] == 'o' && in[i+3] == 'c'
                  && in[i+4] == 'e' && in[i+5] == 'd' && in[i+6] == 'u' && in[i+7] == 'r' && in[i+8] == 'e')
            {
                i+=9;
                token_list[token_cnt].kind = procsym;
                token_cnt++;
            }
            else if (in[i] == 'v' && in[i+1] == 'a' && in[i+2] == 'r')
            {
                i+=3;
                token_list[token_cnt].kind = varsym;
                token_cnt++;
            }
            else if (in[i] == 'r' && in[i+1] == 'e' && in[i+2] == 'a' && in[i+3] == 'd')
            {
                i+=3;
                token_list[token_cnt].kind = readsym;
                token_cnt++;
            }
        }

        //Takes a character and returns a token into tokenId.
        tokenId = characterToToken(in[i]);
        if (tokenId != 0)
        {
			int equals;
			//Check if symbol is combined with equal sign
			if(tokenId == lessym && characterToToken(in[i+1]) == gtrsym){
					token_list[token_cnt].kind = neqsym;
					token_cnt++;
					i++;
					continue;
				}
			if (tokenId == lessym && characterToToken(in[i+1]) == eqsym){
					token_list[token_cnt].kind = leqsym;
					token_cnt++;
					i++;
					continue;
            }    

			// >= case
			if(tokenId == gtrsym && characterToToken(in[i+1]) == eqsym){
					token_list[token_cnt].kind = geqsym;
					token_cnt++;
					i++;
					continue;
			}

            // := case
			if(tokenId == colonsym && characterToToken(in[i+1]) == eqsym){
					token_list[token_cnt].kind = becomessym;
					token_cnt++;
					i++;
					continue;
				}
                // else symbol is invalid


            // save current token
            token_list[token_cnt].kind = tokenId;
            token_cnt++;

            continue;
        }

        // Reads in identifiers to id
        if (isalpha(in[i]))
        {
            int flag= 1;
			int id_length;
            char id[MAX_IDENTIFIER_LENGTH] = {'\0'};
            int j, k, l;
            //sanitize string
            

            id_length=0;

            j=i;

            while(isalpha(in[j]) || isdigit(in[j]))
            {
                // indentifier exceeds max length
                if (id_length > MAX_IDENTIFIER_LENGTH)
                {
                    addError(24);
                    return NULL;
                }

                // else create string for identifier
                id[id_length] = in[j];
                id_length++;
                j++;
            }

            //store that we have an identifier
            token_list[token_cnt].kind = identsym;
            strcpy(token_list[token_cnt].name,id);
            token_cnt++;


            i= j-1;
            continue;


        }


        // Takes in numbers
        if (isdigit(in[i]))
        {
			int digits;
			int number;
            int j= i;
			int k;
            // gets length of current number
            for (j= i; j< MAX_NUM_LENGTH+i +1 ; j++)
            {

                if (isspace(in[j]) || iscntrl(in[j]))
                    break;

                // cannot have letter in number
                if (isalpha(in[j]))
                {
                    addError(24);
                    return NULL;
                }
            }

            j=i;

            // checks to see if digit is too long
            digits=1;
            number=0;
            char newnum[6];
            int excess_flag = 0;
            int p = 0;
            while (isdigit(in[j]))
            {
                newnum[p] = in[j];
                p++;
                if (digits > MAX_NUM_LENGTH)
                {
                    addError(25);
                    return NULL;
                }
                digits++;
                j++;
            }

           

            // Stores number
            token_list[token_cnt].kind = numbersym;
            token_list[token_cnt].val = atoi(newnum);
            token_cnt++;

            i= j-1;
            continue;
        }

    }

	num_print= 0;
	id_cnt= 0;
  



    lexPrint = token_cnt;
    return token_list;
}




// turns character to token
int characterToToken(char sym)
{
    if (sym == '*')
    {
        return multsym;
    }
    else if (sym == '+')
    {
        return plussym;
    }
    else if (sym == '-')
    {
        return minussym;
    }
    else if (sym == '/')
    {
        return slashsym;
    }
    else if (sym == '(')
    {
        return lparentsym;
    }
    else if (sym == ')')
    {
        return rparentsym;
    }
    else if (sym == ',')
    {
        return commasym;
    }
    else if (sym == ';')
    {
        return semicolonsym;
    }
    else if (sym == '.')
    {
        return periodsym;
    }
    else if (sym == '<')
    {
        return lessym;
    }
    else if (sym == '>')
    {
        return gtrsym;
    }
    else if (sym == '=')
    {
        return eqsym;
    }
    else if (sym == ':')
    {
        return colonsym;
    }
    else
    {
        return 0;
    }
}

// turns token to character
char* tokenToCharacter(int token){

    char* temp;

    if (token == multsym)
    {
        return "*";
    }
    else if (token == plussym)
    {
        return "+";
    }
    else if (token == minussym)
    {
        return "-";
    }
    else if (token == slashsym)
    {
        return "/";
    }
    else if (token == lessym)
    {
        return "<";
    }
    else if (token == gtrsym)
    {
        return ">";
    }
    else if (token == eqsym)
    {
        return "=";
    }
    else if (token == leqsym)
    {
        return "<=";
    }
    else if (token == geqsym)
    {
        return  ">=";
    }
    else if (token == becomessym)
    {
        return ":=";
    }
    else if (token == neqsym)
    {
        return "<>";
    }
    else if (token == lparentsym)
    {
        return "(";
    }
    else if (token == rparentsym)
    {
        return ")";
    }
    else if (token == commasym)
    {
        return ",";
    }
    else if (token == semicolonsym)
    {
        return ";";
    }
    else if(token == periodsym)
    {
        return ".";
    }
    else if (token == varsym)
    {
        return "var";
    }
    else if (token == beginsym)
    {
        return "begin";
    }
    else if (token == identsym)
    {
        strcpy(temp,id_list[id_total_number]);
        id_total_number++;
        return temp;
    }
    else if (token == endsym)
    {
        return "end";
    }
    else if (token == ifsym)
    {
        return "if";
    }
    else if (token == thensym)
    {
        return "then";
    }
    else if (token == whilesym)
    {
        return "while";
    }
    else if (token == dosym)
    {
        return "do";
    }
    else if (token == callsym)
    {
        return "call";
    }
    else if (token == constsym)
    {
        return "const";
    }
    else if (token == elsesym)
    {
        return "else";
    }
    else if (token == procsym)
    {
        return "procedure";
    }
    else if (token == readsym)
    {
        return "read";
    }
    else if (token == writesym)
    {
        return "write";
    }
    else if (token == oddsym)
    {
        return "odd";
    }
    else
    {
        return 0;
    }
}

// adds error
void addError(int errNum)
{
    errors[numErrors] = errNum;
    numErrors++;
}