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

		usint8 getprio(){ return symbollimit & 0x0F; }
		usint8 getparamnum(){ return (symbollimit >> 4) & 0x03; }
		usint8 getparamleftlimit(){ return symbollimit & 0x80; }
		usint8 getparamrightlimit(){ return symbollimit & 0x40; }
		int8   getsymboltype() { return symboltype; }
		//cpps_symbol(byte prio, byte paramnum,byte paramlimit,std::string funcname)
		//{
		//	symbollimit = (prio & 0x0F) | ((paramnum & 0x03) << 4) | paramlimit;
		//	symbolfuncname = funcname;
		//}
		cpps_symbol(usint8 prio, usint8 paramnum, usint8 paramlimit,std::string name, usint8 type)
		{
#if defined(__APPLE__) && defined(__MACH__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wimplicit-int-conversion"
#endif
			symbollimit = (prio & 0x0F) | ((paramnum & 0x03) << 0x4) | paramlimit;
#if defined(__APPLE__) && defined(__MACH__)
#pragma clang diagnostic pop
#endif
			symboltype = type;
			symbolfuncname = name;
		}
	};
	bool			cpps_parse_isleftasso(C*c,usint8 prio);
	cpps_symbol*	cpps_parse_getsymbol(C*c,std::string& symbolstr, bool leftsymbol);
	void			cpps_regsymbols(C * c);

}
#endif // CPPS_CAST_CPPS_HEAD_
