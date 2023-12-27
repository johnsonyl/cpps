/*
    ===========================================================================
    Math functions for BigInt
    ===========================================================================
*/

#ifndef BIG_INT_MATH_FUNCTIONS_HPP
#define BIG_INT_MATH_FUNCTIONS_HPP

#include <string>

#include "../BigInt.h"


/*
    abs
    ---
    Returns the absolute value of a BigInt.
*/
inline 
BigInt abs(const BigInt& num) {
    return num < 0 ? -num : num;
}


/*
    big_pow10
    ---------
    Returns a BigInt equal to 10^exp.
    NOTE: exponent should be a non-negative integer.
*/
inline
BigInt big_pow10(size_t exp) {
    return BigInt("1" + std::string(exp, '0'));
}


/*
    pow (BigInt)
    ------------
    Returns a BigInt equal to base^exp.
*/
inline
BigInt pow(const BigInt& base, int exp) {
    if (exp < 0) {
        if (base == 0)
            throw std::logic_error("Cannot divide by zero");
        return abs(base) == 1 ? base : 0;
    }
    if (exp == 0) {
        if (base == 0)
            throw std::logic_error("Zero cannot be raised to zero");
        return 1;
    }

    BigInt result = base, result_odd = 1;
    while (exp > 1) {
        if (exp % 2)
            result_odd *= result;
        result *= result;
        exp /= 2;
    }

    return result * result_odd;
}


/*
    pow (Integer)
    -------------
    Returns a BigInt equal to base^exp.
*/
inline
BigInt pow(const long long& base, int exp) {
    return pow(BigInt(base), exp);
}


/*
    pow (String)
    ------------
    Returns a BigInt equal to base^exp.
*/
inline
BigInt pow(const std::string& base, int exp) {
    return pow(BigInt(base), exp);

}


/*
    sqrt
    ----
    Returns the positive integer square root of a BigInt using Newton's method.
    NOTE: the input must be non-negative.
*/
inline
BigInt sqrt(const BigInt& num) {
    if (num < 0)
        throw std::invalid_argument("Cannot compute square root of a negative integer");

    // Optimisations for small inputs:
    if (num == 0)
        return 0;
    else if (num < 4)
        return 1;
    else if (num < 9)
        return 2;
    else if (num < 16)
        return 3;

    BigInt sqrt_prev = -1;
    // The value for `sqrt_current` is chosen close to that of the actual
    // square root.
    // Since a number's square root has at least one less than half as many
    // digits as the number,
    //     sqrt_current = 10^(half_the_digits_in_num - 1)
    BigInt sqrt_current = big_pow10(num.to_string().size() / 2 - 1);

    while (abs(sqrt_current - sqrt_prev) > 1) {
        sqrt_prev = sqrt_current;
        sqrt_current = (num / sqrt_prev + sqrt_prev) / 2;
    }

    return sqrt_current;
}


/*
    gcd(BigInt, BigInt)
    -------------------
    Returns the greatest common divisor (GCD, a.k.a. HCF) of two BigInts using
    Euclid's algorithm.
*/
inline
BigInt gcd(const BigInt &num1, const BigInt &num2){
    BigInt abs_num1 = abs(num1);
    BigInt abs_num2 = abs(num2);

    // base cases:
    if (abs_num2 == 0)
        return abs_num1;    // gcd(a, 0) = |a|
    if (abs_num1 == 0)
        return abs_num2;    // gcd(0, a) = |a|

    BigInt remainder = abs_num2;
    while (remainder != 0) {
        remainder = abs_num1 % abs_num2;
        abs_num1 = abs_num2;    // previous remainder
        abs_num2 = remainder;   // current remainder
    }

    return abs_num1;
}


/*
    gcd(BigInt, Integer)
    --------------------
*/
inline
BigInt gcd(const BigInt& num1, const long long& num2){
    return gcd(num1, BigInt(num2));
}


/*
    gcd(BigInt, String)
    -------------------
*/
inline
BigInt gcd(const BigInt& num1, const std::string& num2){
    return gcd(num1, BigInt(num2));
}


/*
    gcd(Integer, BigInt)
    --------------------
*/
inline
BigInt gcd(const long long& num1, const BigInt& num2){
    return gcd(BigInt(num1), num2);
}


/*
    gcd(String, BigInt)
    -------------------
*/
inline
BigInt gcd(const std::string& num1, const BigInt& num2){
    return gcd(BigInt(num1), num2);
}


/*
    lcm(BigInt, BigInt)
    -------------------
    Returns the least common multiple (LCM) of two BigInts.
*/
inline
BigInt lcm(const BigInt& num1, const BigInt& num2) {
    if (num1 == 0 or num2 == 0)
        return 0;

    return abs(num1 * num2) / gcd(num1, num2);
}


/*
    lcm(BigInt, Integer)
    --------------------
*/
inline
BigInt lcm(const BigInt& num1, const long long& num2){
    return lcm(num1, BigInt(num2));
}


/*
    lcm(BigInt, String)
    -------------------
*/
inline
BigInt lcm(const BigInt& num1, const std::string& num2){
    return lcm(num1, BigInt(num2));
}


/*
    lcm(Integer, BigInt)
    --------------------
*/
inline
BigInt lcm(const long long& num1, const BigInt& num2){
    return lcm(BigInt(num1), num2);
}


/*
    lcm(String, BigInt)
    -------------------
*/
inline
BigInt lcm(const std::string& num1, const BigInt& num2){
    return lcm(BigInt(num1), num2);
}
inline
unsigned char ToHex(unsigned char x)
{
	return  x > 9 ? x + 55 : x + 48;
}
inline
unsigned char FromHex(unsigned char x)
{
	unsigned char y = 0;
	if (x >= 'A' && x <= 'Z') y = x - 'A' + 10;
	else if (x >= 'a' && x <= 'z') y = x - 'a' + 10;
	else if (x >= '0' && x <= '9') y = x - '0';
	return y;
}


#endif  // BIG_INT_MATH_FUNCTIONS_HPP
