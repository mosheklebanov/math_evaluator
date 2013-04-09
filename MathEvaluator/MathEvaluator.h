#ifndef MATHEVALUATOR_H_INCLUDED
#define MATHEVALUATOR_H_INCLUDED

typedef enum e_eqsignifier //stored as `int`
{
    VAR,
    CONST,
    OP
} EqSignifier;

typedef struct equationstackvalue_t
{
    EqSignifier s;
    void* eq_pointer;
    struct equationstackvalue_t* next;
} EQUATION_STACK_NODE;

typedef struct equation_t
{
    EQUATION_STACK_NODE* first;
} EQUATION;

typedef struct vartablevalue_t
{
    char* name;
    double value;
    struct vartablevalue_t* next;
} VARTABLE_VALUE;

typedef struct vartable_t
{
    VARTABLE_VALUE* first;
    VARTABLE_VALUE* last;
} VARTABLE;

int parse_equation(EQUATION* eq, const char* expr);
int free_equation(EQUATION* eq);
void walk_eqation(EQUATION* eq);

int init_vartable(VARTABLE* vt);
int append_to_vartable(VARTABLE* vt, const char* name, double value);
int free_vartable(VARTABLE* vt);

int eval_equation(EQUATION* eq, VARTABLE* vt, double* result);

#endif // MATHEVALUATOR_H_INCLUDED
