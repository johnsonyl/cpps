#ifndef CPPS_API_CPPS_HEAD_
#define CPPS_API_CPPS_HEAD_

//===================================
//@Author		:	Johnson
//@QQ			:	88481106
//@Email		:	88481106@qq.com
//@Date			:	2020/12/15 (yy/mm/dd)
//@Module		:	CPPS_API
//@Description	:	»ù´¡º¯Êý¿â
//@website		:	http://cppscript.org
//==================================


namespace cpps
{
	bool cpps_ispair(const cpps_value& v);
	bool cpps_isset(const cpps_value& v);
	bool cpps_ismap(const cpps_value& v);
	bool cpps_isbasevar(const cpps_value& v);
	bool cpps_isstring(const cpps_value& v);
	bool cpps_isvector(const cpps_value& v);
	bool cpps_isrange(const cpps_value& v);
	bool cpps_isint(const cpps_value& v);
	bool cpps_isbool(const cpps_value& v);
	bool cpps_isnumber(const cpps_value& v);
	bool cpps_isnull(const cpps_value& v);
	bool cpps_isfunction(const cpps_value& v);
	bool cpps_isclassvar(const cpps_value& v);
	bool cpps_isclass(const cpps_value& v);
	bool cpps_isellipsis(const cpps_value& v);
	bool cpps_istuple(const cpps_value& v);
}

#endif // CPPS_BASE_CPPS_HEAD_