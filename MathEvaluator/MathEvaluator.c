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
    eq->first = n;

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
                n->eq_pointer = malloc(i-i2+1);
                memcpy(n->eq_pointer, expr+i2, i-i2);
                *(char*)(n->eq_pointer+(i-i2)) = 0; //set null-terminator
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
                int i3 = i;
                while (isalnum(expr[i3+1]))
                    i3++;
                n->s = VAR;
                n->eq_pointer = malloc(i3-i+1);
                memcpy(n->eq_pointer, expr+i+1, i3-i);
                *(char*)(n->eq_pointer+(i3-i)) = 0; //set null-terminator
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
            n->next = 0;
        }
    }

    return 0;
}

int free_equation(EQUATION* eq)
{
    EQUATION_STACK_NODE* i = eq->first;
    EQUATION_STACK_NODE* j;

    while (i->next) //while there are elements to traverse
    {
        j = i->next;
        if (i->s!=OP)
            free(i->eq_pointer);
        free(i);
        i = j;
    }
    return 0;
}

/* DEBUG */
void walk_eqation(EQUATION* eq)
{
    printf("--\n");
    EQUATION_STACK_NODE* n = eq->first;
    while (n->next)
    {
        switch (n->s)
        {
            case VAR:
            printf("VAR \t%s\n", (char*)n->eq_pointer);
            break;
            case CONST:
            printf("CONST \t%f\n", *(double*)n->eq_pointer);
            case OP:
            printf("OP \t%s\n", (char*)n->eq_pointer);
            break;
            default:
            printf("---- Faulty element ----");
        }
        n = n->next;
    }
    printf("--\n");
}

int init_vartable(VARTABLE* vt)
{
    vt->first = 0;
    vt->last = 0;
    return 0;
}

int append_to_vartable(VARTABLE* vt, const char* name, double value)
{
    if (vt->last==0)
    {
        VARTABLE_VALUE* v = malloc(sizeof(VARTABLE_VALUE));
        vt->first = v;
        vt->last = v;
    }

    VARTABLE_VALUE* v_last = vt->last;
    VARTABLE_VALUE* v_next = malloc(sizeof(VARTABLE_VALUE));

    v_next->name = strdup(name);
    v_next->value = value;
    v_next->next = 0;

    v_last->next = v_next;
    vt->last = v_last;

    return 0;
}

int free_vartable(VARTABLE* vt)
{
    VARTABLE_VALUE* i = vt->first;
    VARTABLE_VALUE* j;

    while (i->next) //while there are elements to traverse
    {
        j = i->next;
        free(i->name);
        free(i);
        i = j;
    }
    return 0;
}

int eval_equation(EQUATION* eq, VARTABLE* vt, double* result)
{
    return -1; //to be implemented
}
