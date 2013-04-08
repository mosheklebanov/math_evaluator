#include <stdio.h>
#include <stdlib.h>
#include "MathEvaluator.h"

int main()
{
    EQUATION* eq = 0;
    printf("\nResult: %d", parse_equation(eq, "(a+0.0)"));

    return 0;
}
