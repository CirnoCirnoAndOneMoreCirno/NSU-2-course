#include "Trit.h"


Trit operator&(const Trit op1, const Trit op2)
{
    if ((op1 == True) && (op2 == True))
        return True;
    if ((op1 == False) || (op2 == False))
        return False;
    return Unknown;
}

Trit operator|(const Trit op1, const Trit op2)
{
    if ((op1 == False) && (op2 == False))
        return False;
    if ((op1 == True) || (op2 == True))
        return True;
    return Unknown;
}

Trit operator!(const Trit op)
{
    switch (op)
    {
    case True:
        return False;
    case False:
        return True;
    default:
        return Unknown;
    }
}