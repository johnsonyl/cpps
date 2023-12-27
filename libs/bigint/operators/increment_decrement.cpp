/*
    ===========================================================================
    Increment and decrement operators
    ===========================================================================
*/

#ifndef BIG_INT_INCREMENT_DECREMENT_OPERATORS_HPP
#define BIG_INT_INCREMENT_DECREMENT_OPERATORS_HPP

#include "../BigInt.h"

/*
    Pre-increment
    -------------
    ++BigInt
*/

BigInt& BigInt::operator++() {
    *this += 1;

    return *this;
}


/*
    Pre-decrement
    -------------
    --BigInt
*/

BigInt& BigInt::operator--() {
    *this -= 1;

    return *this;
}


/*
    Post-increment
    --------------
    BigInt++
*/

BigInt BigInt::operator++(int) {
    BigInt temp = *this;
    *this += 1;

    return temp;
}


/*
    Post-decrement
    --------------
    BigInt--
*/

BigInt BigInt::operator--(int) {
    BigInt temp = *this;
    *this -= 1;

    return temp;
}

#endif  // BIG_INT_INCREMENT_DECREMENT_OPERATORS_HPP
