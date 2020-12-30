#ifndef CPPS_STRING_CPPS_HEAD_
#define CPPS_STRING_CPPS_HEAD_

//===================================
//@Author		:	Johnson
//@QQ			:	88481106
//@Email		:	88481106@qq.com
//@Date			:	2015/11/25 (yy/mm/dd)
//@Module		:	CPPS_STRING
//@Description	:	创建String接口
//@website		:	http://cpps.wiki
//==================================


namespace cpps
{
	struct string : public std::string
	{
		cpps_integer	cpps_size();
		cpps_integer	cpps_string_find(std::string v2,object off);
		cpps_integer	cpps_string_rfind(std::string v2,object off);
		cpps::string*	cpps_string_replace(std::string v2, std::string v3);
		void			cpps_string_clear();
		void			cpps_string_copyto(cpps::string *tar,object off,object len);
		cpps_value		cpps_string_split(C* c, std::string v2, object len);
		cpps_value		cpps_string_cut(C* c, cpps_integer len);
		std::string		cpps_string_strcut( std::string v2,std::string v3);
		cpps_value		cpps_string_strcuts(C*c, std::string v2,std::string v3);
		bool			cpps_string_empty();
		bool			cpps_string_startswith(std::string v);
		bool			cpps_string_endswith(std::string v);
		std::string		cpps_string_sub(cpps_integer off,object len);
		cpps_integer	cpps_string_at(cpps_integer off);
		std::string		cpps_string_tolower();
		std::string		cpps_string_toupper();
		std::string		cpps_string_join(cpps_vector *vec);
		void			cpps_string_trim();
		void			cpps_string_ltrim();
		void			cpps_string_rtrim();
		void			cpps_string_pop_back(object len);
		void			cpps_string_push_back(cpps_integer charcode);
		void			cpps_string_append(std::string v);
	};
	void	cpps_regstring(C *c);

}

#endif