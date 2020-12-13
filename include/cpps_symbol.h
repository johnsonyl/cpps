#ifndef CPPS_SYMBOL_CPPS_HEAD_
#define CPPS_SYMBOL_CPPS_HEAD_

//===================================
//@Author		:	Shaji
//@Email		:	441781535@qq.com
//@Date			:	2015/11/26 (yy/mm/dd)
//@Module		:	CPPS_SYMBOL
//@Description	:	������
//@website		:	http://cpps.wiki
//==================================

#define MAXSYMBOLPRIO 9 // ���ó���16

#define NOLIMIT		((byte)0x00)
#define LEFTMUSTVAR ((byte)0x80)
#define RIGHTMUSTVAR ((byte)0x40)

namespace cpps
{
	enum cpps_symbol_type
	{
		CPPS_SYMBOL_TYPE_LEFTAUTOINCREASE,
		CPPS_SYMBOL_TYPE_LEFTAUTODECREASE,
		CPPS_SYMBOL_TYPE_PLUS,
		CPPS_SYMBOL_TYPE_MINUS,
		CPPS_SYMBOL_TYPE_NOT,
		CPPS_SYMBOL_TYPE_RIGHTAUTOINCREASE,
		CPPS_SYMBOL_TYPE_RIGHTAUTODECREASE,
		CPPS_SYMBOL_TYPE_MUL,
		CPPS_SYMBOL_TYPE_DIVIDE,
		CPPS_SYMBOL_TYPE_QUYU,
		CPPS_SYMBOL_TYPE_ADD,
		CPPS_SYMBOL_TYPE_ADD2,
		CPPS_SYMBOL_TYPE_ADD3,
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
		CPPS_SYMBOL_TYPE_SUBTRACTANDASSIGNMENT,
		CPPS_SYMBOL_TYPE_MULANDASSIGNMENT,
		CPPS_SYMBOL_TYPE_DIVIDEANDASSIGNMENT,
		CPPS_SYMBOL_TYPE_AND,
		CPPS_SYMBOL_TYPE_OR,
		CPPS_SYMBOL_TYPE_STRCATASSIGNMENT,
		CPPS_SYMBOL_TYPE_TERNARYOPERATOR,

	};
	struct cpps_symbol;
	typedef	std::map<std::string, cpps_symbol*> CPPS_SYMBOL_MAP;

	struct cpps_symbol
	{
		//����������,��һλ��ʾ����������ƣ��ڶ�λ��ʾ�Ҳ���������
		//����λ����λ��ʾ���������������λ��ʾ�������ȼ�
		byte		symbollimit;		
		//����������
		//std::string symbolfuncname;		//����������
		int8	symboltype;

		byte getprio(){ return symbollimit & 0x0F; }
		byte getparamnum(){ return (symbollimit >> 4) & 0x03; }
		byte getparamleftlimit(){ return symbollimit & 0x80; }
		byte getparamrightlimit(){ return symbollimit & 0x40; }

		//cpps_symbol(byte prio, byte paramnum,byte paramlimit,std::string funcname)
		//{
		//	symbollimit = (prio & 0x0F) | ((paramnum & 0x03) << 4) | paramlimit;
		//	symbolfuncname = funcname;
		//}
		cpps_symbol(byte prio, byte paramnum, byte paramlimit, byte type)
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
		}
	};
	bool			cpps_parse_isleftasso(byte prio);
	cpps_symbol*	cpps_parse_getsymbol(std::string symbolstr, bool leftsymbol);
	void			cpps_regsymbols(C * c);

}
#endif // CPPS_CAST_CPPS_HEAD_