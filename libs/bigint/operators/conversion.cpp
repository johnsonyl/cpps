/*
    ===========================================================================
    Conversion functions for BigInt
    ===========================================================================
*/

#ifndef BIG_INT_CONVERSION_FUNCTIONS_HPP
#define BIG_INT_CONVERSION_FUNCTIONS_HPP

#include <string>
#include "../BigInt.h"
#include "../functions/math.h"
/*
    to_string
    ---------
    Converts a BigInt to a string.
*/

std::string BigInt::to_string() const {
    // prefix with sign if negative
    return this->sign == '-' ? "-" + this->value : this->value;
}


/*
    to_int
    ------
    Converts a BigInt to an int.
    NOTE: If the BigInt is out of range of an int, stoi() will throw an
    out_of_range exception.
*/

int BigInt::to_int() const {
    return std::stoi(this->to_string());
}


/*
    to_long
    -------
    Converts a BigInt to a long int.
    NOTE: If the BigInt is out of range of a long int, stol() will throw an
    out_of_range exception.
*/

long BigInt::to_long() const {
    return std::stol(this->to_string());
}


/*
    to_long_long
    ------------
    Converts a BigInt to a long long int.
    NOTE: If the BigInt is out of range of a long long int, stoll() will throw
    an out_of_range exception.
*/

long long BigInt::to_long_long() const {
    return std::stoll(this->to_string());
}

/*
	to_hex
	------------
	Converts a BigInt to a hex.
*/
void hex_calculation(BigInt v, std::string &_out)
{
	BigInt ret = v / 16;
	BigInt v2 = v % 16;
	_out.insert(_out.begin(), tolower(ToHex(v2.to_int())));
	if (ret > 0)
		hex_calculation(ret, _out);
}

std::string BigInt::to_hex() const {
	std::string _out;
	hex_calculation(*this, _out);
	return _out;
}
#endif  // BIG_INT_CONVERSION_FUNCTIONS_HPP
