/*  PM/0 Virtual Machine
    Charles McCampbell-Hill
    09/22/2020
    COP 3402
    Euripedes Montagne
*/


// Header files
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "codegen.h"
#include "vm.h"


// Predefined values
#define MAX_STACK_HEIGHT 1000
#define MAX_CODE_LENGTH 500

// Functions to be used
void print(int ctr);
void fetch();
void execute();
void printStack();
int base(int l, int base);


// Struct to hold instructions


// File pointer used for input
FILE *ifp;

// Arrays for registers and stack
int reg[8] = {0};
int stack[MAX_STACK_HEIGHT] = {0};

// Array of instructions from code
struct ir;

// Halt set to 0/Can be set to 1 to stop program
int halt = 0;

// Stack pointer/Base pointer/Program counter
int sp = MAX_STACK_HEIGHT;
int bp = MAX_STACK_HEIGHT - 1;
int pc = 0;
struct instruction ir;


// Main function
void virtual_machine(instruction* code)
{
    

    int ctr = 0;
   
    
    // Print initial values
    printf("\t\t\tpc\tbp\tsp\n");
    printf("Intial Values\t\t%d\t%d\t%d\n", pc,bp,sp);

    while (halt == 0)
        {

            // Fetch cycle
            fetch();
            // Execute cycle
            execute();

            printf("%d %d %d\t\t%d\t%d\t%d\n",ir.r,ir.l,ir.m,pc,bp,sp);
            printStack(); 
            halt = 1;     
        }
    print(cx);
    }


void fetch()
{
    // instruction is fetched and place in IR register
    ir = code[pc];
    // increment program counter by 1;
    pc++;
    return;
}


void execute()
{

// executes base on OPCODE
 switch(ir.op)
 {
    // Load literal
    case 1:
        //printf("%d LIT\t", pc-1);
        reg[ir.r] = ir.m;
        break;
    // Return
    case 2:
        //printf("%d RTN\t", pc-1);
        sp = bp+1;
        bp = stack[sp-2];
        pc = stack[sp-3];
        break;
    // Load into register a stack value 
    case 3:
        //printf("%d LOD\t",pc-1);
        reg[ir.r] = stack[base(ir.l, bp) - ir.m];
        break; 
    // Store in stack a register value
    case 4:
        //printf("%d STO\t", pc-1);
        stack[base(ir.l,bp)- ir.m] = reg[ir.r];
        break;
    // Call prodcedure
    case 5:
        //printf("%d CAL\t", pc-1);
        stack[sp-1] = base(ir.l,bp);
        stack[sp-2] = bp;
        stack[sp-3] = pc;
        bp = sp-1;
        pc = ir.m;
        break;
    // Allocate M memory words
    case 6:
        //printf("%d INC\t", pc-1);
        sp = sp-ir.m;
        break;
    // Jump to instruction M
    case 7:
        //printf("%d JMP\t", pc-1);
        pc = ir.m;
        break;
    // Jump to M if R == 0
    case 8:
        //printf("%d JPC\t", pc-1);
        if (reg[ir.r] == 0)
        {
            pc = ir.m;
        }
        break;
    // Based on M, either writes, reads, or ends program
    case 9:
        //printf("%d SYS\t", pc-1);
            switch (ir.m)
            {
                case 1:
                    //printf("%d\t", reg[ir.r]);
                    break;
                case 2:
                    //printf("Enter Value: ");
                    scanf("%d", &reg[ir.r]);
                    break;
                case 3:
                    halt = 1;
                    break;
            }
        break;
    // Makes register value negative
    case 10:
        //printf("%d NEG ", pc-1);
        reg[ir.r] = -(reg[ir.r]);
        break;
    // Adds two register values
    case 11: 
        //printf("%d ADD ",pc-1);
        reg[ir.r] = reg[ir.l] +reg[ir.m];
        break;
    // Subtracts two register values
    case 12: 
        //printf("%d SUB ", pc-1);
        reg[ir.r] = reg[ir.l] - reg[ir.m];
        break;
    // Multiplies two register values
    case 13:
        //printf("%d MUL ",pc-1);
        reg[ir.r] = reg[ir.l] * reg[ir.m];
        break;
    // Divides two register values
    case 14:
        //printf("%d DIV ",pc-1);
        reg[ir.r] = (reg[ir.l]/reg[ir.m]);
    // If a register value is odd or not
    case 15: 
        //printf("%d ODD",pc-1);
        reg[ir.r] = reg[ir.r] % 2;
        break;
    // Mod a register by value M
    case 16: 
        //printf("%d MOD ",pc-1);
        reg[ir.r] = reg[ir.l] % reg[ir.m];
        break;
    // If equal 1 else 0
    case 17: 
        //printf("%d EQL ",pc-1);
        if (reg[ir.l]==reg[ir.m])
        {
            reg[ir.r] = 1;
        }
        else
        {
            reg[ir.r] = 0;
        }
        break;
    // If not equal 1 else 0
    case 18: 
        //printf("%d NEQ ", pc-1);
        if (reg[ir.l]!=reg[ir.m])
        {
            reg[ir.r] = 1;
        }
        else
        {
            reg[ir.r] = 0;
        }
        break;
    // If L < M then 1 else 0
    case 19:
        //printf("%d LSS ", pc-1);
        if (reg[ir.l]<reg[ir.m])
        {
            reg[ir.r] = 1;
        }
        else
        {
            reg[ir.r] = 0;
        }
        break;
    // IF L <= M then 1 else 0
    case 20: 
        //printf("%d LEQ ", pc-1);
        if (reg[ir.l]<=reg[ir.m])
        {
            reg[ir.r] = 1;
        }
        else
        {
            reg[ir.r] = 0;
        }
        break;
    // If L > M then 1 else 0
    case 21: 
        //printf("%d GTR ", pc-1);
        if (reg[ir.l]>reg[ir.m])
        {
            reg[ir.r] = 1;
        }
        else
        {
            reg[ir.r] = 0;
        }
        break;
    // If L >= M then 1 else 0
    case 22:
        //printf("%d GEQ ",pc-1);
        if (reg[ir.l]>=reg[ir.m])
        {
            reg[ir.r] = 1;
        }
        else
        {
            reg[ir.r] = 0;
        }
        break;

    // Default case
    default:
        break;
        return;
    }
    return; 
}

// Finds base L levels away
int base(int l, int base)
{
    int b1;
    b1 = base;
    while (l>0)
    {
        b1 = stack[b1];
        l--;
    }
    return b1;
}

// Prints operation to take place on line i
void print(int ctr)
{
    for (int i = 0; i < ctr; i++)
    {
        int op = code[i].op;

        switch(op)
        {
            case 1: printf("lit\t");  break;
            case 2: printf("rtn\t");  break;
            case 3: printf("lod\t");  break;
            case 4: printf("sto\t");  break;
            case 5: printf("cal\t");  break;
            case 6: printf("inc\t");  break;
            case 7: printf("jmp\t");  break;
            case 8: printf("jpc\t");  break;
            case 9: printf("sys\t");  break;
            case 10:printf("sys\t");  break;
            case 11:printf("sys\t");  break;
            case 12:printf("neg\t");  break;
            case 13:printf("add\t");  break;
            case 14:printf("sub\t");  break;
            case 15:printf("mul\t");  break;
            case 16:printf("div\t");  break;
            case 17:printf("odd\t");  break;
            case 18:printf("mod\t");  break;
            case 19:printf("eql\t");  break;
            case 20:printf("neq\t");  break;
            case 21:printf("lss\t");  break;
            case 22:printf("leq\t");  break;
            case 23:printf("gtr\t");  break;
            case 24:printf("geq\t");  break;
        }
        printf("%d\t%d\t%d\n", code[i].r, code[i].l, code[i].m);
    }
    return;
}

// Prints the values in the stack & register array
void printStack()
{
   if (bp == 0) return;
    printf("Register: ");
    for (int i = 0; i < 8; i++)
    {
        printf("%d ", reg[i]);
    }
    printf("\n");
    printf("Stack: ");
    int countything = 0;
    if (sp == MAX_STACK_HEIGHT)
    {
        for(int i = 0; i < 5; i++)
        {
            printf("0 ");
        }
    }
    for (int i = MAX_STACK_HEIGHT; i > sp; i--)
    {
        if (countything == 5) printf("| ");
        printf("%d ", stack[i-1]);
        countything++;
    }
    printf("\n");
}