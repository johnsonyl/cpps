#ifndef CPPS_CLASS_HANDLER_CPPS_HEAD_
#define CPPS_CLASS_HANDLER_CPPS_HEAD_

//===================================
//@Author		:	Johnson
//@QQ			:	88481106
//@Email		:	88481106@qq.com
//@Date			:	2015/11/20 (yy/mm/dd)
//@Module		:	CPPS_MODULE
//@Description	:	Cpp×¢²áµ½Cpps½Ó¿Ú
//@website		:	http://cppscript.org
//==================================
namespace cpps 
{
	class cpps_class_handler
	{
	public:
		cpps_class_handler();
		~cpps_class_handler();

		cpps_value		def_call(C* c, cpps::cpps_value ps, ...);
		cpps_value		def_operator_call(C* c, cpps_value right);
		void*			get_ptr();
		void*		__ptr;
		FUNCMAP		*__func;
		cpps_class_free __free;
	};



}



#endif //CPPS_CLASS_HANDLER_CPPS_HEAD_