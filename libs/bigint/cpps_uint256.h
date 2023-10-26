#pragma once
#include "Integer.hpp"
#include <cpps/cpps.h>

using uint256_t = JIO::Integer<32, false>;
class cpps_uint256
{
public:

	cpps_uint256() {
	}
	// +
	cpps_uint256 * _add(cpps::C* c, cpps::object _right);
	//_constructor
	void _constructor(cpps::C* c, cpps::object _right);
	// -
	cpps_uint256 * _sub(cpps::C* c, cpps::object _right);
	// *
	cpps_uint256 * _mul(cpps::C* c, cpps::object _right);
	// /
	cpps_uint256 * _div(cpps::C* c, cpps::object _right);
	// &
	cpps_uint256 * _add2(cpps::C* c, cpps::object _right);
	// &=
	cpps_uint256 * _add2andassignment(cpps::C* c, cpps::object _right);
	// |=
	cpps_uint256 * _add3andassignment(cpps::C* c, cpps::object _right);
	// |
	cpps_uint256 * _add3(cpps::C* c, cpps::object _right);
	// %
	cpps_uint256 * _quyu(cpps::C* c, cpps::object _right);
	// =
	cpps_uint256 * _assignment(cpps::C* c, cpps::object _right);
	// <
	bool			_less(cpps::C* c, cpps::object _right);
	// >
	bool			_bigger(cpps::C* c, cpps::object _right);
	// >=
	bool			_biggerorequel(cpps::C* c, cpps::object _right);
	// ==
	bool			_equel(cpps::C* c, cpps::object _right);
	// <=
	bool			_lessandequel(cpps::C* c, cpps::object _right);
	// ++
	cpps_uint256 * _rightautoincrease(cpps::C* c);
	// --
	cpps_uint256 * _rightautodecrease(cpps::C* c);
	// ++left
	cpps_uint256 * _leftautoincrease(cpps::C* c);
	// --left
	cpps_uint256 * _leftautodecrease(cpps::C* c);
	// ^
	cpps_uint256 * _add4(cpps::C* c, cpps::object _right);
	// ^=
	cpps_uint256 * _add4andassignment(cpps::C* c, cpps::object _right);
	// !=
	bool			_notequel(cpps::C* c, cpps::object _right);
	// <<=
	cpps_uint256 * _add5andassignment(cpps::C* c, cpps::object _right);
	// >>=
	cpps_uint256 * _add6andassignment(cpps::C* c, cpps::object _right);
	// *=
	cpps_uint256 * _mulandassignment(cpps::C* c, cpps::object _right);
	// /=
	cpps_uint256 * _divandassignment(cpps::C* c, cpps::object _right);
	// += 
	cpps_uint256 * _addandassignment(cpps::C* c, cpps::object _right);
	// -= 
	cpps_uint256 * _subandassignment(cpps::C* c, cpps::object _right);
	// <<
	cpps_uint256 * _add5(cpps::C* c, cpps::object _right);
	// >>
	cpps_uint256 * _add6(cpps::C* c, cpps::object _right);
	// -left
	cpps_uint256 * _minus(cpps::C* c);
	// +left
	cpps_uint256 * _plus(cpps::C* c);
	// !left
	bool			_not(cpps::C* c);
	// ~left
	cpps_uint256 * _not2(cpps::C* c);
	//tostring
	std::string _tostring();
	//tostring
	std::string _tohex();
	//tostring
	std::string _todec();
	//toint
	cpps_integer _toint();
	//hash_value
	cpps_integer _hash_value();

	
	uint256_t _value;
};

