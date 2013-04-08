#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "MathEvaluator.h"

#define IS_OPERATOR(c) ((c)=='+' || (c)=='-' || (c)=='*' || (c)=='/')

const static char* OP_ADDITION       = "+";
const static char* OP_SUBTRACTION    = "-";
const static char* OP_MULTIPLICATION = "*";
const static char* OP_DIVISION       = "/";

void* get_operator_void_ptr(char op)
{
    if (op=='+')
        return (void*)OP_ADDITION;
    if (op=='-')
        return (void*)OP_SUBTRACTION;
    if (op=='*')
        return (void*)OP_MULTIPLICATION;
    if (op=='/')
        return (void*)OP_DIVISION;
    return 0;
}


int parse_equation(EQUATION* eq, const char* expr)
{
    EQUATION_STACK_NODE* n = malloc(sizeof(EQUATION_STACK_NODE));

    int open_par = 0;
    int close_par = 0;

    int i;

    for (i=0; expr[i]; i++)
    {
        if (expr[i]=='(')
            open_par++;
        else if (expr[i]==')')
            close_par++;
    }

    if (open_par!=close_par)
        return -1;

    int level = 0;

    for (i=0; expr[i]; i++)
    {
        if (expr[i]=='(')
            level++;
        else if (expr[i]==')')
            level--;

        if (level==open_par && IS_OPERATOR(expr[i]))
        {
            // breakpoint
            printf("At position %d\n", i);
            int i2 = i;
            while (isalnum(expr[i2-1]))
                i2--;

            double val1 = atof(expr+i2);
            double val2 = atof(expr+i+1);

            /* printf("%f %f\n", atof(expr+i2), atof(expr+i+1));
            printf("%d %d\n", expr[i2]=='0', expr[i+1]=='0'); */

            //load val1
            if (val1==0 && expr[i2]!=0)
            {
                n->s = VAR;
                n->eq_pointer = strdup(expr+i2);
            }
            else
            {
                n->s = CONST;
                n->eq_pointer = malloc(sizeof(double));
                *(double*)(n->eq_pointer) = val1;
            }
            n->next = malloc(sizeof(EQUATION_STACK_NODE));
            n = n->next;

            //load val2
            if (val2==0 && expr[i+1]!=0)
            {
                n->s = VAR;
                n->eq_pointer = strdup(expr+i+1);
            }
            else
            {
                n->s = CONST;
                n->eq_pointer = malloc(sizeof(double));
                *(double*)(n->eq_pointer) = val2;
            }
            n->next = malloc(sizeof(EQUATION_STACK_NODE));
            n = n->next;

            //load operator
            n->s = OP;
            n->eq_pointer = get_operator_void_ptr(expr[i]);
            n->next = malloc(sizeof(EQUATION_STACK_NODE));
            n = n->next;
        }
    }

    return 0;
}
