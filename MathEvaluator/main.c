#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "MathEvaluator.h"

int main()
{
    EQUATION eq;
    assert(parse_equation(&eq, "mat+(bath*76)")==0);
    walk_eqation(&eq);
    return 0;
}
