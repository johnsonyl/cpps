#include "cpps_uint256.h"

cpps_uint256* cpps_uint256::_add(cpps::C *c,cpps::object _right)
{
    cpps_uint256* ret = NULL;
    cpps::newclass<cpps_uint256>(c, &ret);
    if (_right.isnumber()) {
        ret->_value = this->_value + cpps_integer(_right.tonumber());
    }
    else if (_right.isint()) {
        ret->_value = this->_value + _right.toint();
    }
    else if (_right.isstring()) {
        ret->_value = this->_value + uint256_t(_right.tostring());
    }
    else if (_right.isclassvar() && _right.getclassname() == "uint256") {
        cpps_uint256* _right_v = cpps::object_cast<cpps_uint256*>(_right);
        ret->_value = this->_value + _right_v->_value;
    }
    return ret;
}
void cpps_uint256::_constructor(cpps::C *c,cpps::object _right)
{
    if (_right.isnull()) return;

    cpps_uint256* ret = this;
    if (_right.isnumber()) {
        ret->_value =  cpps_integer(_right.tonumber());
    }
    else if (_right.isint()) {
        ret->_value =  _right.toint();
    }
    else if (_right.isstring()) {
        ret->_value =  uint256_t(_right.tostring());
    }
    else if (_right.isclassvar() && _right.getclassname() == "uint256") {
        cpps_uint256* _right_v = cpps::object_cast<cpps_uint256*>(_right);
        ret->_value = _right_v->_value;
    }
}

cpps_uint256* cpps_uint256::_sub(cpps::C* c, cpps::object _right)
{
    cpps_uint256* ret = NULL;
    cpps::newclass<cpps_uint256>(c, &ret);
    if (_right.isnumber()) {
        ret->_value = this->_value - cpps_integer(_right.tonumber());
    }
    else if (_right.isint()) {
        ret->_value = this->_value - _right.toint();
    }
    else if (_right.isstring()) {
        ret->_value = this->_value - uint256_t(_right.tostring());
    }
    else if (_right.isclassvar() && _right.getclassname() == "uint256") {
        cpps_uint256* _right_v = cpps::object_cast<cpps_uint256*>(_right);
        ret->_value = this->_value - _right_v->_value;
    }
    return ret;
}

cpps_uint256* cpps_uint256::_mul(cpps::C* c, cpps::object _right)
{
    cpps_uint256* ret = NULL;
    cpps::newclass<cpps_uint256>(c, &ret);
    if (_right.isnumber()) {
        ret->_value = this->_value * cpps_integer(_right.tonumber());
    }
    else if (_right.isint()) {
        ret->_value = this->_value * _right.toint();
    }
    else if (_right.isstring()) {
        ret->_value = this->_value * uint256_t(_right.tostring());
    }
    else if (_right.isclassvar() && _right.getclassname() == "uint256") {
        cpps_uint256* _right_v = cpps::object_cast<cpps_uint256*>(_right);
        ret->_value = this->_value * _right_v->_value;
    }
    return ret;
}

cpps_uint256* cpps_uint256::_div(cpps::C* c, cpps::object _right)
{
    cpps_uint256* ret = NULL;
    cpps::newclass<cpps_uint256>(c, &ret);
    if (_right.isnumber()) {
        ret->_value = this->_value / cpps_integer(_right.tonumber());
    }
    else if (_right.isint()) {
        ret->_value = this->_value / _right.toint();
    }
    else if (_right.isstring()) {
        ret->_value = this->_value / uint256_t(_right.tostring());
    }
    else if (_right.isclassvar() && _right.getclassname() == "uint256") {
        cpps_uint256* _right_v = cpps::object_cast<cpps_uint256*>(_right);
        ret->_value = this->_value / _right_v->_value;
    }
    return ret;
}

cpps_uint256* cpps_uint256::_add2(cpps::C* c, cpps::object _right)
{
    cpps_uint256* ret = NULL;
    cpps::newclass<cpps_uint256>(c, &ret);
    if (_right.isnumber()) {
        ret->_value = this->_value & cpps_integer(_right.tonumber());
    }
    else if (_right.isint()) {
        ret->_value = this->_value & _right.toint();
    }
    else if (_right.isstring()) {
        ret->_value = this->_value & uint256_t(_right.tostring());
    }
    else if (_right.isclassvar() && _right.getclassname() == "uint256") {
        cpps_uint256* _right_v = cpps::object_cast<cpps_uint256*>(_right);
        ret->_value = this->_value & _right_v->_value;
    }
    return ret;
}

cpps_uint256* cpps_uint256::_add2andassignment(cpps::C* c, cpps::object _right)
{
    cpps_uint256* ret = this;
    if (_right.isnumber()) {
        ret->_value = this->_value & cpps_integer(_right.tonumber());
    }
    else if (_right.isint()) {
        ret->_value = this->_value & _right.toint();
    }
    else if (_right.isstring()) {
        ret->_value = this->_value & uint256_t(_right.tostring());
    }
    else if (_right.isclassvar() && _right.getclassname() == "uint256") {
        cpps_uint256* _right_v = cpps::object_cast<cpps_uint256*>(_right);
        ret->_value = this->_value & _right_v->_value;
    }
    return ret;
}

cpps_uint256* cpps_uint256::_add3andassignment(cpps::C* c, cpps::object _right)
{
    cpps_uint256* ret = this;
    if (_right.isnumber()) {
        ret->_value = this->_value | cpps_integer(_right.tonumber());
    }
    else if (_right.isint()) {
        ret->_value = this->_value | _right.toint();
    }
    else if (_right.isstring()) {
        ret->_value = this->_value | uint256_t(_right.tostring());
    }
    else if (_right.isclassvar() && _right.getclassname() == "uint256") {
        cpps_uint256* _right_v = cpps::object_cast<cpps_uint256*>(_right);
        ret->_value = this->_value | _right_v->_value;
    }
    return ret;
}

cpps_uint256* cpps_uint256::_add3(cpps::C* c, cpps::object _right)
{
    cpps_uint256* ret = NULL;
    cpps::newclass<cpps_uint256>(c, &ret);
    if (_right.isnumber()) {
        ret->_value = this->_value | cpps_integer(_right.tonumber());
    }
    else if (_right.isint()) {
        ret->_value = this->_value | _right.toint();
    }
    else if (_right.isstring()) {
        ret->_value = this->_value | uint256_t(_right.tostring());
    }
    else if (_right.isclassvar() && _right.getclassname() == "uint256") {
        cpps_uint256* _right_v = cpps::object_cast<cpps_uint256*>(_right);
        ret->_value = this->_value | _right_v->_value;
    }
    return ret;
}

cpps_uint256* cpps_uint256::_quyu(cpps::C* c, cpps::object _right)
{
    cpps_uint256* ret = NULL;
    cpps::newclass<cpps_uint256>(c, &ret);
    if (_right.isnumber()) {
        ret->_value = this->_value % cpps_integer(_right.tonumber());
    }
    else if (_right.isint()) {
        ret->_value = this->_value % _right.toint();
    }
    else if (_right.isstring()) {
        ret->_value = this->_value % uint256_t(_right.tostring());
    }
    else if (_right.isclassvar() && _right.getclassname() == "uint256") {
        cpps_uint256* _right_v = cpps::object_cast<cpps_uint256*>(_right);
        ret->_value = this->_value % _right_v->_value;
    }
    return ret;
}

cpps_uint256* cpps_uint256::_assignment(cpps::C* c, cpps::object _right)
{
    cpps_uint256* ret = this;
    if (_right.isnumber()) {
        ret->_value = cpps_integer(_right.tonumber());
    }
    else if (_right.isint()) {
        ret->_value = _right.toint();
    }
    else if (_right.isstring()) {
        ret->_value = uint256_t(_right.tostring());
    }
    else if (_right.isclassvar() && _right.getclassname() == "uint256") {
        cpps_uint256* _right_v = cpps::object_cast<cpps_uint256*>(_right);
        ret->_value = _right_v->_value;
    }
    return ret;
}

bool cpps_uint256::_less(cpps::C* c, cpps::object _right)
{
    bool ret = false;
    if (_right.isnumber()) {
        ret = this->_value < cpps_integer(_right.tonumber());
    }
    else if (_right.isint()) {
        ret = this->_value < _right.toint();
    }
    else if (_right.isstring()) {
        ret = this->_value < uint256_t(_right.tostring());
    }
    else if (_right.isclassvar() && _right.getclassname() == "uint256") {
        cpps_uint256* _right_v = cpps::object_cast<cpps_uint256*>(_right);
        ret = this->_value < _right_v->_value;
    }
    return ret;
}

bool cpps_uint256::_bigger(cpps::C* c, cpps::object _right)
{
    bool ret = false;
    if (_right.isnumber()) {
        ret = this->_value > cpps_integer(_right.tonumber());
    }
    else if (_right.isint()) {
        ret = this->_value > _right.toint();
    }
    else if (_right.isstring()) {
        ret = this->_value > uint256_t(_right.tostring());
    }
    else if (_right.isclassvar() && _right.getclassname() == "uint256") {
        cpps_uint256* _right_v = cpps::object_cast<cpps_uint256*>(_right);
        ret = this->_value > _right_v->_value;
    }
    return ret;
}

bool cpps_uint256::_biggerorequel(cpps::C* c, cpps::object _right)
{
    bool ret = false;
    if (_right.isnumber()) {
        ret = this->_value >= cpps_integer(_right.tonumber());
    }
    else if (_right.isint()) {
        ret = this->_value >= _right.toint();
    }
    else if (_right.isstring()) {
        ret = this->_value >= uint256_t(_right.tostring());
    }
    else if (_right.isclassvar() && _right.getclassname() == "uint256") {
        cpps_uint256* _right_v = cpps::object_cast<cpps_uint256*>(_right);
        ret = this->_value >= _right_v->_value;
    }
    return ret;
}

bool cpps_uint256::_equel(cpps::C* c, cpps::object _right)
{
    bool ret = false;
    if (_right.isnumber()) {
        ret = this->_value == cpps_integer(_right.tonumber());
    }
    else if (_right.isint()) {
        ret = this->_value == _right.toint();
    }
    else if (_right.isstring()) {
        ret = this->_value == uint256_t(_right.tostring());
    }
    else if (_right.isclassvar() && _right.getclassname() == "uint256") {
        cpps_uint256* _right_v = cpps::object_cast<cpps_uint256*>(_right);
        ret = this->_value == _right_v->_value;
    }
    return ret;
}

bool cpps_uint256::_lessandequel(cpps::C* c, cpps::object _right)
{
    bool ret = false;
    if (_right.isnumber()) {
        ret = this->_value <= cpps_integer(_right.tonumber());
    }
    else if (_right.isint()) {
        ret = this->_value <= _right.toint();
    }
    else if (_right.isstring()) {
        ret = this->_value <= uint256_t(_right.tostring());
    }
    else if (_right.isclassvar() && _right.getclassname() == "uint256") {
        cpps_uint256* _right_v = cpps::object_cast<cpps_uint256*>(_right);
        ret = this->_value <= _right_v->_value;
    }
    return ret;
}

cpps_uint256* cpps_uint256::_rightautoincrease(cpps::C* c)
{
    cpps_uint256* ret = NULL;
    cpps::newclass<cpps_uint256>(c, &ret);
    ret->_value = this->_value++;

    return ret;
}

cpps_uint256* cpps_uint256::_rightautodecrease(cpps::C* c)
{
    cpps_uint256* ret = NULL;
    cpps::newclass<cpps_uint256>(c, &ret);
    ret->_value = this->_value--;
    return ret;
}

cpps_uint256* cpps_uint256::_leftautoincrease(cpps::C* c)
{
    cpps_uint256* ret = NULL;
    cpps::newclass<cpps_uint256>(c, &ret);
    ret->_value = ++this->_value;
    return ret;
}

cpps_uint256* cpps_uint256::_leftautodecrease(cpps::C* c)
{
    cpps_uint256* ret = NULL;
    cpps::newclass<cpps_uint256>(c, &ret);
    ret->_value = this->_value--;
    return ret;
}

cpps_uint256* cpps_uint256::_add4(cpps::C* c, cpps::object _right)
{
    cpps_uint256* ret = NULL;
    cpps::newclass<cpps_uint256>(c, &ret);
    if (_right.isnumber()) {
        ret->_value = this->_value ^ cpps_integer(_right.tonumber());
    }
    else if (_right.isint()) {
        ret->_value = this->_value ^ _right.toint();
    }
    else if (_right.isstring()) {
        ret->_value = this->_value ^ uint256_t(_right.tostring());
    }
    else if (_right.isclassvar() && _right.getclassname() == "uint256") {
        cpps_uint256* _right_v = cpps::object_cast<cpps_uint256*>(_right);
        ret->_value = this->_value ^ _right_v->_value;
    }
    return ret;
}

cpps_uint256* cpps_uint256::_add4andassignment(cpps::C* c, cpps::object _right)
{
    cpps_uint256* ret = this;
    if (_right.isnumber()) {
        ret->_value = this->_value ^ cpps_integer(_right.tonumber());
    }
    else if (_right.isint()) {
        ret->_value = this->_value ^ _right.toint();
    }
    else if (_right.isstring()) {
        ret->_value = this->_value ^ uint256_t(_right.tostring());
    }
    else if (_right.isclassvar() && _right.getclassname() == "uint256") {
        cpps_uint256* _right_v = cpps::object_cast<cpps_uint256*>(_right);
        ret->_value = this->_value ^ _right_v->_value;
    }
    return ret;
}

bool cpps_uint256::_notequel(cpps::C* c, cpps::object _right)
{
    bool ret = false;
    if (_right.isnumber()) {
        ret = this->_value != cpps_integer(_right.tonumber());
    }
    else if (_right.isint()) {
        ret = this->_value != _right.toint();
    }
    else if (_right.isstring()) {
        ret = this->_value != uint256_t(_right.tostring());
    }
    else if (_right.isclassvar() && _right.getclassname() == "uint256") {
        cpps_uint256* _right_v = cpps::object_cast<cpps_uint256*>(_right);
        ret = this->_value != _right_v->_value;
    }
    return ret;
}

cpps_uint256* cpps_uint256::_add5andassignment(cpps::C* c, cpps::object _right)
{
    cpps_uint256* ret = this;
    if (_right.isnumber()) {
        ret->_value = this->_value << cpps_integer(_right.tonumber());
    }
    else if (_right.isint()) {
        ret->_value = this->_value << _right.toint();
    }
    else if (_right.isstring()) {
        ret->_value = this->_value << uint256_t(_right.tostring());
    }
    else if (_right.isclassvar() && _right.getclassname() == "uint256") {
        cpps_uint256* _right_v = cpps::object_cast<cpps_uint256*>(_right);
        ret->_value = this->_value <<= _right_v->_value;
    }
    return ret;
}

cpps_uint256* cpps_uint256::_add6andassignment(cpps::C* c, cpps::object _right)
{
    cpps_uint256* ret = this;
    if (_right.isnumber()) {
        ret->_value = this->_value >> cpps_integer(_right.tonumber());
    }
    else if (_right.isint()) {
        ret->_value = this->_value >> _right.toint();
    }
    else if (_right.isstring()) {
        ret->_value = this->_value >> uint256_t(_right.tostring());
    }
    else if (_right.isclassvar() && _right.getclassname() == "uint256") {
        cpps_uint256* _right_v = cpps::object_cast<cpps_uint256*>(_right);
        ret->_value = this->_value >> _right_v->_value;
    }
    return ret;
}

cpps_uint256* cpps_uint256::_mulandassignment(cpps::C* c, cpps::object _right)
{
    cpps_uint256* ret = this;
    if (_right.isnumber()) {
        ret->_value = this->_value * cpps_integer(_right.tonumber());
    }
    else if (_right.isint()) {
        ret->_value = this->_value * _right.toint();
    }
    else if (_right.isstring()) {
        ret->_value = this->_value * uint256_t(_right.tostring());
    }
    else if (_right.isclassvar() && _right.getclassname() == "uint256") {
        cpps_uint256* _right_v = cpps::object_cast<cpps_uint256*>(_right);
        ret->_value = this->_value * _right_v->_value;
    }
    return ret;
}

cpps_uint256* cpps_uint256::_divandassignment(cpps::C* c, cpps::object _right)
{
    cpps_uint256* ret = this;
    if (_right.isnumber()) {
        ret->_value = this->_value / cpps_integer(_right.tonumber());
    }
    else if (_right.isint()) {
        ret->_value = this->_value / _right.toint();
    }
    else if (_right.isstring()) {
        ret->_value = this->_value / uint256_t(_right.tostring());
    }
    else if (_right.isclassvar() && _right.getclassname() == "uint256") {
        cpps_uint256* _right_v = cpps::object_cast<cpps_uint256*>(_right);
        ret->_value = this->_value / _right_v->_value;
    }
    return ret;
}

cpps_uint256* cpps_uint256::_addandassignment(cpps::C* c, cpps::object _right)
{
    cpps_uint256* ret = this;
    if (_right.isnumber()) {
        ret->_value = this->_value + cpps_integer(_right.tonumber());
    }
    else if (_right.isint()) {
        ret->_value = this->_value + _right.toint();
    }
    else if (_right.isstring()) {
        ret->_value = this->_value + uint256_t(_right.tostring());
    }
    else if (_right.isclassvar() && _right.getclassname() == "uint256") {
        cpps_uint256* _right_v = cpps::object_cast<cpps_uint256*>(_right);
        ret->_value = this->_value + _right_v->_value;
    }
    return ret;
}

cpps_uint256* cpps_uint256::_subandassignment(cpps::C* c, cpps::object _right)
{
    cpps_uint256* ret = this;
    if (_right.isnumber()) {
        ret->_value = this->_value - cpps_integer(_right.tonumber());
    }
    else if (_right.isint()) {
        ret->_value = this->_value - _right.toint();
    }
    else if (_right.isstring()) {
        ret->_value = this->_value - uint256_t(_right.tostring());
    }
    else if (_right.isclassvar() && _right.getclassname() == "uint256") {
        cpps_uint256* _right_v = cpps::object_cast<cpps_uint256*>(_right);
        ret->_value = this->_value - _right_v->_value;
    }
    return ret;
}

cpps_uint256* cpps_uint256::_add5(cpps::C* c, cpps::object _right)
{
    cpps_uint256* ret = NULL;
    cpps::newclass<cpps_uint256>(c, &ret);
    if (_right.isnumber()) {
        ret->_value = this->_value << cpps_integer(_right.tonumber());
    }
    else if (_right.isint()) {
        ret->_value = this->_value << _right.toint();
    }
    else if (_right.isstring()) {
        ret->_value = this->_value << uint256_t(_right.tostring());
    }
    else if (_right.isclassvar() && _right.getclassname() == "uint256") {
        cpps_uint256* _right_v = cpps::object_cast<cpps_uint256*>(_right);
        ret->_value = this->_value << _right_v->_value;
    }
    return ret;
}

cpps_uint256* cpps_uint256::_add6(cpps::C* c, cpps::object _right)
{
    cpps_uint256* ret = NULL;
    cpps::newclass<cpps_uint256>(c, &ret);
    if (_right.isnumber()) {
        ret->_value = this->_value >> cpps_integer(_right.tonumber());
    }
    else if (_right.isint()) {
        ret->_value = this->_value >> _right.toint();
    }
    else if (_right.isstring()) {
        ret->_value = this->_value >> uint256_t(_right.tostring());
    }
    else if (_right.isclassvar() && _right.getclassname() == "uint256") {
        cpps_uint256* _right_v = cpps::object_cast<cpps_uint256*>(_right);
        ret->_value = this->_value >> _right_v->_value;
    }
    return ret;
}

cpps_uint256* cpps_uint256::_minus(cpps::C* c)
{
    cpps_uint256* ret = NULL;
    cpps::newclass<cpps_uint256>(c, &ret);
    ret->_value = -this->_value;
    return ret;
}

cpps_uint256* cpps_uint256::_plus(cpps::C* c)
{
    cpps_uint256* ret = NULL;
    cpps::newclass<cpps_uint256>(c, &ret);
    ret->_value = +this->_value;
    return ret;
}

bool cpps_uint256::_not(cpps::C* c)
{
    bool ret = false;
    ret = !this->_value;
    return ret;
}

cpps_uint256* cpps_uint256::_not2(cpps::C* c)
{
    cpps_uint256* ret = NULL;
    cpps::newclass<cpps_uint256>(c, &ret);
    ret->_value = ~this->_value;
    return ret;
}

std::string cpps_uint256::_tostring()
{
    return this->_value.tostring();
}

std::string cpps_uint256::_tohex()
{
    return this->_value.tohex();
}

std::string cpps_uint256::_todec()
{
    return this->_value.todec();
}

cpps_integer cpps_uint256::_toint()
{
    return (cpps_integer)this->_value;
}

cpps_integer cpps_uint256::_hash_value()
{
    return std::hash<std::string>()(_tostring());
}
