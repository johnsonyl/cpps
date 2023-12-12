#include "cpps/cpps.h"

namespace cpps
{
	object doclassfunction(C* c, object domain, object func);

	bool cpps_to_bool(const cpps_value& src)
	{
		const cpps_value& obj = src.real();
		bool ret = 0;
		if (obj.tt == CPPS_TNUMBER)
		{
			ret = obj.value.number;
		}
		if (obj.tt == CPPS_TBOOLEAN)
		{
			ret = obj.value.b;
		}
		else if (obj.tt == CPPS_TINTEGER)
		{
			ret = obj.value.integer;
		}
		else if (obj.tt == CPPS_TUINTEGER)
		{
			ret = obj.value.uinteger;
		}
		else if (obj.tt == CPPS_TSTRING)
		{
			std::string* tmpStr = cpps_get_string(obj);
			ret = tmpStr->size();
		}
		else {
			ret = obj.tt != CPPS_TNIL;
		}
		return ret;
	}
	cpps_number cpps_to_number(const cpps_value& src)
	{
		const cpps_value& obj = src.real();
		cpps_number ret = 0;
		if (obj.tt == CPPS_TNUMBER)
		{
			ret = obj.value.number;
		}
		else if (obj.tt == CPPS_TINTEGER)
		{
			ret = cpps_integer2number(obj.value.integer);
		}
		else if (obj.tt == CPPS_TUINTEGER)
		{
			ret = cpps_integer2number(obj.value.uinteger);
		}
		else if (obj.tt == CPPS_TSTRING)
		{

			std::string* tmpStr = cpps_get_string(obj);

			cpps_str2d(tmpStr->c_str(), &ret);
		}
		return ret;
	}

	cpps_integer cpps_to_integer(const cpps_value& src)
	{
		const cpps_value& obj = src.real();
		cpps_integer ret = 0;
		if (obj.tt == CPPS_TNUMBER)
		{
			ret = cpps_number2integer(obj.value.number);
		}
		else if (obj.tt == CPPS_TINTEGER)
		{
			ret = obj.value.integer;
		}
		else if (obj.tt == CPPS_TUINTEGER)
		{
			ret = obj.value.integer;
		}
		else if (obj.tt == CPPS_TSTRING)
		{
			std::string* tmpStr = cpps_get_string(obj);
			cpps_str2i64(tmpStr->c_str(), &ret);
		}
		return ret;
	}
	cpps_uinteger cpps_to_uinteger(const cpps_value& src)
	{
		const cpps_value& obj = src.real();
		cpps_uinteger ret = 0;
		if (obj.tt == CPPS_TUINTEGER)
			ret = obj.value.uinteger;
		else
			ret = (cpps_uinteger)cpps_to_integer(src);
		return ret;
	}
	std::string* cpps_get_string(const cpps_value& src)
	{
		const cpps_value& obj = src.real();
		if (obj.tt == CPPS_TSTRING)
		{
			cpps_cppsclassvar* cppsclassvar = (cpps_cppsclassvar*)obj.value.domain;
			cpps::string* tmpStr = (cpps::string*)cppsclassvar->getclsptr();
			return &tmpStr->__str;
		}

		return NULL;
	}
	std::string cpps_to_string(const cpps_value& src)
	{
		const cpps_value& obj = src.real();
		if (obj.tt == CPPS_TNUMBER)
		{
			return std::to_string( obj.value.number);
		}
		else if (obj.tt == CPPS_TINTEGER)
		{
			return std::to_string(obj.value.integer);
		}
		else if (obj.tt == CPPS_TUINTEGER)
		{
			return std::to_string(obj.value.uinteger);
		}
		else if (obj.tt == CPPS_TSTRING)
		{
			cpps_cppsclassvar* cppsclassvar = (cpps_cppsclassvar*)obj.value.domain;
			cpps::string* tmpStr = (cpps::string*)cppsclassvar->getclsptr();
			return tmpStr->__str;
		}
		else if (obj.tt == CPPS_TBOOLEAN)
		{
			return (obj.value.b == 0 ? "false" : "true");
		}

		return "";
	}


	cpps_vector* cpps_to_cpps_vector(const cpps_value& src)
	{
		const cpps_value& obj = src.real();
		if (!obj.isdomain()) return NULL;
		cpps_cppsclassvar* cppsclassvar = (cpps_cppsclassvar*)obj.value.domain;
		cpps_vector* ret = (cpps_vector*)cppsclassvar->getclsptr();
		return ret;
	}
	cpps_map* cpps_to_cpps_map(const cpps_value& src)
	{
		const cpps_value& obj = src.real();
		if (!obj.isdomain()) return NULL;
		cpps_cppsclassvar* cppsclassvar = (cpps_cppsclassvar*)obj.value.domain;
		cpps_map* ret = (cpps_map*)cppsclassvar->getclsptr();
		return ret;
	}
	cpps_set* cpps_to_cpps_set(const cpps_value& src)
	{
		const cpps_value& obj = src.real();
		if (!obj.isdomain()) return NULL;
		cpps_cppsclassvar* cppsclassvar = (cpps_cppsclassvar*)obj.value.domain;
		cpps_set* ret = (cpps_set*)cppsclassvar->getclsptr();
		return ret;
	}

	cpps::cpps_pair* cpps_to_cpps_pair(const cpps_value& src)
	{
		const cpps_value& obj = src.real();
		if (!obj.isdomain()) return NULL;
		cpps_cppsclassvar* cppsclassvar = (cpps_cppsclassvar*)obj.value.domain;
		cpps_pair* ret = (cpps_pair*)cppsclassvar->getclsptr();
		return ret;
	}

	cpps::cpps_domain* cpps_to_domain(const cpps_value& src)
	{
		const cpps_value& obj = src.real();
		if (!obj.isdomain()) return NULL;
		return obj.value.domain;
	}

	cpps_cppsclass* cpps_to_cpps_cppsclass(const cpps_value& src)
	{
		const cpps_value& obj = src.real();
		if (!obj.isdomain()) return NULL;
		cpps_cppsclass* cppsclassvar = (cpps_cppsclass*)obj.value.domain;
		return cppsclassvar;
	}

	cpps::cpps_cppsclassvar* cpps_to_cpps_cppsclassvar(const cpps_value& src)
	{
		const cpps_value& obj = src.real();
		if (!obj.isdomain()) return NULL;
		cpps_cppsclassvar* cppsclassvar = (cpps_cppsclassvar*)obj.value.domain;
		return cppsclassvar;
	}

	bool cpps_value::operator<(const cpps_value& right) const
	{
		if (tt == right.tt)
		{
			switch (tt)
			{
			case CPPS_TNUMBER: return value.number < right.value.number;
			case CPPS_TINTEGER: return value.integer < right.value.integer;
			case CPPS_TUINTEGER: return value.uinteger < right.value.uinteger;
			case CPPS_TSTRING: {
				std::string* tmpStr = cpps_get_string(*this);
				std::string* tmpStr2 = cpps_get_string(right);
				return (*(tmpStr)) < (*(tmpStr2));
			}
			case CPPS_TFUNCTION:
			case CPPS_TLAMBDAFUNCTION:
			{
				return value.uinteger < right.value.uinteger;
			}
			case CPPS_TCLASSVAR: {
				object left = object(*this);
				cpps_cppsclassvar* cppsclassvar = cpps_to_cpps_cppsclassvar(*this);
				cpps_cppsclass* cppsclass = cppsclassvar->getcppsclass();
				C* c = cppsclass->getDefaultCState();
				cpps_function* func = cppsclass->getoperator("<");
				if (func) {
					object right_object = object(right);
					cpps_cppsclassvar* right_cppsclassvar = cpps_to_cpps_cppsclassvar(right);
					cpps_cppsclass* right_cppsclass = right_cppsclassvar->getcppsclass();
					if (right_cppsclass != cppsclass) return cppsclass < right_cppsclass;
					object symbolfunc = cpps_value(func);
					cpps::cpps_value ret;
					if (func->getIsNeedC()) {
						ret = doclassfunction(c, left, symbolfunc, cpps::object::create(c, c), right).getval();
					}
					else {
						ret = doclassfunction(c, left, symbolfunc, right).getval();
					}
					return cpps_to_bool(ret);
				}
			}
			default:
				throw(cpps_error("unknow file", 0, 0, "<   ... It can't be used as a key.type: %d", right.tt));
			}
		}
		return tt < right.tt;
	}

	bool cpps_value::operator>(const cpps_value &right) const
	{
		if (tt == right.tt)
		{
			switch (tt)
			{
			case CPPS_TNUMBER: return value.number > right.value.number;
			case CPPS_TINTEGER: return value.integer > right.value.integer;
			case CPPS_TUINTEGER: return value.uinteger > right.value.uinteger;
			case CPPS_TSTRING: {
				std::string* tmpStr = cpps_get_string(*this);
				std::string* tmpStr2 = cpps_get_string(right);
				return (*(tmpStr)) > (*(tmpStr2));
			}
			case CPPS_TFUNCTION:
			case CPPS_TLAMBDAFUNCTION:
			{
				return value.uinteger > right.value.uinteger;
			}
			case CPPS_TCLASSVAR: {
				object left = object(*this);
				cpps_cppsclassvar* cppsclassvar = cpps_to_cpps_cppsclassvar(*this);
				cpps_cppsclass* cppsclass = cppsclassvar->getcppsclass();
				C* c = cppsclass->getDefaultCState();
				cpps_function* func = cppsclass->getoperator(">");
				if (func) {
					object right_object = object(right);
					cpps_cppsclassvar* right_cppsclassvar = cpps_to_cpps_cppsclassvar(right);
					cpps_cppsclass* right_cppsclass = right_cppsclassvar->getcppsclass();
					if (right_cppsclass != cppsclass) return cppsclass > right_cppsclass;
					object symbolfunc = cpps_value(func);
					cpps::cpps_value ret;
					if (func->getIsNeedC()) {
						ret = doclassfunction(c, left, symbolfunc, cpps::object::create(c, c), right).getval();
					}
					else {
						ret = doclassfunction(c, left, symbolfunc, right).getval();
					}
					return cpps_to_bool(ret);
				}
			}
			default:
				throw(cpps_error("unknow file", 0, 0, ">   ... It can't be used as a key. type: %d", right.tt));
			}
		}
		return tt > right.tt;
	}

	bool cpps_value::operator==(const cpps_value &right) const
	{
		if (tt == right.tt)
		{
			switch (tt)
			{
			case CPPS_TNUMBER: return value.number == right.value.number;
			case CPPS_TINTEGER: return value.integer == right.value.integer;
			case CPPS_TUINTEGER: return value.uinteger == right.value.uinteger;
			case CPPS_TSTRING: {
				std::string* tmpStr = cpps_get_string(*this);
				std::string* tmpStr2 = cpps_get_string(right);
				return (*(tmpStr)) == (*(tmpStr2));
			}
			case CPPS_TFUNCTION:
			case CPPS_TLAMBDAFUNCTION:
			{
				return value.uinteger == right.value.uinteger;
			}
			case CPPS_TCLASSVAR: {
				object left = object(*this);
				cpps_cppsclassvar* cppsclassvar = cpps_to_cpps_cppsclassvar(*this);
				cpps_cppsclass* cppsclass = cppsclassvar->getcppsclass();
				C* c = cppsclass->getDefaultCState();
				cpps_function* func = cppsclass->getoperator("==");
				if (func) {
					object right_object = object(right);
					cpps_cppsclassvar* right_cppsclassvar = cpps_to_cpps_cppsclassvar(right);
					cpps_cppsclass* right_cppsclass = right_cppsclassvar->getcppsclass();
					if (right_cppsclass != cppsclass) return cppsclass == right_cppsclass;
					if (right_cppsclassvar == cppsclassvar) return true; //同一个指针
					object symbolfunc = cpps_value(func);
					cpps::cpps_value ret;
					if (func->getIsNeedC()) {
						ret = doclassfunction(c, left, symbolfunc, cpps::object::create(c, c), right).getval();
					}
					else {
						ret = doclassfunction(c, left, symbolfunc, right).getval();
					}
					return cpps_to_bool(ret);
				}
			}
			default:
				throw(cpps_error("unknow file", 0, 0, "==   ... It can't be used as a key. type: %d", right.tt));
			}
		}
		return false;
	}
	void cpps_value::_initstring(C* c, const char* s) {
		std::string* str = NULL;
		newclass<std::string>(c, &str, this);
		str->append(s);
	}
	void cpps_value::_initstring(C* c, const std::string& s) {
		std::string* str = NULL;
		newclass<std::string>(c, &str, this);
		str->append(s);
	}
	cpps_value::cpps_value(C* c, const char* s)
	{
		_initstring(c, s);
	}

	cpps_value::cpps_value(C* c, const std::string& s)
	{
		_initstring(c, s);
	}

	cpps_value::cpps_value(C* c, const std::string&& s)
	{
		_initstring(c, s);
	}

	cpps_value::cpps_value(cpps_value* v)
	{
		tt = CPPS_TREF;
		value.value = v;
	}

	cpps_value::cpps_value(cpps_cppsclassvar* d)
	{
		tt = CPPS_TCLASSVAR;
		value.domain = (cpps_domain*)(d);
		incruse();
	}

	cpps_value::cpps_value(cpps_cppsclass* d)
	{
		tt = CPPS_TCLASS;
		value.domain = (cpps_domain*)(d);
	}

	cpps_value::cpps_value(cpps_domain* d)
	{
		tt = CPPS_TDOMAIN;
		value.domain = d;
	}

	cpps_value::cpps_value(cpps_function* f)
	{
		tt = CPPS_TFUNCTION;
		value.func = f;
	}

	cpps_value::cpps_value(cpps_classvar_quato* f)
	{
		tt = CPPS_TQUOTECLASSVAR;
		value.quato = f;
	}

	cpps_value::cpps_value(const bool b)
	{
		tt = CPPS_TBOOLEAN;
		value.b = b;
	}

	cpps_value::cpps_value(const cpps_integer i)
	{
		tt = CPPS_TINTEGER;
		value.integer = i;
	}

	cpps_value::cpps_value(const cpps_number n)
	{
		tt = CPPS_TNUMBER;
		value.number = n;
	}

	cpps_value::cpps_value()
	{
		tt = CPPS_TNIL;
		value.integer = 0;
	}

	cpps_value::cpps_value(const cpps_value& v)
	{
		tt = v.tt;
		value = v.value;
		incruse();
	}
#ifdef _WIN32

	cpps_value::cpps_value(const unsigned __int64 i)
	{
		tt = CPPS_TUINTEGER;
		value.uinteger = i;
	}
#else
	cpps_value::cpps_value(const long unsigned int i)
	{
		tt = CPPS_TUINTEGER;
		value.uinteger = i;
	}
#endif
	cpps_value::cpps_value(char& strv)
	{
		tt = CPPS_TSTRINGV;
		value.strv = &strv;
	}
	cpps_value::~cpps_value()
	{
		decruse();
		tt = CPPS_TNIL;
		value.integer = 0;
	}

	bool cpps_value::operator<=(const cpps_value right) const
	{
		if (tt == right.tt)
		{
			switch (tt)
			{
			case CPPS_TNUMBER: return value.number <= right.value.number;
			case CPPS_TINTEGER: return value.integer <= right.value.integer;
			case CPPS_TUINTEGER: return value.uinteger <= right.value.uinteger;
			case CPPS_TSTRING: {
				cpps_cppsclassvar* cppsclassvar = (cpps_cppsclassvar*)value.domain;
				std::string* tmpStr = (std::string*)cppsclassvar->getclsptr();

				cpps_cppsclassvar* cppsclassvar2 = (cpps_cppsclassvar*)right.value.domain;
				std::string* tmpStr2 = (std::string*)cppsclassvar2->getclsptr();

				return (*(tmpStr)) <= (*(tmpStr2));
			}
			case CPPS_TFUNCTION:
			case CPPS_TLAMBDAFUNCTION:
			{
				return value.uinteger <= right.value.uinteger;
			}
			case CPPS_TCLASSVAR: {
				object left = object(*this);
				cpps_cppsclassvar* cppsclassvar = cpps_to_cpps_cppsclassvar(*this);
				cpps_cppsclass* cppsclass = cppsclassvar->getcppsclass();
				C* c = cppsclass->getDefaultCState();
				cpps_function* func = cppsclass->getoperator("<=");
				if (func) {
					object right_object = object(right);
					cpps_cppsclassvar* right_cppsclassvar = cpps_to_cpps_cppsclassvar(right);
					cpps_cppsclass* right_cppsclass = right_cppsclassvar->getcppsclass();
					if (right_cppsclass != cppsclass) return cppsclass <= right_cppsclass;
					object symbolfunc = cpps_value(func);
					cpps::cpps_value ret;
					if (func->getIsNeedC()) {
						ret = doclassfunction(c, left, symbolfunc, cpps::object::create(c, c), right).getval();
					}
					else {
						ret = doclassfunction(c, left, symbolfunc, right).getval();
					}
					return cpps_to_bool(ret);
				}
			}
			default:
				throw(cpps_error("unknow file", 0, 0, "<=   ... It can't be used as a key.type: %d", right.tt));
			}
		}
		return tt <= right.tt;
	}

	cpps::cpps_value& cpps_value::operator=(const cpps_value& v)
	{
		// 避免自赋值
		assert(this != &v);
		v.incruse();
		decruse();
		tt = v.tt;
		value = v.value;

		return *this;
	}


	bool cpps_value::operator>=(const cpps_value right) const
	{
		if (tt == right.tt)
		{
			switch (tt)
			{
			case CPPS_TNUMBER: return value.number >= right.value.number;
			case CPPS_TINTEGER: return value.integer >= right.value.integer;
			case CPPS_TUINTEGER: return value.uinteger >= right.value.uinteger;
			case CPPS_TSTRING: {
				cpps_cppsclassvar* cppsclassvar = (cpps_cppsclassvar*)value.domain;
				std::string* tmpStr = (std::string*)cppsclassvar->getclsptr();

				cpps_cppsclassvar* cppsclassvar2 = (cpps_cppsclassvar*)right.value.domain;
				std::string* tmpStr2 = (std::string*)cppsclassvar2->getclsptr();

				return (*(tmpStr)) >= (*(tmpStr2));
			}
			case CPPS_TFUNCTION:
			case CPPS_TLAMBDAFUNCTION:
			{
				return value.uinteger >= right.value.uinteger;
			}
			case CPPS_TCLASSVAR: {
				object left = object(*this);
				cpps_cppsclassvar* cppsclassvar = cpps_to_cpps_cppsclassvar(*this);
				cpps_cppsclass* cppsclass = cppsclassvar->getcppsclass();
				C* c = cppsclass->getDefaultCState();
				cpps_function* func = cppsclass->getoperator(">=");
				if (func) {
					object right_object = object(right);
					cpps_cppsclassvar* right_cppsclassvar = cpps_to_cpps_cppsclassvar(right);
					cpps_cppsclass* right_cppsclass = right_cppsclassvar->getcppsclass();
					if (right_cppsclass != cppsclass) return cppsclass >= right_cppsclass;
					object symbolfunc = cpps_value(func);
					cpps::cpps_value ret;
					if (func->getIsNeedC()) {
						ret = doclassfunction(c, left, symbolfunc, cpps::object::create(c, c), right).getval();
					}
					else {
						ret = doclassfunction(c, left, symbolfunc, right).getval();
					}
					return cpps_to_bool(ret);
				}
			}
			default:
				throw(cpps_error("unknow file", 0, 0, ">=   ... It can't be used as a key.type: %d", right.tt));
			}
		}
		return tt >= right.tt;
	}

	void cpps_value::decruse()
	{
		if (tt == CPPS_TINTEGER) return;
		if (tt == CPPS_TUINTEGER) return;
		if (tt == CPPS_TNIL) return;
		if (tt == CPPS_TCLASSVAR || tt == CPPS_TSTRING || tt == CPPS_TLAMBDAFUNCTION || tt == CPPS_TTUPLE)
			value.domain->decruse();
	}
	void cpps_value::decruse() const
	{
		if (tt == CPPS_TINTEGER) return;
		if (tt == CPPS_TUINTEGER) return;
		if (tt == CPPS_TNIL) return;
		if (tt == CPPS_TCLASSVAR || tt == CPPS_TSTRING || tt == CPPS_TLAMBDAFUNCTION || tt == CPPS_TTUPLE)
			value.domain->decruse();
	}

	 void cpps_value::incruse()
	{
		 if (tt == CPPS_TINTEGER) return;
		 if (tt == CPPS_TUINTEGER) return;
		 if (tt == CPPS_TNIL) return;
		 if (tt == CPPS_TCLASSVAR || tt == CPPS_TSTRING || tt == CPPS_TLAMBDAFUNCTION || tt == CPPS_TTUPLE)
			value.domain->incruse();
	 }
	 void cpps_value::incruse()const
	 {
		 if (tt == CPPS_TINTEGER) return;
		 if (tt == CPPS_TUINTEGER) return;
		 if (tt == CPPS_TNIL) return;
		 if (tt == CPPS_TCLASSVAR || tt == CPPS_TSTRING || tt == CPPS_TLAMBDAFUNCTION || tt == CPPS_TTUPLE)
			 value.domain->incruse();
	 }

	bool cpps_value::isdomain() const
	{
		if (tt == CPPS_TINTEGER) return false;
		if (tt == CPPS_TUINTEGER) return false;
		if (tt == CPPS_TNIL) return false;
		return tt == CPPS_TDOMAIN || tt == CPPS_TCLASS || tt == CPPS_TCLASSVAR || tt == CPPS_TSTRING || tt == CPPS_TTUPLE;
	}

	bool cpps_value::isref() const
	{
		return tt == CPPS_TREF;
	}
	cpps_value	cpps_value::ref()
	{
		return cpps_value(this);
	}


	const cpps::cpps_value& cpps_value::real() const
	{
		return isref() ? *value.value : *this;
	}
	cpps::cpps_value& cpps_value::real()
	{
		return isref() ? *value.value : *this;
	}

	size_t cpps_value::hash::operator()(const cpps_value& _Keyval) const
	{

		switch (_Keyval.tt)
		{
		case CPPS_TNUMBER: return std::hash<cpps_number>()(_Keyval.value.number);
		case CPPS_TINTEGER: return std::hash<cpps_integer>()(_Keyval.value.integer);
		case CPPS_TUINTEGER: return std::hash<cpps_uinteger>()(_Keyval.value.uinteger);
		case CPPS_TSTRING: {
			std::string* tmpStr = cpps_get_string(_Keyval);
			return std::hash<std::string>()(*(tmpStr));
		}
		case CPPS_TFUNCTION:
		case CPPS_TLAMBDAFUNCTION:
		{
			return std::hash<cpps_integer>()(_Keyval.value.integer);
		}
		case CPPS_TCLASSVAR: {
			object left = object(_Keyval);
			cpps_cppsclassvar* cppsclassvar = cpps_to_cpps_cppsclassvar(_Keyval);
			cpps_cppsclass* cppsclass = cppsclassvar->getcppsclass();
			C* c = cppsclass->getDefaultCState();
			object hash_value = left["hash_value"];
			if (hash_value.isfunction()) {
				return (size_t)doclassfunction(c, left, hash_value).toint();
			}
		}
		default:
			throw(cpps_error("unknow file", 0, 0, "==   ... It can't be used as a HashKey. type: %d", _Keyval.tt));
		}
		return _Keyval.tt;
	}

}
