/*
    ===========================================================================
    Arithmetic-assignment operators
    ===========================================================================
*/
#include "../BigInt.h"

/*
    BigInt += BigInt
    ----------------
*/

BigInt& BigInt::operator+=(const BigInt& num) {
    *this = *this + num;

    return *this;
}


/*
    BigInt -= BigInt
    ----------------
*/

BigInt& BigInt::operator-=(const BigInt& num) {
    *this = *this - num;

    return *this;
}


/*
    BigInt *= BigInt
    ----------------
*/

BigInt& BigInt::operator*=(const BigInt& num) {
    *this = *this * num;

    return *this;
}


/*
    BigInt /= BigInt
    ----------------
*/

BigInt& BigInt::operator/=(const BigInt& num) {
    *this = *this / num;

    return *this;
}


/*
    BigInt %= BigInt
    ----------------
*/

BigInt& BigInt::operator%=(const BigInt& num) {
    *this = *this % num;

    return *this;
}


/*
    BigInt += Integer
    -----------------
*/

BigInt& BigInt::operator+=(const long long& num) {
    *this = *this + BigInt(num);

    return *this;
}


/*
    BigInt -= Integer
    -----------------
*/

BigInt& BigInt::operator-=(const long long& num) {
    *this = *this - BigInt(num);

    return *this;
}


/*
    BigInt *= Integer
    -----------------
*/

BigInt& BigInt::operator*=(const long long& num) {
    *this = *this * BigInt(num);

    return *this;
}


/*
    BigInt /= Integer
    -----------------
*/

BigInt& BigInt::operator/=(const long long& num) {
    *this = *this / BigInt(num);

    return *this;
}


/*
    BigInt %= Integer
    -----------------
*/

BigInt& BigInt::operator%=(const long long& num) {
    *this = *this % BigInt(num);

    return *this;
}


/*
    BigInt += String
    ----------------
*/

BigInt& BigInt::operator+=(const std::string& num) {
    *this = *this + BigInt(num);

    return *this;
}


/*
    BigInt -= String
    ----------------
*/

BigInt& BigInt::operator-=(const std::string& num) {
    *this = *this - BigInt(num);

    return *this;
}


/*
    BigInt *= String
    ----------------
*/

BigInt& BigInt::operator*=(const std::string& num) {
    *this = *this * BigInt(num);

    return *this;
}


/*
    BigInt /= String
    ----------------
*/

BigInt& BigInt::operator/=(const std::string& num) {
    *this = *this / BigInt(num);

    return *this;
}


/*
    BigInt %= String
    ----------------
*/

BigInt& BigInt::operator%=(const std::string& num) {
    *this = *this % BigInt(num);

    return *this;
}
