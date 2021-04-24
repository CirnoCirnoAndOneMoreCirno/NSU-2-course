//
// Created by Andrew on 026 26.09.20.
//
#include "TritSet.h"


TritSet TritSet::operator&(TritSet& op)
{
    if ((this->sizeOfArray == 0) || (op.sizeOfArray == 0))
        exit(1);

    TritSet res(std::max(this->sizeInTrits, op.sizeInTrits));

    for (uint i = 0; i <= res.sizeInTrits; ++i)
    {
        res[i] = (*this)[i] & op[i];
    }
    return res;
}

TritSet TritSet::operator|(TritSet& op)
{
    if ((this->sizeOfArray == 0) || (op.sizeOfArray == 0))
        exit(1);

    TritSet res(std::max(this->sizeInTrits, op.sizeInTrits));
    for (uint i = 0; i <= res.lastTritIndex; ++i)
    {
        res[i] = (*this)[i] | op[i];
    }
    return res;
}

TritSet TritSet::operator!()
{
   
    if (this->sizeOfArray == 0)
        exit(1);
    TritSet res(this->sizeInTrits);
    for (uint i = 0; i <= this->lastTritIndex; ++i)
        res[i] = !((Trit)(*this)[i]);
    return res;
}

TritSetProxy::TritSetProxy(uint intPos, uint tritPos, TritSet* s)
{
    index = sizeof(uint) * 4 * intPos + tritPos;
    uintPosition = intPos;
    tritPosition = tritPos;
    set = s;
}

TritSet::TritSet(TritSet& set)
{
    sizeInTrits = set.sizeInTrits;
    sizeOfArray = set.sizeOfArray;
    lastTritIndex = set.lastTritIndex;
    if (set.tritArray == nullptr)
        tritArray = nullptr;
    else
    {
        tritArray = new uint[sizeOfArray];
        memcpy(tritArray, set.tritArray, sizeof(uint) * sizeOfArray);
    }
}


TritSet::TritSet(const uint size)
{
    sizeInTrits = size;
    sizeOfArray = (2 * size) / (8 * sizeof(unsigned int));
    if (((2 * size) % (8 * sizeof(unsigned int)) != 0))
        sizeOfArray++;
    tritArray = new uint[sizeOfArray];
    memset(tritArray, 0, sizeOfArray * sizeof(uint));
    if (size > 0) // Для set(0)
        lastTritIndex = size - 1;
    else
        lastTritIndex = 0;


}

TritSet::TritSet()
{
    lastTritIndex = 0;
    sizeOfArray = 0;
    sizeInTrits = 0;
    tritArray = nullptr;
}

TritSet::~TritSet()
{
    if (tritArray != nullptr)
        delete[] tritArray;
}

void TritSet::setBit(uint uintPos, uint bitPos, uint value)
{
    if (value)
        tritArray[uintPos] = tritArray[uintPos] | 1u << (8 * sizeof(uint) - bitPos - 1);
    else
        tritArray[uintPos] = tritArray[uintPos] & ~(1u << (8 * sizeof(uint) - bitPos - 1));
}

uint TritSet::capacity() const
{
    return (this->sizeOfArray * 8 * sizeof(uint)) / 2;
}

uint TritSet::length()
{
    if (this->sizeOfArray == 0)
        return 0;
    if ((*this)[lastTritIndex] == Unknown)
        this->updateLastKnownTrit();
    if (((*this)[lastTritIndex] == Unknown) && (this->lastTritIndex == Unknown))
        return 0;
    return this->lastTritIndex + 1;
}

void TritSet::expandArrayByIndex(uint index)
{
    //index++;
    uint tritsInUint = 8 * sizeof(uint) / 2;
    uint newLength = (index+1) / tritsInUint;
    if ((index+1) % tritsInUint != 0)
        newLength++;
    if (newLength > this->sizeOfArray)
    {
        uint* newArray = new uint[newLength];

        if (this->tritArray != nullptr)
        {
            memcpy(newArray, this->tritArray, this->sizeOfArray * sizeof(uint));
            delete[] this->tritArray;
        }

        this->tritArray = newArray;

        for (uint i = this->sizeOfArray; i < newLength; ++i)
            this->tritArray[i] = 0; //все добавленные значения считаем unknown
        this->sizeOfArray = newLength;
        this->lastTritIndex = index;
    }
    sizeInTrits = index + 1;
}

void TritSet::updateLastKnownTrit()
{
    int i = lastTritIndex - 1;

    if (i > (int)(4 * sizeof(uint) - 1)) // Сразу пробегаем такие элементы массива, в которых все биты равны нулю
    {
        int index = i / (4 * sizeof(uint)) + 1;
        while ((tritArray[index] == 0) && (index > 0))
        {
            index--;
        }
        i = std::max(16 * (index - 1) + 15, 0);
    }
    while ((i > 0) && ((*this)[i] == Unknown))
    {
        i--;
    }
    if (i > 0)
        lastTritIndex = i;
    else
        lastTritIndex = 0;
}

void TritSet::trim(std::size_t lastIndex)
{
    if (lastIndex > lastTritIndex)
        return;

    uint indexInArray = lastIndex / (4 * sizeof(uint)) + 1;
    uint lastIndexInArray = lastTritIndex / (4 * sizeof(uint));

    for (uint i = indexInArray; i <= lastIndexInArray; ++i)
        tritArray[i] = 0;
    uint tritIndex = lastIndex % ((sizeof(uint) * 8) / 2);
    uint mask = 1 << (8 * sizeof(uint) - tritIndex * 2);
    mask--;
    mask = ~mask;
    if (tritIndex == 0)
        mask = 0;
    this->tritArray[indexInArray - 1] &= mask;
    if (lastIndex > 0)
        lastTritIndex = lastIndex - 1;  //trim(0)
    else
        lastTritIndex = 0;
}

uint TritSet::cardinality(Trit value)
{
    uint res = 0;
    for (uint i = 0; (i <= lastTritIndex) && (sizeOfArray > 0); i++)//На случай пустого TritSet'а
    {
        if ((*this)[i] == value)
            res++;
    }
    if ((value == Unknown) && (lastTritIndex < (sizeInTrits - 1)))
        res += sizeInTrits - lastTritIndex - 1;
    return res;
}


std::unordered_map< Trit, int, std::hash<int> > TritSet::cardinality()
{
    std::unordered_map< Trit, int, std::hash<int> > res;
    for (uint i = 0; i <= lastTritIndex; i++)
    {
        res[(Trit)((*this)[i])]++;
    }
    if (lastTritIndex < (sizeInTrits - 1))
        res[Unknown] += sizeInTrits - lastTritIndex - 1;
    return res;
}

void TritSet::shrink()
{
    if (sizeOfArray == 0)
        return; //Возможно, стоит убрать

    if ((*this)[lastTritIndex] == Unknown)
    {
        updateLastKnownTrit();
    }
    if ((lastTritIndex == 0) && ((*this)[lastTritIndex] == Unknown))
    {
        sizeOfArray = 0;
        sizeInTrits = 0;
        delete[] tritArray;
        tritArray = nullptr;
        return;
    }

    sizeOfArray = (2 * (lastTritIndex + 1)) / (8 * sizeof(unsigned int));
    sizeInTrits = lastTritIndex + 1;

    if (((2 * (lastTritIndex + 1)) % (8 * sizeof(unsigned int)) != 0))
        sizeOfArray++;

    uint* newArray = new uint[sizeOfArray];
    std::memcpy(newArray, this->tritArray, this->sizeOfArray * sizeof(uint));
    delete[] this->tritArray;
    this->tritArray = newArray;
}

TritSetProxy TritSet::operator[](uint index)
{
    uint tritsInUint = (8 * sizeof(uint)) / 2;
    uint uintPos = index / tritsInUint;
    uint tritPos = index % tritsInUint;
    return TritSetProxy(uintPos, tritPos, this);
}

Trit TritSet::getValue(uint uintPos, uint tritPos)
{
    if (this->sizeInTrits == 0) 
        return Unknown; //Для пустого TritSet

    uint tritsInUint = (8 * sizeof(uint)) / 2;

    if ((uintPos * tritsInUint + tritPos) > lastTritIndex)
        return Unknown;

    uint val = ((this->tritArray)[uintPos] >> (8 * sizeof(uint) - 2 * tritPos - 2));
    val = val & 3;
    switch (val)
    {
    case 0:
    {
        return Unknown;
    }
    case 1:
    {
        return False;
    }
    case 2:
    {
        return True;
    }
    default:
        exit(1);
    }
}

TritSetProxy::operator Trit()
{
    return set->getValue(uintPosition, tritPosition);
}

TritSetProxy& TritSetProxy::operator=(Trit value)
{
    if (value == Unknown && index >= set->sizeInTrits)
        return *this;

    if (((uintPosition+1) > set->sizeOfArray) || (set->sizeOfArray == 0))
    {
        set->expandArrayByIndex(4 * uintPosition * sizeof(uint) + tritPosition);
    }

    if ((this->set->lastTritIndex < this->index) && (value != Unknown))
    {
        this->set->sizeInTrits = this->index + 1;
        this->set->lastTritIndex = this->index;
    }

    /*
    if ((this->set->lastTritIndex == this->index) && (value == Unknown))
    {
        this->set->updateLastKnownTrit();
    }
    */

    set->setBit(uintPosition, 2 * tritPosition, (value & 2) >> 1);
    set->setBit(uintPosition, 2 * tritPosition + 1, value & 1);

    return *this;
}


TritSetProxy& TritSetProxy::operator=(TritSetProxy& operand)
{
    Trit value = operand.set->getValue(operand.uintPosition, operand.tritPosition);
    *this = value;

    return *this;
}

TritSet& TritSet::operator=(TritSet& op)
{
    this->lastTritIndex = op.lastTritIndex;
    this->sizeOfArray = op.sizeOfArray;
    this->sizeInTrits = op.sizeInTrits;
    delete[] this->tritArray;
    if (op.tritArray != nullptr)
    {
        this->tritArray = new uint[op.sizeOfArray];
        memcpy(this->tritArray, op.tritArray, op.sizeOfArray * sizeof(uint));
    }
    else
        this->tritArray = nullptr;
    return *this;
}

bool TritSetProxy::operator==(Trit operand)
{
    return ((Trit)(*this) == operand);
}


bool TritSetProxy::operator==(TritSetProxy& operand)
{
    return ((Trit)(*this) == (Trit)operand);
}