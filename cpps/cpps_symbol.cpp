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
			throw(cpps_error("0",0,0,"不能对布尔值做这种运算啊！"));
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

	cpps_value cpps_subtract(cpps_value a, cpps_value b)
	{
		if (a.tt == CPPS_TBOOLEAN || b.tt == CPPS_TBOOLEAN)
		{
			throw(cpps_error("0", 0, 0, "不能对布尔值做这种运算啊！"));
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
			throw(cpps_error("0", 0, 0, "不能对布尔值做这种运算啊！"));
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
			throw(cpps_error("0", 0, 0, "不能对布尔值做这种运算啊！"));
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
			throw(cpps_error("0", 0, 0, "不能对布尔值做这种运算啊！"));
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

	cpps_value cpps_strcat(cpps_value a, cpps_value b)
	{
		cpps_value _result;
		_result.tt = CPPS_TSTRING;
		_result.str = cpps_to_string(a) + cpps_to_string(b);

		return _result;
	}

	cpps_value cpps_strcatassignment(cpps_value a, cpps_value b)
	{
		if (a.tt == CPPS_TNIL)
		{
			return cpps_value();
		}

		a.value.value->tt = CPPS_TSTRING;

		if (b.tt == CPPS_TREGVAR)
			a.value.value->str = cpps_to_string(*(a.value.value)) + cpps_to_string(*(b.value.value));
		else
			a.value.value->str = cpps_to_string(*(a.value.value)) + cpps_to_string(b);

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
			throw(cpps_error("0", 0, 0, "不能对非数字进行自增操作！"));
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
			throw(cpps_error("0", 0, 0, "不能对非数字进行自增操作！"));
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
			throw(cpps_error("0", 0, 0, "不能对非数字进行自减操作！"));
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
			throw(cpps_error("0", 0, 0, "不能对非数字进行自减操作！"));
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
			throw(cpps_error("0", 0, 0, "不能对数字和变量以外的类型取正！"));
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
			throw(cpps_error("0", 0, 0, "不能对数字和变量以外的类型取负！"));
			break;
		}
		return _result;
	}

	cpps_value cpps_assignment(cpps_value a,cpps_value b)
	{
		if (a.tt == CPPS_TNIL)
		{
			return cpps_value();
		}

		if (b.tt == CPPS_TREGVAR)
			*(a.value.value) = *(b.value.value);
		else
			*(a.value.value) = b;

		return *(a.value.value);
// 		a.value.var->setValue(b);
// 		return a.value.var->getValue();
	}

	cpps_value cpps_addandassignment(cpps_value a, cpps_value b)
	{
		if (a.tt == CPPS_TNIL)
		{
			throw(cpps_error("0", 0, 0, "变量未定义！"));
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
			throw(cpps_error("0", 0, 0, "变量未定义！"));
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
			throw(cpps_error("0", 0, 0, "变量未定义！"));
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
				throw(cpps_error("0", 0, 0, "不能将非数字和数字进行比较！"));
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
				throw(cpps_error("0", 0, 0, "不能将非数字和数字进行比较！"));
			}
			break;
		case CPPS_TSTRING:
			if (b.tt == CPPS_TSTRING)
			{
				ret.value.b = a.str > b.str;
			}
			else
			{
				throw(cpps_error("0", 0, 0, "不能将非字符串和字符串进行比较！"));
			}
			break;
		default:
			throw(cpps_error("0", 0, 0, "这俩不能比较！"));
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
				throw(cpps_error("0", 0, 0, "不能将非数字和数字进行比较！"));
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
				throw(cpps_error("0", 0, 0, "不能将非数字和数字进行比较！"));
			}
			break;
		case CPPS_TSTRING:
			if (b.tt == CPPS_TSTRING)
			{
				ret.value.b = a.str < b.str;
			}
			else
			{
				throw(cpps_error("0", 0, 0, "不能将非字符串和字符串进行比较！"));
			}
			break;
		default:
			throw(cpps_error("0", 0, 0, "这俩不能比较！"));
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
				throw(cpps_error("0", 0, 0, "不能将非数字和数字进行比较！"));
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
				throw(cpps_error("0", 0, 0, "不能将非数字和数字进行比较！"));
			}
			break;
		case CPPS_TSTRING:
			if (b.tt == CPPS_TSTRING)
			{
				ret.value.b = a.str >= b.str;
			}
			else
			{
				throw(cpps_error("0", 0, 0, "不能将非字符串和字符串进行比较！"));
			}
			break;
		default:
			throw(cpps_error("0", 0, 0, "这俩不能比较！"));
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
				throw(cpps_error("0", 0, 0, "不能将非数字和数字进行比较！"));
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
				throw(cpps_error("0", 0, 0, "不能将非数字和数字进行比较！"));
			}
			break;
		case CPPS_TSTRING:
			if (b.tt == CPPS_TSTRING)
			{
				ret.value.b = a.str <= b.str;
			}
			else
			{
				throw(cpps_error("0", 0, 0, "不能将非字符串和字符串进行比较！"));
			}
			break;
		default:
			throw(cpps_error("0", 0, 0, "这俩不能比较！"));
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
		
			break;
		case CPPS_TSTRING:
			if (b.tt == CPPS_TSTRING)
			{
				ret.value.b = a.str == b.str;
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
		cpps_value ret;
		ret.tt = CPPS_TBOOLEAN;
		ret.value.b = true;
		switch (a.tt)
		{
		case CPPS_TNIL:
			ret.value.b = b.tt != CPPS_TNIL;
			break;
		case CPPS_TNUMBER:
			if (b.tt == CPPS_TNUMBER)
			{
				ret.value.b = a.value.number != b.value.number;
			}
			else if (b.tt == CPPS_TINTEGER)
			{
				ret.value.b = a.value.number != b.value.integer;
			}
			
			break;
		case CPPS_TINTEGER:
			if (b.tt == CPPS_TNUMBER)
			{
				ret.value.b = a.value.integer != b.value.number;
			}
			else if (b.tt == CPPS_TINTEGER)
			{
				ret.value.b = a.value.integer != b.value.integer;
			}
			
			break;
		case CPPS_TSTRING:
			if (b.tt == CPPS_TSTRING)
			{
				ret.value.b = a.str != b.str;
			}
			
			break;
		case CPPS_TBOOLEAN:
			if (b.tt == CPPS_TBOOLEAN)
			{
				if (a.value.b == 0 && b.value.b != 0)
				{
					ret.value.b = 1;
				}
				else if (a.value.b != 0 && b.value.b == 0)
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

		bool b0 = converter::apply(a);
		bool b1 = converter::apply(b);
		ret.value.b = b0 && b1;

		return ret;

	}
	cpps_value cpps_or(cpps_value a, cpps_value b)
	{
		cpps_value ret;
		ret.tt = CPPS_TBOOLEAN;


		typedef cpps_converter<bool> converter;

		bool b0 = converter::apply(a);
		bool b1 = converter::apply(b);
		ret.value.b = b0 || b1;

		return ret;
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
			def("-", cpps_subtract),
			def("..", cpps_strcat),
			def(">", cpps_bigger),
			def(">=", cpps_biggerorequel),
			def("<", cpps_less),
			def("<=", cpps_lessorequel),
			def("==", cpps_equel),
			def("!=", cpps_notequel),
			def("=", cpps_assignment),
			def("+=", cpps_addandassignment),
			def("-=", cpps_subtractandassignment),
			def("*=", cpps_mulandassignment),
			def("/=", cpps_divideandassignment),
			def("&&", cpps_and),
			def("||", cpps_or),
			def("..=", cpps_strcatassignment)
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
		symbolmap["="]		= new cpps_symbol(8, 2, LEFTMUSTVAR,	"=");
		symbolmap["+="]		= new cpps_symbol(8, 2, LEFTMUSTVAR,	"+=");
		symbolmap["-="]		= new cpps_symbol(8, 2, LEFTMUSTVAR,	"-=");
		symbolmap["*="]		= new cpps_symbol(8, 2, LEFTMUSTVAR,	"*=");
		symbolmap["/="]		= new cpps_symbol(8, 2, LEFTMUSTVAR,	"/=");
		symbolmap["..="]	= new cpps_symbol(8, 2, LEFTMUSTVAR,	"..=");

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