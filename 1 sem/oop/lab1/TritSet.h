//
// Created by Andrew on 026 26.09.20.
//

#ifndef LAB1_TritSet_H
#define LAB1_TritSet_H

#include <iostream>
#include <cstring>
#include <algorithm>
#include <unordered_map>
#include "Trit.h"

typedef unsigned int uint;

class TritSetProxy;

class TritSet
{
    friend TritSetProxy;
private:
    void setBit(uint uintPos, uint tritPos, uint value);
    void expandArrayByIndex(uint index);
    uint sizeOfArray; //размер вектора в uint-ах
    uint sizeInTrits;
    uint lastTritIndex; // Все триты дальше считаются за Unknown
    uint* tritArray;
    Trit getValue(uint uintPos, uint tritPos);
    void updateLastKnownTrit();
public:
    uint capacity() const; //Вместимость в тритах
    uint length();
    void shrink();
    std::unordered_map< Trit, int, std::hash<int>> cardinality();
    TritSet(TritSet& set);
    TritSet(const uint sizeInTrits);
    TritSet();
    ~TritSet();
    void trim(std::size_t lastIndex);
    std::size_t cardinality(Trit value);

    TritSetProxy operator[](uint index);
    TritSet operator&(TritSet& op);
    TritSet operator|(TritSet& op);
    TritSet operator!();
    TritSet& operator=(TritSet& op);
};

class TritSetProxy
{
    friend TritSet;
    TritSetProxy(uint intPos, uint tritPos, TritSet* s);
    uint uintPosition;
    uint tritPosition;
    uint index;
    TritSet* set;
public:
    operator Trit();
    TritSetProxy& operator=(Trit value);
    TritSetProxy& operator=(TritSetProxy& operand);
    bool operator==(Trit operand);
    bool operator==(TritSetProxy& operand);
};

#endif //LAB1_TritSet_H
