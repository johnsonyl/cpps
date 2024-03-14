#ifndef CPPS_SYMBOL_CPPS_HEAD_
#define CPPS_SYMBOL_CPPS_HEAD_

//===================================
//@Author		:	Shaji
//@Email		:	441781535@qq.com
//@Date			:	2015/11/26 (yy/mm/dd)
//@Module		:	CPPS_SYMBOL
//@Description	:	操作符
//@website		:	http://cppscript.org
//==================================

namespace cpps
{
	enum cpps_symbol_type
	{
		CPPS_SYMBOL_TYPE_LEFTAUTOINCREASE,
		CPPS_SYMBOL_TYPE_LEFTAUTODECREASE,
		CPPS_SYMBOL_TYPE_PLUS,
		CPPS_SYMBOL_TYPE_MINUS,
		CPPS_SYMBOL_TYPE_NOT,
		CPPS_SYMBOL_TYPE_NOT2,
		CPPS_SYMBOL_TYPE_RIGHTAUTOINCREASE,
		CPPS_SYMBOL_TYPE_RIGHTAUTODECREASE,
		CPPS_SYMBOL_TYPE_MUL,
		CPPS_SYMBOL_TYPE_DIVIDE,
		CPPS_SYMBOL_TYPE_QUYU,
		CPPS_SYMBOL_TYPE_ADD,
		CPPS_SYMBOL_TYPE_ADD2,
		CPPS_SYMBOL_TYPE_ADD3,
		CPPS_SYMBOL_TYPE_ADD4,
		CPPS_SYMBOL_TYPE_ADD5,
		CPPS_SYMBOL_TYPE_ADD6,
		CPPS_SYMBOL_TYPE_SUBTRACT,
		CPPS_SYMBOL_TYPE_STRCAT,
		CPPS_SYMBOL_TYPE_BIGGER,
		CPPS_SYMBOL_TYPE_BIGGEROREQUEL,
		CPPS_SYMBOL_TYPE_LESS,
		CPPS_SYMBOL_TYPE_LESSOREQUEL,
		CPPS_SYMBOL_TYPE_EQUEL,
		CPPS_SYMBOL_TYPE_NOTEQUEL,
		CPPS_SYMBOL_TYPE_ASSIGNMENT,
		CPPS_SYMBOL_TYPE_ADDANDASSIGNMENT,
		CPPS_SYMBOL_TYPE_ADD2ANDASSIGNMENT,
		CPPS_SYMBOL_TYPE_ADD3ANDASSIGNMENT,
		CPPS_SYMBOL_TYPE_ADD4ANDASSIGNMENT,
		CPPS_SYMBOL_TYPE_ADD5ANDASSIGNMENT,
		CPPS_SYMBOL_TYPE_ADD6ANDASSIGNMENT,
		CPPS_SYMBOL_TYPE_SUBTRACTANDASSIGNMENT,
		CPPS_SYMBOL_TYPE_MULANDASSIGNMENT,
		CPPS_SYMBOL_TYPE_DIVIDEANDASSIGNMENT,
		CPPS_SYMBOL_TYPE_AND,
		CPPS_SYMBOL_TYPE_OR,
		CPPS_SYMBOL_TYPE_STRCATASSIGNMENT,
		CPPS_SYMBOL_TYPE_TERNARYOPERATOR,
		CPPS_SYMBOL_TYPE_GETOBJECT,
		CPPS_SYMBOL_TYPE_GETSUBOBJECT,
		CPPS_SYMBOL_TYPE_POW,
		CPPS_SYMBOL_TYPE_POWANDASSIGNMENT,
		CPPS_SYMBOL_TYPE_FLOORDIVISION,
		CPPS_SYMBOL_TYPE_FLOORDIVISIONANDASSIGNMENT,
		CPPS_SYMBOL_TYPE_FUNC,
		CPPS_SYMBOL_TYPE_END

	};

	struct cpps_symbol
	{
		//操作符限制,第一位表示左操作数限制，第二位表示右操作符限制
		//第三位第四位表示参数个数，最后四位表示操作优先级
		usint8		symbollimit;		
		//操作符函数
		std::string symbolfuncname;		//操作函数名
		int8	symboltype;

		usint8 getprio();
		usint8 getparamnum();
		usint8 getparamleftlimit();
		usint8 getparamrightlimit();
		int8   getsymboltype();
		//cpps_symbol(byte prio, byte paramnum,byte paramlimit,std::string funcname)
		//{
		//	symbollimit = (prio & 0x0F) | ((paramnum & 0x03) << 4) | paramlimit;
		//	symbolfuncname = funcname;
		//}
		cpps_symbol(usint8 prio, usint8 paramnum, usint8 paramlimit, std::string name, usint8 type);
	};
	bool			cpps_parse_isleftasso(C*c,usint8 prio);
	cpps_symbol*	cpps_parse_getsymbol(C*c,std::string& symbolstr, bool leftsymbol);
	void			cpps_regsymbols(C * c);

	void cpps_add(cpps_value& a, cpps_value& b, cpps_value& _result);
	void cpps_add2(cpps_value& a, cpps_value& b, cpps_value& _result);
	void cpps_add3(cpps_value& a, cpps_value& b, cpps_value& _result);
	void cpps_add4(cpps_value& a, cpps_value& b, cpps_value& _result);
	void cpps_add5(cpps_value& a, cpps_value& b, cpps_value& _result);
	void cpps_add6(cpps_value& a, cpps_value& b, cpps_value& _result);
	void cpps_subtract(cpps_value& a, cpps_value& b, cpps_value& _result);
	void cpps_mul(cpps_value& a, cpps_value& b, cpps_value& _result);
	void cpps_quyu(cpps_value& a, cpps_value& b, cpps_value& _result);
	void cpps_divide(cpps_value& a, cpps_value& b, cpps_value& _result);
	void cpps_strcat(C* c, cpps_value& a, cpps_value& b, cpps_value& ret);
	void cpps_strcatassignment(C* c, cpps_value& a, cpps_value& b, cpps_value& ret);
	void cpps_rightautoincrease(cpps_value& a, cpps_value& ret);
	void cpps_leftautoincrease(cpps_value& a, cpps_value& ret);
	void cpps_rightautodecrease(cpps_value& a, cpps_value& ret);
	void cpps_leftautodecrease(cpps_value& a, cpps_value& ret);
	void cpps_plus(cpps_value& a, cpps_value& _result);
	void cpps_minus(cpps_value& a, cpps_value& _result);
	void cpps_assignment(C* c, cpps_value& a, cpps_value& b, cpps::cpps_value& ret);
	void cpps_addandassignment(cpps_value& a, cpps_value& b, cpps::cpps_value& _result);
	void cpps_add3andassignment(cpps_value& a, cpps_value& b, cpps::cpps_value& _result);
	void cpps_add4andassignment(cpps_value& a, cpps_value& b, cpps::cpps_value& _result);
	void cpps_add2andassignment(cpps_value& a, cpps_value& b, cpps::cpps_value& _result);
	void cpps_add5andassignment(cpps_value& a, cpps_value& b, cpps::cpps_value& _result);
	void cpps_add6andassignment(cpps_value& a, cpps_value& b, cpps::cpps_value& _result);
	void cpps_symbol_pow(cpps_value& a, cpps_value& b, cpps_value& _result);
	void cpps_symbol_powandassignment(cpps_value& a, cpps_value& b, cpps::cpps_value& _result);
	void cpps_symbol_floordivision(cpps_value& a, cpps_value& b, cpps_value& _result);
	void cpps_symbol_floordivisionandassignment(cpps_value& a, cpps_value& b, cpps::cpps_value& _result);
	void cpps_subtractandassignment(cpps_value& a, cpps_value& b, cpps::cpps_value& _result);
	void cpps_mulandassignment(cpps_value& a, cpps_value& b, cpps::cpps_value& _result);
	void cpps_divideandassignment(cpps_value& a, cpps_value& b, cpps::cpps_value& _result);
	void cpps_bigger(cpps_value& a, cpps_value& b, cpps_value& ret);
	void cpps_less(cpps_value& a, cpps_value& b, cpps_value& ret);
	void cpps_biggerorequel(cpps_value& a, cpps_value& b, cpps_value& ret);
	void cpps_lessorequel(cpps_value& a, cpps_value& b, cpps_value& ret);
	void cpps_equel(cpps_value& a, cpps_value& b, cpps_value& ret);
	void cpps_notequel(cpps_value& a, cpps_value& b, cpps_value& ret);
	void cpps_not(cpps_value a, cpps_value& ret);
	void cpps_not2(cpps_value a, cpps_value& ret);


}
#endif // CPPS_CAST_CPPS_HEAD_
