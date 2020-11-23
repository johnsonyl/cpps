#include "cpps.h"

namespace cpps
{

	
	CPPS_SYMBOL_MAP		symbolmap;						//操作符表
	CPPS_SYMBOL_MAP		leftsymbolmap;					//左操作符表
	bool				symbolleftasso[MAXSYMBOLPRIO];	//操作符是否左结合


	cpps_value cpps_add(cpps_value a, cpps_value b)
	{
		if (a.tt == CPPS_TBOOLEAN || b.tt == CPPS_TBOOLEAN)
		{
			throw(cpps_error("0",0,0,"This operation cannot be performed on Boolean values."));
		}
		cpps_value _result;
		_result.tt = CPPS_TNIL;
		switch (a.tt)
		{
		case CPPS_TINTEGER:
			if (b.tt == CPPS_TINTEGER)
			{
				_result.tt = CPPS_TINTEGER;
				_result.value.integer = cpps_to_integer(a) + cpps_to_integer(b);
			}
			else if (b.tt == CPPS_TNUMBER)
			{
				_result.tt = CPPS_TNUMBER;
				_result.value.number = cpps_to_number(a) + cpps_to_number(b);
			}
			else if (b.tt == CPPS_TSTRING)
			{
				_result.tt = CPPS_TNUMBER;
				_result.value.number = cpps_to_number(a) + cpps_to_number(b);
			}
			break;
		case CPPS_TNUMBER:
		case CPPS_TSTRING:
			_result.tt = CPPS_TNUMBER;
			_result.value.number = cpps_to_number(a) + cpps_to_number(b);
			break;
		default:
			_result.tt = CPPS_TNIL;
			break;
		}
		return _result;
	}
	cpps_value cpps_add2(cpps_value a, cpps_value b)
	{
		if (a.tt == CPPS_TBOOLEAN || b.tt == CPPS_TBOOLEAN)
		{
			throw(cpps_error("0", 0, 0, "This operation cannot be performed on Boolean values."));
		}
		cpps_value _result;
		_result.tt = CPPS_TNIL;
		switch (a.tt)
		{
		case CPPS_TINTEGER:
			if (b.tt == CPPS_TINTEGER)
			{
				_result.tt = CPPS_TINTEGER;
				_result.value.integer = cpps_to_integer(a) & cpps_to_integer(b);
			}
			else if (b.tt == CPPS_TNUMBER)
			{
				_result.tt = CPPS_TINTEGER;
				_result.value.integer = cpps_to_integer(a) & cpps_to_integer(b);
			}
			else if (b.tt == CPPS_TSTRING)
			{
				_result.tt = CPPS_TINTEGER;
				_result.value.integer = cpps_to_integer(a) & cpps_to_integer(b);
			}
			break;
		case CPPS_TNUMBER:
		case CPPS_TSTRING:
			_result.tt = CPPS_TINTEGER;
			_result.value.integer = cpps_to_integer(a) & cpps_to_integer(b);
			break;
		default:
			_result.tt = CPPS_TNIL;
			break;
		}
		return _result;
	}

	cpps_value cpps_add3(cpps_value a, cpps_value b)
	{
		if (a.tt == CPPS_TBOOLEAN || b.tt == CPPS_TBOOLEAN)
		{
			throw(cpps_error("0", 0, 0, "This operation cannot be performed on Boolean values."));
		}
		cpps_value _result;
		_result.tt = CPPS_TNIL;
		switch (a.tt)
		{
		case CPPS_TINTEGER:
			if (b.tt == CPPS_TINTEGER)
			{
				_result.tt = CPPS_TINTEGER;
				_result.value.integer = cpps_to_integer(a) | cpps_to_integer(b);
			}
			else if (b.tt == CPPS_TNUMBER)
			{
				_result.tt = CPPS_TINTEGER;
				_result.value.integer = cpps_to_integer(a) | cpps_to_integer(b);
			}
			else if (b.tt == CPPS_TSTRING)
			{
				_result.tt = CPPS_TINTEGER;
				_result.value.integer = cpps_to_integer(a) | cpps_to_integer(b);
			}
			break;
		case CPPS_TNUMBER:
		case CPPS_TSTRING:
			_result.tt = CPPS_TINTEGER;
			_result.value.integer = cpps_to_integer(a) | cpps_to_integer(b);
			break;
		default:
			_result.tt = CPPS_TNIL;
			break;
		}
		return _result;
	}

	cpps_value cpps_subtract(cpps_value a, cpps_value b)
	{
		if (a.tt == CPPS_TBOOLEAN || b.tt == CPPS_TBOOLEAN)
		{
			throw(cpps_error("0", 0, 0, "This operation cannot be performed on Boolean values."));
		}
		cpps_value _result;
		_result.tt = CPPS_TNIL;
		switch (a.tt)
		{
		case CPPS_TINTEGER:
			if (b.tt == CPPS_TINTEGER)
			{
				_result.tt = CPPS_TINTEGER;
				_result.value.integer = cpps_to_integer(a) - cpps_to_integer(b);
			}
			else if (b.tt == CPPS_TNUMBER)
			{
				_result.tt = CPPS_TNUMBER;
				_result.value.number = cpps_to_number(a) - cpps_to_number(b);
			}
			else if (b.tt == CPPS_TSTRING)
			{
				_result.tt = CPPS_TNUMBER;
				_result.value.number = cpps_to_number(a) - cpps_to_number(b);
			}
			break;
		case CPPS_TNUMBER:
		case CPPS_TSTRING:
			_result.tt = CPPS_TNUMBER;
			_result.value.number = cpps_to_number(a) - cpps_to_number(b);
			break;
		default:
			_result.tt = CPPS_TNIL;
			break;
		}
		return _result;
	}
	
	cpps_value cpps_mul(cpps_value a, cpps_value b)
	{
		if (a.tt == CPPS_TBOOLEAN || b.tt == CPPS_TBOOLEAN)
		{
			throw(cpps_error("0", 0, 0, "This operation cannot be performed on Boolean values."));
		}
		cpps_value _result;
		_result.tt = CPPS_TNIL;
		switch (a.tt)
		{
		case CPPS_TINTEGER:
			if (b.tt == CPPS_TINTEGER)
			{
				_result.tt = CPPS_TINTEGER;
				_result.value.integer = cpps_to_integer(a) * cpps_to_integer(b);
			}
			else if (b.tt == CPPS_TNUMBER)
			{
				_result.tt = CPPS_TNUMBER;
				_result.value.number = cpps_to_number(a) * cpps_to_number(b);
			}
			else if (b.tt == CPPS_TSTRING)
			{
				_result.tt = CPPS_TNUMBER;
				_result.value.number = cpps_to_number(a) * cpps_to_number(b);
			}
			break;
		case CPPS_TNUMBER:
		case CPPS_TSTRING:
			_result.tt = CPPS_TNUMBER;
			_result.value.number = cpps_to_number(a) * cpps_to_number(b);
			break;
		default:
			_result.tt = CPPS_TNIL;
			break;
		}
		return _result;
	}
	
	cpps_value cpps_quyu(cpps_value a, cpps_value b)
	{
		if (a.tt == CPPS_TBOOLEAN || b.tt == CPPS_TBOOLEAN)
		{
			throw(cpps_error("0", 0, 0, "This operation cannot be performed on Boolean values."));
		}
		cpps_value _result;
		_result.tt = CPPS_TNIL;
		switch (a.tt)
		{
		case CPPS_TINTEGER:
		case CPPS_TNUMBER:
		case CPPS_TSTRING:
			if (cpps_to_number(b) != 0)
			{
				_result.tt = CPPS_TINTEGER;
				_result.value.integer = cpps_to_integer(a) % cpps_to_integer(b);
			}
			break;
		default:
			_result.tt = CPPS_TNIL;
			break;
		}
		return _result;
	}
	cpps_value cpps_divide(cpps_value a, cpps_value b)
	{
		if (a.tt == CPPS_TBOOLEAN || b.tt == CPPS_TBOOLEAN)
		{
			throw(cpps_error("0", 0, 0, "This operation cannot be performed on Boolean values."));
		}
		cpps_value _result;
		_result.tt = CPPS_TNIL;
		switch (a.tt)
		{
		case CPPS_TNUMBER:
		case CPPS_TINTEGER:
		case CPPS_TSTRING:
			if (cpps_to_number(b) != 0)
			{
				_result.tt = CPPS_TNUMBER;
				_result.value.number = cpps_to_number(a) / cpps_to_number(b);
			}
			break;
		default:
			_result.tt = CPPS_TNIL;
			break;
		}
		return _result;
	}

	cpps_value cpps_strcat(C*c,cpps_value a, cpps_value b)
	{
		std::string *str;
		cpps_value ret = newClassPtr<std::string>(c, &str);
		ret.tt = CPPS_TSTRING;
		str->append(a.tt == CPPS_TSTRING ? *cpps_get_string(a) : cpps_to_string(a));
		str->append(b.tt == CPPS_TSTRING ? *cpps_get_string(b) : cpps_to_string(b));

		return ret;
	}

	cpps_value cpps_strcatassignment(cpps_value a, cpps_value b)
	{
		if (a.tt == CPPS_TNIL)
		{
			return cpps_value();
		}

		a.value.value->tt = CPPS_TSTRING;

		cpps_cppsclassvar *cppsclassvar = (cpps_cppsclassvar *)a.value.value->value.domain;
		std::string *tmpStr = (std::string *)cppsclassvar->getclsptr();
		if (b.tt == CPPS_TREGVAR)
		{
			tmpStr->append(*cpps_get_string(*(b.value.value)));
		}
		else
		{
			tmpStr->append(*cpps_get_string(b));
		}

		return *(a.value.value);

	
	}
	
	cpps_value cpps_rightautoincrease(cpps_value a)
	{
		cpps_value &v = *(a.value.value);
		cpps_value ret = v;
		switch (v.tt)
		{
		case CPPS_TNUMBER:
			v.value.number ++;
			break;
		case CPPS_TINTEGER:
			v.value.integer++;
			break;
		default:
			throw(cpps_error("0", 0, 0, "Cannot do ++ operation on non number."));
			break;
		}
		return ret;
	}

	cpps_value cpps_leftautoincrease(cpps_value a)
	{
		cpps_value &v = *(a.value.value);
		switch (v.tt)
		{
		case CPPS_TNUMBER:
			v.value.number++;
			break;
		case CPPS_TINTEGER:
			v.value.integer++;
			break;
		default:
			throw(cpps_error("0", 0, 0, "Cannot do ++ operation on non number."));
			break;
		}
		return v;
	}

	cpps_value cpps_rightautodecrease(cpps_value a)
	{
		cpps_value &v = *(a.value.value);
		cpps_value ret = v;
		switch (v.tt)
		{
		case CPPS_TNUMBER:
			v.value.number--;
			break;
		case CPPS_TINTEGER:
			v.value.integer--;
			break;
		default:
			throw(cpps_error("0", 0, 0, "Cannot do -- operation on non number."));
			break;
		}
		return ret;
	}

	cpps_value cpps_leftautodecrease(cpps_value a)
	{
		cpps_value &v = *(a.value.value);
		switch (v.tt)
		{
		case CPPS_TNUMBER:
			v.value.number--;
			break;
		case CPPS_TINTEGER:
			v.value.integer--;
			break;
		default:
			throw(cpps_error("0", 0, 0, "Cannot do -- operation on non number."));
			break;
		}
		return v;
	}

	//正号
	cpps_value cpps_plus(cpps_value a)
	{
		cpps_value _result;
		_result.tt = CPPS_TNIL;

		switch (a.tt)
		{
		case CPPS_TINTEGER:
			_result.tt = CPPS_TINTEGER;
			_result.value.integer = a.value.integer;
			break;
		case CPPS_TNUMBER:
		case CPPS_TSTRING:
			_result.tt = CPPS_TINTEGER;
			_result.value.number = cpps_to_number(a);
		default:
			throw(cpps_error("0", 0, 0, "Cannot do +a operation on non number."));
			break;
		}
		return _result;
	}

	cpps_value cpps_minus(cpps_value a)
	{
		cpps_value _result;
		_result.tt = CPPS_TNIL;

		switch (a.tt)
		{
		case CPPS_TINTEGER:
			_result.tt = CPPS_TINTEGER;
			_result.value.integer = -a.value.integer;
			break;
		case CPPS_TNUMBER:
		case CPPS_TSTRING:
			_result.tt = CPPS_TINTEGER;
			_result.value.number = -cpps_to_number(a);
		default:
			throw(cpps_error("0", 0, 0, "Cannot do -a operation on non number."));
			break;
		}
		return _result;
	}

	cpps_value cpps_assignment(C*c,cpps_value a,cpps_value b)
	{
		if (a.tt == CPPS_TNIL)
		{
			return cpps_value();
		}

		if (b.tt == CPPS_TREGVAR)
		{
			if (b.value.value->tt == CPPS_TSTRING)
			{

				std::string *str;
				cpps_value ret = newClassPtr<std::string>(c, &str);
				ret.tt = CPPS_TSTRING;


				cpps_cppsclassvar *cppsclassvar = (cpps_cppsclassvar *)b.value.value->value.domain;
				std::string *tmpStr = (std::string *)cppsclassvar->getclsptr();

				str->append(tmpStr->c_str());
				*(a.value.value) = ret;

			}
			else
				*(a.value.value) = *(b.value.value);

			(*(a.value.value)).parentLambdaVar = NULL;

		}
		else {

			if (b.tt == CPPS_TSTRING)
			{
				std::string *str;
				cpps_value ret = newClassPtr<std::string>(c, &str);
				ret.tt = CPPS_TSTRING;

				cpps_cppsclassvar *cppsclassvar = (cpps_cppsclassvar *)b.value.domain;
				std::string *tmpStr = (std::string *)cppsclassvar->getclsptr();

				str->append(tmpStr->begin(),tmpStr->end());
				*(a.value.value) = ret;
			}
			else
				*(a.value.value) = b;

			(*(a.value.value)).parentLambdaVar = NULL;
		}

		return *(a.value.value);
// 		a.value.var->setValue(b);
// 		return a.value.var->getValue();
	}

	cpps_value cpps_addandassignment(cpps_value a, cpps_value b)
	{
		if (a.tt == CPPS_TNIL)
		{
			throw(cpps_error("0", 0, 0, "The variable is nil."));
		}
		cpps_value & v = *(a.value.value);
		*(a.value.value) = cpps_add(v, b);
		return *(a.value.value);


// 		cpps_value & v = a.value.var->getValue();
// 		a.value.var->setValue(cpps_add(v, b));
// 		return a.value.var->getValue();
	}

	cpps_value cpps_subtractandassignment(cpps_value a, cpps_value b)
	{
		if (a.tt == CPPS_TNIL)
		{
			throw(cpps_error("0", 0, 0, "The variable is nil."));
		}

		cpps_value & v = *(a.value.value);
		*(a.value.value) = cpps_subtract(v, b);
		return *(a.value.value);

// 		cpps_value & v = a.value.var->getValue();
// 		a.value.var->setValue(cpps_subtract(v, b));
// 		return a.value.var->getValue();
	}

	cpps_value cpps_mulandassignment(cpps_value a, cpps_value b)
	{
		if (a.tt == CPPS_TNIL)
		{
			throw(cpps_error("0", 0, 0, "The variable is nil."));
		}

		cpps_value & v = *(a.value.value);
		*(a.value.value) = cpps_mul(v, b);
		return *(a.value.value);

// 		cpps_value & v = a.value.var->getValue();
// 		a.value.var->setValue(cpps_mul(v, b));
// 		return a.value.var->getValue();
	}

	cpps_value cpps_divideandassignment(cpps_value a, cpps_value b)
	{
		if (a.tt == CPPS_TNIL)
		{
			throw(cpps_error("0", 0, 0, "变量未定义！"));
		}

		cpps_value & v = *(a.value.value);
		*(a.value.value) = cpps_divide(v, b);
		return *(a.value.value);

// 		cpps_value & v = a.value.var->getValue();
// 		a.value.var->setValue(cpps_divide(v, b));
// 		return a.value.var->getValue();
	}

	cpps_value cpps_bigger(cpps_value a, cpps_value b)
	{
		cpps_value ret;
		ret.tt = CPPS_TBOOLEAN;
		switch (a.tt)
		{
		case CPPS_TNUMBER:
			if (b.tt == CPPS_TNUMBER)
			{
				ret.value.b = a.value.number > b.value.number;
			}
			else if (b.tt == CPPS_TINTEGER)
			{
				ret.value.b = a.value.number > b.value.integer;
			}
			else
			{
				throw(cpps_error("0", 0, 0, "Can't compare non number with number."));
			}
			break;
		case CPPS_TINTEGER:
			if (b.tt == CPPS_TNUMBER)
			{
				ret.value.b = a.value.integer > b.value.number;
			}
			else if (b.tt == CPPS_TINTEGER)
			{
				ret.value.b = a.value.integer > b.value.integer;
			}
			else
			{
				throw(cpps_error("0", 0, 0, "Can't compare non number with number."));
			}
			break;
		case CPPS_TSTRING:
			if (b.tt == CPPS_TSTRING)
			{
				cpps_cppsclassvar *cppsclassvar = (cpps_cppsclassvar *)a.value.domain;
				std::string *tmpStr = (std::string *)cppsclassvar->getclsptr();

				cpps_cppsclassvar *cppsclassvar2 = (cpps_cppsclassvar *)b.value.domain;
				std::string *tmpStr2 = (std::string *)cppsclassvar2->getclsptr();
				ret.value.b = *(tmpStr) > *(tmpStr2);
			}
			else
			{
				throw(cpps_error("0", 0, 0, "Cannot compare non string with string."));
			}
			break;
		default:
			throw(cpps_error("0", 0, 0, "These two can't be compared."));
			break;
		}
		return ret;
	}

	cpps_value cpps_less(cpps_value a, cpps_value b)
	{
		cpps_value ret;
		ret.tt = CPPS_TBOOLEAN;
		switch (a.tt)
		{
		case CPPS_TNUMBER:
			if (b.tt == CPPS_TNUMBER)
			{
				ret.value.b = a.value.number < b.value.number;
			}
			else if (b.tt == CPPS_TINTEGER)
			{
				ret.value.b = a.value.number < b.value.integer;
			}
			else
			{
				throw(cpps_error("0", 0, 0, "Can't compare non number with number."));
			}
			break;
		case CPPS_TINTEGER:
			if (b.tt == CPPS_TNUMBER)
			{
				ret.value.b = a.value.integer < b.value.number;
			}
			else if (b.tt == CPPS_TINTEGER)
			{
				ret.value.b = a.value.integer < b.value.integer;
			}
			else
			{
				throw(cpps_error("0", 0, 0, "Can't compare non number with number."));
			}
			break;
		case CPPS_TSTRING:
			if (b.tt == CPPS_TSTRING)
			{
				cpps_cppsclassvar *cppsclassvar = (cpps_cppsclassvar *)a.value.domain;
				std::string *tmpStr = (std::string *)cppsclassvar->getclsptr();

				cpps_cppsclassvar *cppsclassvar2 = (cpps_cppsclassvar *)b.value.domain;
				std::string *tmpStr2 = (std::string *)cppsclassvar2->getclsptr();
				ret.value.b = *(tmpStr) < *(tmpStr2);
			}
			else
			{
				throw(cpps_error("0", 0, 0, "Cannot compare non string with string."));
			}
			break;
		default:
			throw(cpps_error("0", 0, 0, "These two can't be compared."));
			break;
		}
		return ret;
	}

	cpps_value cpps_biggerorequel(cpps_value a, cpps_value b)
	{
		cpps_value ret;
		ret.tt = CPPS_TBOOLEAN;
		switch (a.tt)
		{
		case CPPS_TNUMBER:
			if (b.tt == CPPS_TNUMBER)
			{
				ret.value.b = a.value.number >= b.value.number;
			}
			else if (b.tt == CPPS_TINTEGER)
			{
				ret.value.b = a.value.number >= b.value.integer;
			}
			else
			{
				throw(cpps_error("0", 0, 0, "Can't compare non number with number."));
			}
			break;
		case CPPS_TINTEGER:
			if (b.tt == CPPS_TNUMBER)
			{
				ret.value.b = a.value.integer >= b.value.number;
			}
			else if (b.tt == CPPS_TINTEGER)
			{
				ret.value.b = a.value.integer >= b.value.integer;
			}
			else
			{
				throw(cpps_error("0", 0, 0, "Can't compare non number with number."));
			}
			break;
		case CPPS_TSTRING:
			if (b.tt == CPPS_TSTRING)
			{
				cpps_cppsclassvar *cppsclassvar = (cpps_cppsclassvar *)a.value.domain;
				std::string *tmpStr = (std::string *)cppsclassvar->getclsptr();

				cpps_cppsclassvar *cppsclassvar2 = (cpps_cppsclassvar *)b.value.domain;
				std::string *tmpStr2 = (std::string *)cppsclassvar2->getclsptr();
				ret.value.b = *(tmpStr) >= *(tmpStr2);
			}
			else
			{
				throw(cpps_error("0", 0, 0, "Cannot compare non string with string."));
			}
			break;
		default:
			throw(cpps_error("0", 0, 0, "These two can't be compared."));
			break;
		}
		return ret;
	}

	cpps_value cpps_lessorequel(cpps_value a, cpps_value b)
	{
		cpps_value ret;
		ret.tt = CPPS_TBOOLEAN;
		switch (a.tt)
		{
		case CPPS_TNUMBER:
			if (b.tt == CPPS_TNUMBER)
			{
				ret.value.b = a.value.number <= b.value.number;
			}
			else if (b.tt == CPPS_TINTEGER)
			{
				ret.value.b = a.value.number <= b.value.integer;
			}
			else
			{
				throw(cpps_error("0", 0, 0, "Can't compare non number with number."));
			}
			break;
		case CPPS_TINTEGER:
			if (b.tt == CPPS_TNUMBER)
			{
				ret.value.b = a.value.integer <= b.value.number;
			}
			else if (b.tt == CPPS_TINTEGER)
			{
				ret.value.b = a.value.integer <= b.value.integer;
			}
			else
			{
				throw(cpps_error("0", 0, 0, "Can't compare non number with number."));
			}
			break;
		case CPPS_TSTRING:
			if (b.tt == CPPS_TSTRING)
			{
				cpps_cppsclassvar *cppsclassvar = (cpps_cppsclassvar *)a.value.domain;
				std::string *tmpStr = (std::string *)cppsclassvar->getclsptr();

				cpps_cppsclassvar *cppsclassvar2 = (cpps_cppsclassvar *)b.value.domain;
				std::string *tmpStr2 = (std::string *)cppsclassvar2->getclsptr();
				ret.value.b = *(tmpStr) <= *(tmpStr2);
			}
			else
			{
				throw(cpps_error("0", 0, 0, "Cannot compare non string with string."));
			}
			break;
		default:
			throw(cpps_error("0", 0, 0, "These two can't be compared."));
			break;
		}
		return ret;
	}

	cpps_value cpps_equel(cpps_value a, cpps_value b)
	{
		cpps_value ret;
		ret.tt = CPPS_TBOOLEAN;
		ret.value.b = false;

		
		switch (a.tt)
		{
		case CPPS_TNIL:
			ret.value.b = b.tt == CPPS_TNIL;
			break;
		case CPPS_TNUMBER:
			if (b.tt == CPPS_TNUMBER)
			{
				ret.value.b = a.value.number == b.value.number;
			}
			else if (b.tt == CPPS_TINTEGER)
			{
				ret.value.b = a.value.number == b.value.integer;
			}
			else if (b.tt == CPPS_TSTRING)
			{
				cpps_number t = cpps_to_number(b);
				ret.value.b = a.value.number == t;
			}
			
			break;
		case CPPS_TINTEGER:
			if (b.tt == CPPS_TNUMBER)
			{
				ret.value.b = a.value.integer == b.value.number;
			}
			else if (b.tt == CPPS_TINTEGER)
			{
				ret.value.b = a.value.integer == b.value.integer;
			}
			else if (b.tt == CPPS_TSTRING)
			{
				cpps_integer t = cpps_to_integer(b);
				ret.value.b = a.value.integer == t;
			}
			break;
		case CPPS_TSTRING:
			if (b.tt == CPPS_TSTRING)
			{

				cpps_cppsclassvar *cppsclassvar = (cpps_cppsclassvar *)a.value.domain;
				std::string *tmpStr = (std::string *)cppsclassvar->getclsptr();

				cpps_cppsclassvar *cppsclassvar2 = (cpps_cppsclassvar *)b.value.domain;
				std::string *tmpStr2 = (std::string *)cppsclassvar2->getclsptr();
				ret.value.b = *(tmpStr) == *(tmpStr2);
			}
			else if (b.tt == CPPS_TINTEGER)
			{
				std::string *t = cpps_get_string(b);

				cpps_cppsclassvar *cppsclassvar = (cpps_cppsclassvar *)a.value.domain;
				std::string *tmpStr = (std::string *)cppsclassvar->getclsptr();
				ret.value.b = *(tmpStr) == *t;
			}
			else if (b.tt == CPPS_TNUMBER)
			{
				std::string *t = cpps_get_string(b);

				cpps_cppsclassvar *cppsclassvar = (cpps_cppsclassvar *)a.value.domain;
				std::string *tmpStr = (std::string *)cppsclassvar->getclsptr();
				ret.value.b = *(tmpStr) == *t;
			}
			break; 
		case CPPS_TBOOLEAN:
			if (b.tt == CPPS_TBOOLEAN)
			{
				if (a.value.b == 0 && b.value.b == 0)
				{
					ret.value.b = 1;
				}
				else if (a.value.b != 0 && b.value.b != 0)
				{
					ret.value.b = 1;
				}
				else
				{
					ret.value.b = 0;
				}
			}
			
			break;
		default:
			break;
		}
		return ret;
	}

	cpps_value cpps_notequel(cpps_value a, cpps_value b)
	{
		cpps_value ret = cpps_equel(a, b);
		ret.value.b = !ret.value.b;
		return ret;
	}

	cpps_value cpps_not(cpps_value a)
	{
		cpps_value ret;
		ret.tt = CPPS_TBOOLEAN;
		
		typedef cpps_converter<bool> converter;

		bool b = converter::apply(a);
		ret.value.b = !b;

		return ret;
	}
	cpps_value cpps_and(cpps_value a, cpps_value b)
	{
		cpps_value ret;
		ret.tt = CPPS_TBOOLEAN;


		typedef cpps_converter<bool> converter;

		ret.value.b = converter::apply(a) && converter::apply(b);

		return ret;

	}
	cpps_value cpps_or(cpps_value a, cpps_value b)
	{
		cpps_value ret;
		ret.tt = CPPS_TBOOLEAN;


		typedef cpps_converter<bool> converter;

		ret.value.b = converter::apply(a) || converter::apply(b);

		return ret;
	}
	cpps_value cpps_ternaryoperator(cpps_value a, cpps_value b,cpps_value c)
	{
		typedef cpps_converter<bool> converter;

		bool b0 = converter::apply(a);

		return b0 ? b : c;
	}
	void cpps_regsymbols(C * c)
	{
		module(c)[
			def("++a", cpps_leftautoincrease),
			def("--a", cpps_leftautodecrease),
			def("+a", cpps_plus),
			def("-a", cpps_minus),
			def("!a", cpps_not),
			def("++", cpps_rightautoincrease),
			def("--", cpps_rightautodecrease),
			def("*", cpps_mul),
			def("/", cpps_divide),
			def("%", cpps_quyu),
			def("+", cpps_add),
			def("&", cpps_add2),
			def("|", cpps_add3),
			def("-", cpps_subtract),
			def_inside("..", cpps_strcat),
			def(">", cpps_bigger),
			def(">=", cpps_biggerorequel),
			def("<", cpps_less),
			def("<=", cpps_lessorequel),
			def("==", cpps_equel),
			def("!=", cpps_notequel),
			def_inside("=", cpps_assignment),
			def("+=", cpps_addandassignment),
			def("-=", cpps_subtractandassignment),
			def("*=", cpps_mulandassignment),
			def("/=", cpps_divideandassignment),
			def("&&", cpps_and),
			def("||", cpps_or),
			def("..=", cpps_strcatassignment),
			def("?", cpps_ternaryoperator)
		];

		leftsymbolmap["++"] = new cpps_symbol(0, 1, RIGHTMUSTVAR,	"++a");
		leftsymbolmap["--"] = new cpps_symbol(0, 1, RIGHTMUSTVAR,	"--a");
		leftsymbolmap["+"]	= new cpps_symbol(0, 1, NOLIMIT,		"+a");
		leftsymbolmap["-"]	= new cpps_symbol(0, 1, NOLIMIT,		"-a");
		leftsymbolmap["!"]	= new cpps_symbol(0, 1, NOLIMIT,		"!a");
		symbolmap["++"]		= new cpps_symbol(0, 1, LEFTMUSTVAR,	"++");
		symbolmap["--"]		= new cpps_symbol(0, 1, LEFTMUSTVAR,	"--");
		symbolmap["*"]		= new cpps_symbol(1, 2, NOLIMIT,		"*");
		symbolmap["/"]		= new cpps_symbol(1, 2, NOLIMIT,		"/");
		symbolmap["%"]		= new cpps_symbol(1, 2, NOLIMIT,		"%");
		symbolmap["+"]		= new cpps_symbol(2, 2, NOLIMIT,		"+");
		symbolmap["&"]		= new cpps_symbol(2, 2, NOLIMIT,		"&");
		symbolmap["|"]		= new cpps_symbol(2, 2, NOLIMIT,		"|");
		symbolmap["-"]		= new cpps_symbol(2, 2, NOLIMIT,		"-");
		symbolmap[".."]		= new cpps_symbol(3, 2, NOLIMIT,		"..");
		symbolmap[">"]		= new cpps_symbol(4, 2, NOLIMIT,		">");
		symbolmap[">="]		= new cpps_symbol(4, 2, NOLIMIT,		">=");
		symbolmap["<"]		= new cpps_symbol(4, 2, NOLIMIT,		"<");
		symbolmap["<="]		= new cpps_symbol(4, 2, NOLIMIT,		"<=");
		symbolmap["=="]		= new cpps_symbol(5, 2, NOLIMIT,		"==");
		symbolmap["!="]		= new cpps_symbol(5, 2, NOLIMIT,		"!=");
		symbolmap["&&"]		= new cpps_symbol(6, 2, NOLIMIT,		"&&");
		symbolmap["||"]		= new cpps_symbol(7, 2, NOLIMIT,		"||");
		symbolmap["?"]		= new cpps_symbol(8, 3, NOLIMIT,		"?");
		symbolmap["="]		= new cpps_symbol(9, 2, LEFTMUSTVAR,	"=");
		symbolmap["+="]		= new cpps_symbol(9, 2, LEFTMUSTVAR,	"+=");
		symbolmap["-="]		= new cpps_symbol(9, 2, LEFTMUSTVAR,	"-=");
		symbolmap["*="]		= new cpps_symbol(9, 2, LEFTMUSTVAR,	"*=");
		symbolmap["/="]		= new cpps_symbol(9, 2, LEFTMUSTVAR,	"/=");
		symbolmap["..="]	= new cpps_symbol(9, 2, LEFTMUSTVAR,	"..=");

		symbolleftasso[0] = false;
		symbolleftasso[1] = true;
		symbolleftasso[2] = true;
		symbolleftasso[3] = true;
		symbolleftasso[4] = true;
		symbolleftasso[5] = true;
		symbolleftasso[6] = true;
		symbolleftasso[7] = true;
		symbolleftasso[8] = false;
	}

	bool cpps_parse_isleftasso(byte prio)
	{
		//assert(getprio() < MAXSYMBOLPRIO);
		return symbolleftasso[prio];
	}

	cpps_symbol* cpps_parse_getsymbol(std::string symbolstr, bool leftsymbol)
	{
		cpps_symbol* ret = NULL;

		CPPS_SYMBOL_MAP::iterator it;
		if (leftsymbol)
		{
			it = leftsymbolmap.find(symbolstr);
			if (it != leftsymbolmap.end())
			{
				ret = it->second;
			}
		}
		else
		{
			it = symbolmap.find(symbolstr);
			if (it != symbolmap.end())
			{
				ret = it->second;
			}
		}

		return ret;
	}

}