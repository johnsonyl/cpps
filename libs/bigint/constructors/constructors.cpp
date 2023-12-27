/*
    ===========================================================================
    Constructors
    ===========================================================================
*/

#include <string>
#include "../BigInt.h"
#include "../functions/utility.h"
#include "../functions/math.h"


/*
    Default constructor
    -------------------
*/

BigInt::BigInt() {
    value = "0";
    sign = '+';
}


/*
    Copy constructor
    ----------------
*/

BigInt::BigInt(const BigInt& num) {
    value = num.value;
    sign = num.sign;
}


/*
    Integer to BigInt
    -----------------
*/

BigInt::BigInt(const long long& num) {
    value = std::to_string(std::abs(num));
    if (num < 0)
        sign = '-';
    else
        sign = '+';
}


/*
    String to BigInt
    ----------------
*/
void	From_Hex(BigInt& _v, std::string _hex)
{
	static BigInt v2 = 16;
	int hexcount = (int)_hex.size();
	for (size_t i = 0; i < _hex.size(); i++) {
		char c = tolower(_hex[i]);
		int _pow = hexcount - (int)(i+1);
		_v +=  pow(v2, _pow) * FromHex(c);
	}
}

BigInt::BigInt(const std::string& num) {
	if (num[0] == '0' and num[1] == 'x') {
		From_Hex(*this, num.substr(2));
	}
    else if (num[0] == '+' or num[0] == '-') {     // check for sign
        std::string magnitude = num.substr(1);
        if (is_valid_number(magnitude)) {
            value = magnitude;
            sign = num[0];
        }
        else {
            throw std::invalid_argument("Expected an integer, got \'" + num + "\'");
        }
    }
    else {      // if no sign is specified
        if (is_valid_number(num)) {
            value = num;
            sign = '+';    // positive by default
        }
        else {
            throw std::invalid_argument("Expected an integer, got \'" + num + "\'");
        }
    }
    strip_leading_zeroes(value);
}
