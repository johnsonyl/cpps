#ifndef CPPS_BASE_CPPS_HEAD_
#define CPPS_BASE_CPPS_HEAD_

//===================================
//@Author		:	Johnson
//@QQ			:	88481106
//@Email		:	88481106@qq.com
//@Date			:	2015/12/8 (yy/mm/dd)
//@Module		:	CPPS_BASE
//@Description	:	»ù´¡º¯Êý¿â
//@website		:	http://cppscript.org
//==================================


namespace cpps
{
	void	cpps_regbase(C *c);

	//public function
	void cpps_base_printf(C*c,object b);
	void cpps_base_printfln(C*c,object b);
	bool cpps_base_issetable(cpps_value v);
	bool cpps_base_ismap(cpps_value v);
	bool cpps_base_isstring(cpps_value v);
	bool cpps_base_isvector(cpps_value v);
	bool cpps_base_isrange(cpps_value v);
	bool cpps_base_isint(cpps_value v);
	bool cpps_base_isnumber(cpps_value v);
	bool cpps_base_isnull(cpps_value v);
	bool cpps_base_isfunction(cpps_value v);
	bool cpps_base_isclassvar(cpps_value v);
	bool cpps_base_isclass(cpps_value v);
	bool cpps_base_isellipsis(cpps_value v);
	bool cpps_base_istuple(cpps_value v);
}

#endif // CPPS_BASE_CPPS_HEAD_