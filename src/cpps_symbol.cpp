#include "cpps/cpps.h"

namespace cpps
{

	cpps_value cpps_calculate_expression(C* c, cpps_domain* domain, cpps_domain* root, node* o, cpps_domain*& leftdomain);
	CPPS_SYMBOL_MAP		symbolmap;						//操作符表
	CPPS_SYMBOL_MAP		leftsymbolmap;					//左操作符表
	bool				symbolleftasso[MAXSYMBOLPRIO];	//操作符是否左结合


	void cpps_add(cpps_value &a, cpps_value b, cpps_value &_result)
	{
		if (a.tt == CPPS_TBOOLEAN || b.tt == CPPS_TBOOLEAN)
		{
			throw(cpps_error("0",0,0,"This operation cannot be performed on Boolean values."));
		}
		;
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
	}
	void cpps_add2(cpps_value &a, cpps_value &b, cpps_value &_result)
	{
		if (a.tt == CPPS_TBOOLEAN || b.tt == CPPS_TBOOLEAN)
		{
			throw(cpps_error("0", 0, 0, "This operation cannot be performed on Boolean values."));
		}
		;
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
	}

	void cpps_add3(cpps_value &a, cpps_value b, cpps_value &_result)
	{
		if (a.tt == CPPS_TBOOLEAN || b.tt == CPPS_TBOOLEAN)
		{
			throw(cpps_error("0", 0, 0, "This operation cannot be performed on Boolean values."));
		}
		
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
	}

	void cpps_subtract(cpps_value& a, cpps_value &b, cpps_value &_result)
	{
		if (a.tt == CPPS_TBOOLEAN || b.tt == CPPS_TBOOLEAN)
		{
			throw(cpps_error("0", 0, 0, "This operation cannot be performed on Boolean values."));
		}
		
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
	}
	
	void cpps_mul(cpps_value& a, cpps_value b, cpps_value &_result)
	{
		if (a.tt == CPPS_TBOOLEAN || b.tt == CPPS_TBOOLEAN)
		{
			throw(cpps_error("0", 0, 0, "This operation cannot be performed on Boolean values."));
		}
		
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
	}
	
	void cpps_quyu(cpps_value &a, cpps_value &b, cpps_value &_result)
	{
		if (a.tt == CPPS_TBOOLEAN || b.tt == CPPS_TBOOLEAN)
		{
			throw(cpps_error("0", 0, 0, "This operation cannot be performed on Boolean values."));
		}
		
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
	}
	void cpps_divide(cpps_value &a, cpps_value b, cpps_value &_result)
	{
		if (a.tt == CPPS_TBOOLEAN || b.tt == CPPS_TBOOLEAN)
		{
			throw(cpps_error("0", 0, 0, "This operation cannot be performed on Boolean values."));
		}
		
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
	}

	void cpps_strcat(C*c,cpps_value &a, cpps_value &b, cpps_value &ret)
	{
		std::string *str;
		ret = newclass<std::string>(c, &str);
		str->append(a.tt == CPPS_TSTRING ? *cpps_get_string(a) : cpps_to_string(a));
		str->append(b.tt == CPPS_TSTRING ? *cpps_get_string(b) : cpps_to_string(b));

	}

	void cpps_strcatassignment(C*c,cpps_value &a, cpps_value &b,cpps_value &ret)
	{
		if (a.tt == CPPS_TNIL)
		{
			return;
		}
		if (a.tt == CPPS_TREGVAR) {

			if (a.value.value->tt == CPPS_TSTRING) {
				cpps_cppsclassvar* cppsclassvar = (cpps_cppsclassvar*)a.value.value->value.domain;
				std::string* tmpStr = (std::string*)cppsclassvar->getclsptr();
				if (b.tt == CPPS_TREGVAR)
				{
					tmpStr->append(*cpps_get_string(*(b.value.value)));
				}
				else
				{
					tmpStr->append(*cpps_get_string(b));
				}

			}
			else {
				std::string* tmpStr = NULL;
				*(a.value.value) = newclass<std::string>(c, &tmpStr);
				if (b.tt == CPPS_TREGVAR)
				{
					tmpStr->append(*cpps_get_string(*(b.value.value)));
				}
				else
				{
					tmpStr->append(*cpps_get_string(b));
				}
			}


			ret = *(a.value.value);
		}
	}
	
	void cpps_rightautoincrease(cpps_value& a, cpps_value &ret)
	{
		if (a.tt == CPPS_TREGVAR) {
			cpps_value& v = *(a.value.value);
			ret = v;
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
		}
		else if (a.tt == CPPS_TSTRINGV)
		{
			char& strv = *(a.value.strv);
			ret = (cpps_integer)strv;
			strv++;
		}
		
	}

	void cpps_leftautoincrease(cpps_value& a,cpps_value &ret)
	{
		if (a.tt == CPPS_TREGVAR) {

			cpps_value& v = *(a.value.value);
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
			ret = v;
		}
		else if (a.tt == CPPS_TSTRINGV)
		{
			char& strv = *(a.value.strv);
			strv++;
			ret = (cpps_integer)strv;
		}
	}

	void cpps_rightautodecrease(cpps_value& a, cpps_value& ret)
	{
		if (a.tt == CPPS_TREGVAR) {

			cpps_value& v = *(a.value.value);
			ret = v;
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
		}
		else if (a.tt == CPPS_TSTRINGV)
		{
			char& strv = *(a.value.strv);
			ret = (cpps_integer)strv;
			strv--;
		}
	}

	void cpps_leftautodecrease(cpps_value& a, cpps_value& ret)
	{
		if (a.tt == CPPS_TREGVAR) {

			cpps_value& v = *(a.value.value);
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
			ret = v;
		}
		else if (a.tt == CPPS_TSTRINGV)
		{
			char& strv = *(a.value.strv);
			strv--;
			ret = (cpps_integer)strv;
		}
	}

	//正号
	void cpps_plus(cpps_value& a, cpps_value &_result)
	{
		
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
	}

	void cpps_minus(cpps_value& a, cpps_value &_result)
	{
		
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
	}

	void cpps_assignment(C*c,cpps_value& a,cpps_value b,cpps::cpps_value &ret)
	{
		if (a.tt == CPPS_TNIL)
		{
			return ;
		}

		if (a.tt == CPPS_TREGVAR) {

			if (b.tt == CPPS_TREGVAR)
			{
				if (b.value.value->tt == CPPS_TSTRING)
				{
					if (a.value.value->tt == CPPS_TSTRING)
					{
						std::string* src = cpps_get_string(*(b.value.value));
						std::string* tar = cpps_get_string(*(a.value.value));
						*tar = *src;
					}
					else
					{
						std::string* str;
						cpps_value ret = newclass<std::string>(c, &str);


						cpps_cppsclassvar* cppsclassvar = (cpps_cppsclassvar*)b.value.value->value.domain;
						std::string* tmpStr = (std::string*)cppsclassvar->getclsptr();

						str->append(tmpStr->c_str());

						*(a.value.value) = ret;
					}

				}
				else
					*(a.value.value) = *(b.value.value);


			}
			else {

				if (b.tt == CPPS_TSTRING)
				{
					if (a.value.value->tt == CPPS_TSTRING)
					{
						std::string* src = cpps_get_string(b);
						std::string* tar = cpps_get_string(*a.value.value);
						*tar = *src;
						//tar->assign(*src);
					}
					else
					{
						std::string* str;
						cpps_value ret = newclass<std::string>(c, &str);

						cpps_cppsclassvar* cppsclassvar = (cpps_cppsclassvar*)b.value.domain;
						std::string* tmpStr = (std::string*)cppsclassvar->getclsptr();

						str->append(tmpStr->begin(), tmpStr->end());
						*(a.value.value) = ret;
					}
				}
				else
					*(a.value.value) = b;

			}

			ret = *(a.value.value);
		}
		else if (a.tt == CPPS_TSTRINGV)
		{
			char& strv = *(a.value.strv);
			strv = (char)cpps_to_integer(b);
			ret = (cpps_integer)strv;
		}
	}

	void cpps_addandassignment(cpps_value& a, cpps_value b, cpps::cpps_value& _result)
	{
		if (a.tt == CPPS_TNIL)
		{
			throw(cpps_error("0", 0, 0, "The variable is nil."));
		}

		if (a.tt == CPPS_TREGVAR) {

			cpps_value& v = *(a.value.value);
			v.decruse();

			if (v.tt == CPPS_TBOOLEAN || b.tt == CPPS_TBOOLEAN)
			{
				throw(cpps_error("0", 0, 0, "This operation cannot be performed on Boolean values."));
			}
			;
			_result.tt = CPPS_TNIL;
			switch (v.tt)
			{
			case CPPS_TINTEGER:
				if (b.tt == CPPS_TINTEGER)
				{
					v.value.integer += cpps_to_integer(b);
				}
				else if (b.tt == CPPS_TNUMBER)
				{
					v.tt = CPPS_TNUMBER;
					v.value.number = cpps_to_number(v) + cpps_to_number(b);
				}
				else if (b.tt == CPPS_TSTRING)
				{
					v.tt = CPPS_TNUMBER;
					v.value.number = cpps_to_number(v) + cpps_to_number(b);
				}
				break;
			case CPPS_TNUMBER:
			case CPPS_TSTRING:
				v.tt = CPPS_TNUMBER;
				v.value.number = cpps_to_number(v) + cpps_to_number(b);
				break;
			default:
				break;
			}
			_result = v;
		}
		else if (a.tt == CPPS_TSTRINGV)
		{
			char& strv = *(a.value.strv);
			strv += (char)cpps_to_integer(b);
			_result = (cpps_integer)strv;
		}

	}

	void cpps_subtractandassignment(cpps_value& a, cpps_value b, cpps::cpps_value& _result)
	{
		if (a.tt == CPPS_TNIL)
		{
			throw(cpps_error("0", 0, 0, "The variable is nil."));
		}

		if (a.tt == CPPS_TREGVAR) {

			cpps_value& v = *(a.value.value);
			v.decruse();

			if (v.tt == CPPS_TBOOLEAN || b.tt == CPPS_TBOOLEAN)
			{
				throw(cpps_error("0", 0, 0, "This operation cannot be performed on Boolean values."));
			}
			;
			_result.tt = CPPS_TNIL;
			switch (v.tt)
			{
			case CPPS_TINTEGER:
				if (b.tt == CPPS_TINTEGER)
				{
					v.value.integer -= cpps_to_integer(b);
				}
				else if (b.tt == CPPS_TNUMBER)
				{
					v.tt = CPPS_TNUMBER;
					v.value.number = cpps_to_number(v) - cpps_to_number(b);
				}
				else if (b.tt == CPPS_TSTRING)
				{
					v.tt = CPPS_TNUMBER;
					v.value.number = cpps_to_number(v) - cpps_to_number(b);
				}
				break;
			case CPPS_TNUMBER:
			case CPPS_TSTRING:
				v.tt = CPPS_TNUMBER;
				v.value.number = cpps_to_number(v) - cpps_to_number(b);
				break;
			default:
				break;
			}
			_result = v;
		}
		else if (a.tt == CPPS_TSTRINGV)
		{
			char& strv = *(a.value.strv);
			strv -= (char)cpps_to_integer(b);
			_result = (cpps_integer)strv;
		}

	}

	void cpps_mulandassignment(cpps_value& a, cpps_value b, cpps::cpps_value& _result)
	{
		if (a.tt == CPPS_TNIL)
		{
			throw(cpps_error("0", 0, 0, "The variable is nil."));
		}

		if (a.tt == CPPS_TREGVAR) {

			cpps_value& v = *(a.value.value);
			v.decruse();

			if (v.tt == CPPS_TBOOLEAN || b.tt == CPPS_TBOOLEAN)
			{
				throw(cpps_error("0", 0, 0, "This operation cannot be performed on Boolean values."));
			}

			_result.tt = CPPS_TNIL;
			switch (v.tt)
			{
			case CPPS_TINTEGER:
				if (b.tt == CPPS_TINTEGER)
				{
					v.value.integer *= cpps_to_integer(b);
				}
				else if (b.tt == CPPS_TNUMBER)
				{
					v.tt = CPPS_TNUMBER;
					v.value.number = cpps_to_number(v) * cpps_to_number(b);
				}
				else if (b.tt == CPPS_TSTRING)
				{
					v.tt = CPPS_TNUMBER;
					v.value.number = cpps_to_number(v) * cpps_to_number(b);
				}
				break;
			case CPPS_TNUMBER:
			case CPPS_TSTRING:
				v.tt = CPPS_TNUMBER;
				v.value.number = cpps_to_number(v) * cpps_to_number(b);
				break;
			default:
				break;
			}
			_result = v;
		}
		else if (a.tt == CPPS_TSTRINGV)
		{
			char& strv = *(a.value.strv);
			strv *= (char)cpps_to_integer(b);
			_result = (cpps_integer)strv;
		}
	}

	void cpps_divideandassignment(cpps_value& a, cpps_value b, cpps::cpps_value& _result)
	{
		if (a.tt == CPPS_TNIL)
		{
			throw(cpps_error("0", 0, 0, "The variable is nil."));
		}

		if (a.tt == CPPS_TREGVAR) {

			cpps_value& v = *(a.value.value);
			v.decruse();

			if (v.tt == CPPS_TBOOLEAN || b.tt == CPPS_TBOOLEAN)
			{
				throw(cpps_error("0", 0, 0, "This operation cannot be performed on Boolean values."));
			}
			;
			_result.tt = CPPS_TNIL;
			switch (v.tt)
			{
			case CPPS_TINTEGER:
				if (b.tt == CPPS_TINTEGER)
				{
					v.value.integer /= cpps_to_integer(b);
				}
				else if (b.tt == CPPS_TNUMBER)
				{
					v.tt = CPPS_TNUMBER;
					v.value.number = cpps_to_number(v) / cpps_to_number(b);
				}
				else if (b.tt == CPPS_TSTRING)
				{
					v.tt = CPPS_TNUMBER;
					v.value.number = cpps_to_number(v) / cpps_to_number(b);
				}
				break;
			case CPPS_TNUMBER:
			case CPPS_TSTRING:
				v.tt = CPPS_TNUMBER;
				v.value.number = cpps_to_number(v) / cpps_to_number(b);
				break;
			default:
				break;
			}
			_result = v;
		}
		else if (a.tt == CPPS_TSTRINGV)
		{
			char& strv = *(a.value.strv);
			strv /= (char)cpps_to_integer(b);
			_result = (cpps_integer)strv;
		}
	}

	void cpps_bigger(cpps_value& a, cpps_value b, cpps_value &ret)
	{
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
	}

	void cpps_less(cpps_value& a, cpps_value b, cpps_value &ret)
	{
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
	}

	void cpps_biggerorequel(cpps_value& a, cpps_value b, cpps_value &ret)
	{
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
	}

	void cpps_lessorequel(cpps_value& a, cpps_value b, cpps_value &ret)
	{
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
	}

	void cpps_equel(cpps_value& a, cpps_value b, cpps_value &ret)
	{
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
	}

	void cpps_notequel(cpps_value& a, cpps_value b, cpps_value &ret)
	{
		cpps_equel(a, b, ret);
		ret.value.b = !ret.value.b;
	}

	void cpps_not(cpps_value a, cpps_value &ret)
	{
		ret.tt = CPPS_TBOOLEAN;
		typedef cpps_converter<bool> converter;
		bool b = converter::apply(a);
		ret.value.b = !b;
	}
	/*cpps_value cpps_and(cpps_value a, cpps_value b)
	{
		cpps_value ret;
		ret.tt = CPPS_TBOOLEAN;


		typedef cpps_converter<bool> converter;

		ret.value.b = converter::apply(a) && converter::apply(b);

		return ret;

	}*/
	/*cpps_value cpps_or(cpps_value a, cpps_value b)
	{
		cpps_value ret;
		ret.tt = CPPS_TBOOLEAN;


		typedef cpps_converter<bool> converter;

		ret.value.b = converter::apply(a) || converter::apply(b);

		return ret;
	}*/
	/*cpps_value cpps_ternaryoperator(cpps_value a, cpps_value b,cpps_value c)
	{
		typedef cpps_converter<bool> converter;

		bool b0 = converter::apply(a);

		return b0 ? b : c;
	}*/
	void cpps_regsymbols(C * c)
	{
		
		
		leftsymbolmap["++"] = new cpps_symbol(0, 1, RIGHTMUSTVAR,	CPPS_SYMBOL_TYPE_LEFTAUTOINCREASE);
		leftsymbolmap["--"] = new cpps_symbol(0, 1, RIGHTMUSTVAR,	CPPS_SYMBOL_TYPE_LEFTAUTODECREASE);
		leftsymbolmap["+"]	= new cpps_symbol(0, 1, NOLIMIT,		CPPS_SYMBOL_TYPE_PLUS);
		leftsymbolmap["-"]	= new cpps_symbol(0, 1, NOLIMIT,		CPPS_SYMBOL_TYPE_MINUS);
		leftsymbolmap["!"]	= new cpps_symbol(0, 1, NOLIMIT,		CPPS_SYMBOL_TYPE_NOT);
		symbolmap["++"]		= new cpps_symbol(0, 1, LEFTMUSTVAR,	CPPS_SYMBOL_TYPE_RIGHTAUTOINCREASE);
		symbolmap["--"]		= new cpps_symbol(0, 1, LEFTMUSTVAR,	CPPS_SYMBOL_TYPE_RIGHTAUTODECREASE);
		symbolmap["*"]		= new cpps_symbol(1, 2, NOLIMIT,		CPPS_SYMBOL_TYPE_MUL);
		symbolmap["/"]		= new cpps_symbol(1, 2, NOLIMIT,		CPPS_SYMBOL_TYPE_DIVIDE);
		symbolmap["%"]		= new cpps_symbol(1, 2, NOLIMIT,		CPPS_SYMBOL_TYPE_QUYU);
		symbolmap["+"]		= new cpps_symbol(2, 2, NOLIMIT,		CPPS_SYMBOL_TYPE_ADD);
		symbolmap["&"]		= new cpps_symbol(2, 2, NOLIMIT,		CPPS_SYMBOL_TYPE_ADD2);
		symbolmap["|"]		= new cpps_symbol(2, 2, NOLIMIT,		CPPS_SYMBOL_TYPE_ADD3);
		symbolmap["-"]		= new cpps_symbol(2, 2, NOLIMIT,		CPPS_SYMBOL_TYPE_SUBTRACT);
		symbolmap[".."]		= new cpps_symbol(3, 2, NOLIMIT,		CPPS_SYMBOL_TYPE_STRCAT);
		symbolmap[">"]		= new cpps_symbol(4, 2, NOLIMIT,		CPPS_SYMBOL_TYPE_BIGGER);
		symbolmap[">="]		= new cpps_symbol(4, 2, NOLIMIT,		CPPS_SYMBOL_TYPE_BIGGEROREQUEL);
		symbolmap["<"]		= new cpps_symbol(4, 2, NOLIMIT,		CPPS_SYMBOL_TYPE_LESS);
		symbolmap["<="]		= new cpps_symbol(4, 2, NOLIMIT,		CPPS_SYMBOL_TYPE_LESSOREQUEL);
		symbolmap["=="]		= new cpps_symbol(5, 2, NOLIMIT,		CPPS_SYMBOL_TYPE_EQUEL);
		symbolmap["!="]		= new cpps_symbol(5, 2, NOLIMIT,		CPPS_SYMBOL_TYPE_NOTEQUEL);
		symbolmap["&&"]		= new cpps_symbol(6, 2, NOLIMIT,		CPPS_SYMBOL_TYPE_AND);
		symbolmap["||"]		= new cpps_symbol(7, 2, NOLIMIT,		CPPS_SYMBOL_TYPE_OR);
		symbolmap["?"]		= new cpps_symbol(8, 3, NOLIMIT,		CPPS_SYMBOL_TYPE_TERNARYOPERATOR);
		symbolmap["="]		= new cpps_symbol(9, 2, LEFTMUSTVAR,	CPPS_SYMBOL_TYPE_ASSIGNMENT);
		symbolmap["+="]		= new cpps_symbol(9, 2, LEFTMUSTVAR,	CPPS_SYMBOL_TYPE_ADDANDASSIGNMENT);
		symbolmap["-="]		= new cpps_symbol(9, 2, LEFTMUSTVAR,	CPPS_SYMBOL_TYPE_SUBTRACTANDASSIGNMENT);
		symbolmap["*="]		= new cpps_symbol(9, 2, LEFTMUSTVAR,	CPPS_SYMBOL_TYPE_MULANDASSIGNMENT);
		symbolmap["/="]		= new cpps_symbol(9, 2, LEFTMUSTVAR,	CPPS_SYMBOL_TYPE_DIVIDEANDASSIGNMENT);
		symbolmap["..="]	= new cpps_symbol(9, 2, LEFTMUSTVAR,	CPPS_SYMBOL_TYPE_STRCATASSIGNMENT);

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

	bool cpps_parse_isleftasso(usint8 prio)
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

	

	void cpps_symbol_handle(C* c, cpps_domain* domain, cpps_domain* root, node* d, cpps_value& ret)
	{
		cpps_domain* leftdomain = NULL;
		cpps_value a = cpps_calculate_expression(c, domain,root, d->l[0], leftdomain);
		leftdomain = NULL;

		switch (d->symbol->symboltype)
		{
		case CPPS_SYMBOL_TYPE_LEFTAUTOINCREASE:
		{
			cpps_leftautoincrease(a, ret);
			break;
		}
		case CPPS_SYMBOL_TYPE_LEFTAUTODECREASE:
		{
			cpps_leftautodecrease(a, ret);
			break;
		}
		case CPPS_SYMBOL_TYPE_PLUS:
		{
			cpps_plus(a, ret);
			break;
		}
		case CPPS_SYMBOL_TYPE_MINUS:
		{
			cpps_minus(a, ret);
			break;
		}
		case CPPS_SYMBOL_TYPE_NOT:
		{
			cpps_not(a, ret);
			break;
		}
		case CPPS_SYMBOL_TYPE_RIGHTAUTOINCREASE:
		{
			cpps_rightautoincrease(a, ret);
			break;
		}
		case CPPS_SYMBOL_TYPE_RIGHTAUTODECREASE:
		{
			cpps_rightautodecrease(a, ret);
			break;
		}
		case CPPS_SYMBOL_TYPE_MUL:
		{
			cpps_value b = cpps_calculate_expression(c, domain, root, d->l[1], leftdomain);
			cpps_mul(a, b, ret);
			break;
		}
		case CPPS_SYMBOL_TYPE_DIVIDE:
		{
			cpps_value b = cpps_calculate_expression(c, domain, root, d->l[1], leftdomain);
			cpps_divide(a, b, ret);
			break;
		}
		case CPPS_SYMBOL_TYPE_QUYU:
		{
			cpps_value b = cpps_calculate_expression(c, domain, root, d->l[1], leftdomain);
			cpps_quyu(a, b, ret);
			break;
		}
		case CPPS_SYMBOL_TYPE_ADD:
		{
			cpps_value b = cpps_calculate_expression(c, domain, root, d->l[1], leftdomain);
			cpps_add(a, b, ret);
			break;
		}
		case CPPS_SYMBOL_TYPE_ADD2:
		{
			cpps_value b = cpps_calculate_expression(c, domain, root, d->l[1], leftdomain);
			cpps_add2(a, b, ret);
			break;
		}
		case CPPS_SYMBOL_TYPE_ADD3:
		{
			cpps_value b = cpps_calculate_expression(c, domain, root, d->l[1], leftdomain);
			cpps_add3(a, b, ret);
			break;
		}
		case CPPS_SYMBOL_TYPE_SUBTRACT:
		{
			cpps_value b = cpps_calculate_expression(c, domain, root, d->l[1], leftdomain);
			cpps_subtract(a, b, ret);
			break;
		}
		case CPPS_SYMBOL_TYPE_STRCAT:
		{
			cpps_value b = cpps_calculate_expression(c, domain, root, d->l[1], leftdomain);
			cpps_strcat(c, a, b, ret);
			break;
		}
		case CPPS_SYMBOL_TYPE_BIGGER:
		{
			cpps_value b = cpps_calculate_expression(c, domain, root, d->l[1], leftdomain);
			cpps_bigger(a, b, ret);
			break;
		}
		case CPPS_SYMBOL_TYPE_BIGGEROREQUEL:
		{
			cpps_value b = cpps_calculate_expression(c, domain, root, d->l[1], leftdomain);
			cpps_biggerorequel(a, b, ret);
			break;
		}
		case CPPS_SYMBOL_TYPE_LESS:
		{
			cpps_value b = cpps_calculate_expression(c, domain, root, d->l[1], leftdomain);
			cpps_less(a, b, ret);
			break;
		}
		case CPPS_SYMBOL_TYPE_LESSOREQUEL:
		{
			cpps_value b = cpps_calculate_expression(c, domain, root, d->l[1], leftdomain);
			cpps_lessorequel(a, b, ret);
			break;
		}
		case CPPS_SYMBOL_TYPE_EQUEL:
		{
			cpps_value b = cpps_calculate_expression(c, domain, root, d->l[1], leftdomain);
			cpps_equel(a, b, ret);
			break;
		}
		case CPPS_SYMBOL_TYPE_NOTEQUEL:
		{
			cpps_value b = cpps_calculate_expression(c, domain, root, d->l[1], leftdomain);
			cpps_notequel(a, b, ret);
			break;
		}
		case CPPS_SYMBOL_TYPE_ASSIGNMENT:
		{
			cpps_value b = cpps_calculate_expression(c, domain, root, d->l[1], leftdomain);
			cpps_assignment(c, a, b, ret);
			break;
		}
		case CPPS_SYMBOL_TYPE_ADDANDASSIGNMENT:
		{
			cpps_value b = cpps_calculate_expression(c, domain, root, d->l[1], leftdomain);
			cpps_addandassignment( a, b, ret);
			break;
		}
		case CPPS_SYMBOL_TYPE_SUBTRACTANDASSIGNMENT:
		{
			cpps_value b = cpps_calculate_expression(c, domain, root, d->l[1], leftdomain);
			cpps_subtractandassignment(a, b, ret);
			break;
		}
		case CPPS_SYMBOL_TYPE_MULANDASSIGNMENT:
		{
			cpps_value b = cpps_calculate_expression(c, domain, root, d->l[1], leftdomain);
			cpps_mulandassignment(a, b, ret);
			break;
		}
		case CPPS_SYMBOL_TYPE_DIVIDEANDASSIGNMENT:
		{
			cpps_value b = cpps_calculate_expression(c, domain, root, d->l[1], leftdomain);
			cpps_divideandassignment(a, b, ret);
			break;
		}
		case CPPS_SYMBOL_TYPE_AND:
		{
			typedef cpps::cpps_converter<bool> convert;
			ret.tt = CPPS_TBOOLEAN;
			ret.value.b =  convert::apply(a) && convert::apply(cpps_calculate_expression(c, domain, root, d->l[1], leftdomain));
			break;
		}
		case CPPS_SYMBOL_TYPE_OR:
		{
			typedef cpps::cpps_converter<bool> convert;
			ret.tt = CPPS_TBOOLEAN;
			ret.value.b = convert::apply(a) || convert::apply(cpps_calculate_expression(c, domain, root, d->l[1], leftdomain));
			break;
		}
		case CPPS_SYMBOL_TYPE_STRCATASSIGNMENT:
		{
			cpps_value b = cpps_calculate_expression(c, domain, root, d->l[1], leftdomain);
			cpps_strcatassignment(c,a, b, ret);
			break;
		}
		case CPPS_SYMBOL_TYPE_TERNARYOPERATOR:
		{
			typedef cpps::cpps_converter<bool> convert;
			ret = convert::apply(a) ? cpps_calculate_expression(c, domain, root, d->l[1], leftdomain) : cpps_calculate_expression(c, domain, root, d->l[2], leftdomain);
			break;
		}
		default:
			break;
		}
	}

}