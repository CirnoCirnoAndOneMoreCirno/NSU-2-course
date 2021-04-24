#pragma once
#ifndef LAB1_Trit_H
#define LAB1_Trit_H


enum Trit { Unknown = 0, False = 1, True = 2 };

Trit operator&(const Trit op1, const Trit op2);
Trit operator|(const Trit op1, const Trit op2);
Trit operator!(const Trit op);

#endif