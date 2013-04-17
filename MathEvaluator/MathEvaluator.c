#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "MathEvaluator.h"

//pre-processor constant TRACE_ACTIVE causes debugging data to the printed to the screen
//if and only if TRACE_ACTIVE evaluates to true
#define TRACE_ACTIVE 1

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
    #if (TRACE_ACTIVE)
        printf("(TRACEINFO) Evaluation Query: '%s'\n\n", expr);
    #endif

    //allocate the first node
    EQUATION_STACK_NODE* n = malloc(sizeof(EQUATION_STACK_NODE));
    eq->first = n;

    //set variables for paranthesis scan
    int open_par = 0;
    int close_par = 0;

    int i;

    //get max level
    for (i=0; expr[i]; i++)
    {
        if (expr[i]=='(')
            open_par++;
        else if (expr[i]==')')
            close_par++;
    }

    #if (TRACE_ACTIVE)
        printf("(TRACEINFO) Finished equation parsing.\n\tOpen parantheses: %d\n\tClosed parantheses: %d\n", open_par, close_par);
    #endif
    //die if the # of open paranthesis doesn't match the # of closed ones
    if (open_par!=close_par)
        return -1;

    //treat open_par as the current max level, level as the current level in the inner loop
    int level;

    while(open_par>=0)
    {
        #if (TRACE_ACTIVE)
            printf("(TRACEINFO) Initiating within outer loop, max level %d\n", open_par);
        #endif
        level = 0; //reset level

        for (i=0; expr[i]; i++)
        {
            if (expr[i]=='(')
                level++;
            else if (expr[i]==')')
                level--;

            if (level>open_par+1) //anything past this point has already been processed
            {
                #if (TRACE_ACTIVE)
                    printf("(TRACEINFO) Character reached '%c' position %d, exiting inner loop\n", expr[i], i);
                #endif
                break;
            }
            else if (level==open_par && IS_OPERATOR(expr[i])) //if we're at the maximum allowed level
            {
                #if (TRACE_ACTIVE)
                    printf("(TRACEINFO) At common level %d position %d character %c, storing operation\n", level, i, expr[i]);
                #endif
                // breakpoint

                int i2 = i;
                while (isalnum(expr[i2-1]))
                    i2--;

                double val1 = atof(expr+i2);
                double val2 = atof(expr+i+1);

                //load val1 when appropraite
                if (expr[i2]==')')
                {
                    #if (TRACE_ACTIVE)
                        printf("(TRACEINFO) In-parantheses expression at position %d excluded from tree (will be inferred)\n", i2);
                    #endif
                    //do nothing
                }
                else if (val1==0 && expr[i2]!=0)
                {
                    #if (TRACE_ACTIVE)
                        printf("(TRACEINFO) Loading from position %d as VAR\n", i2);
                    #endif
                    n->s = VAR;
                    n->eq_pointer = malloc(i-i2+1);
                    memcpy(n->eq_pointer, expr+i2, i-i2);
                    *(char*)(n->eq_pointer+(i-i2)) = 0; //set null-terminator

                    //prepare for next element
                    n->next = malloc(sizeof(EQUATION_STACK_NODE));
                    n = n->next;
                }
                else
                {
                    #if (TRACE_ACTIVE)
                        printf("(TRACEINFO) Loading from position %d as CONST\n", i2);
                    #endif
                    n->s = CONST;
                    n->eq_pointer = malloc(sizeof(double));
                    *(double*)(n->eq_pointer) = val1;

                    //prepare for next element
                    n->next = malloc(sizeof(EQUATION_STACK_NODE));
                    n = n->next;
                }

                //load val2 when appropriate
                if (expr[i+1]=='(')
                {
                    #if (TRACE_ACTIVE)
                        printf("(TRACEINFO) In-parantheses expression at position %d excluded from tree (will be inferred)\n", i+1);
                    #endif
                    //do nothing
                }
                else if (val2==0 && expr[i+1]!=0)
                {
                    #if (TRACE_ACTIVE)
                        printf("(TRACEINFO) Loading from position %d as VAR\n", i+1);
                    #endif
                    int i3 = i;
                    while (isalnum(expr[i3+1]))
                        i3++;
                    n->s = VAR;
                    n->eq_pointer = malloc(i3-i+1);
                    memcpy(n->eq_pointer, expr+i+1, i3-i);
                    *(char*)(n->eq_pointer+(i3-i)) = 0; //set null-terminator

                    //prepare for next element
                    n->next = malloc(sizeof(EQUATION_STACK_NODE));
                    n = n->next;
                }
                else
                {
                    #if (TRACE_ACTIVE)
                        printf("(TRACEINFO) Loading from position %d as CONST\n", i+1);
                    #endif
                    n->s = CONST;
                    n->eq_pointer = malloc(sizeof(double));
                    *(double*)(n->eq_pointer) = val2;

                    //prepare for next element
                    n->next = malloc(sizeof(EQUATION_STACK_NODE));
                    n = n->next;
                }

                //load operator
                #if (TRACE_ACTIVE)
                    printf("(TRACEINFO) Loading operator %c from position %d\n", expr[i], i);
                #endif
                n->s = OP;
                n->eq_pointer = get_operator_void_ptr(expr[i]);
                n->next = malloc(sizeof(EQUATION_STACK_NODE));
                n = n->next;
                n->next = 0;
            } //end last if

        } //end inner loop
        open_par--;
    } //end outer loop

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
    int element = 0;
    printf("--\n");
    EQUATION_STACK_NODE* n = eq->first;
    while (n->next)
    {
        switch (n->s)
        {
            case VAR:
            printf("[%d]VAR \t%s\n", ++element, (char*)n->eq_pointer);
            break;
            case CONST:
            printf("[%d]CONST \t%f\n", ++element, *(double*)n->eq_pointer);
            break;
            case OP:
            printf("[%d]OP \t\t%s\n", ++element, (char*)n->eq_pointer);
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
