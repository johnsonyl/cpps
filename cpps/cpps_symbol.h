#ifndef CPPS_SYMBOL_CPPS_HEAD_
#define CPPS_SYMBOL_CPPS_HEAD_

//===================================
//@Author		:	Shaji
//@Email		:	441781535@qq.com
//@Date			:	2015/11/26 (yy/mm/dd)
//@Module		:	CPPS_SYMBOL
//@Description	:	操作符
//@website		:	http://cpps.wiki
//==================================

#define MAXSYMBOLPRIO 9 // 不得超过16

#define NOLIMIT		((byte)0x00)
#define LEFTMUSTVAR ((byte)0x80)
#define RIGHTMUSTVAR ((byte)0x40)

namespace cpps
{

	struct cpps_symbol;
	typedef	std::map<std::string, cpps_symbol*> CPPS_SYMBOL_MAP;

	struct cpps_symbol
	{
		//操作符限制,第一位表示左操作数限制，第二位表示右操作符限制
		//第三位第四位表示参数个数，最后四位表示操作优先级
		byte		symbollimit;		
		//操作符函数
		std::string symbolfuncname;		//操作函数名

		byte getprio(){ return symbollimit & 0x0F; }
		byte getparamnum(){ return (symbollimit >> 4) & 0x03; }
		byte getparamleftlimit(){ return symbollimit & 0x80; }
		byte getparamrightlimit(){ return symbollimit & 0x40; }

		cpps_symbol(byte prio, byte paramnum,byte paramlimit,std::string funcname)
		{
			symbollimit = (prio & 0x0F) | ((paramnum & 0x03) << 4) | paramlimit;
			symbolfuncname = funcname;
		}
	};
	bool			cpps_parse_isleftasso(byte prio);
	cpps_symbol*	cpps_parse_getsymbol(std::string symbolstr, bool leftsymbol);
	void			cpps_regsymbols(C * c);

}
#endif // CPPS_CAST_CPPS_HEAD_