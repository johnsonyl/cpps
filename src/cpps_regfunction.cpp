#include "cpps/cpps.h"


namespace cpps
{

	
	std::string transformat(std::string fmt, cpps_value v)
	{
		char buffer[1024] = {0};
		if (v.tt == CPPS_TSTRING)
		{

			std::string *tmpStr = cpps_get_string(v);

			sprintf(buffer, fmt.c_str(), tmpStr->c_str());
		}
		else if (v.tt == CPPS_TBOOLEAN)
		{
			sprintf(buffer, fmt.c_str(), v.value.b != 0);
		}
		else if (v.tt == CPPS_TNUMBER)
		{
			sprintf(buffer, fmt.c_str(), v.value.number);
		}
		else if (v.tt == CPPS_TINTEGER)
		{
			sprintf(buffer, fmt.c_str(), v.value.integer);
		}
		else if (v.tt == CPPS_TUINTEGER)
		{
			sprintf(buffer, fmt.c_str(), v.value.uinteger);
		}
		return buffer;
	}

	size_t formatcheck(C *c,std::string &fmt, size_t j, size_t i, std::string &take, cpps_value &v)
	{
		std::string  newstr = "";
		switch (fmt[j])
		{
		case 'd':
		case 'u':
		case 'o':
		case 'c':
		case 'i':
		case 'X':
		case 'x':
			newstr = transformat(take, cpps_to_integer(v));
			fmt.replace(i, take.size(), newstr);
			break;
		case 'f':
			newstr = transformat(take, cpps_to_number(v));
			fmt.replace(i, take.size(), newstr);
			break;
		case 's':
			newstr = transformat(take, cpps_value(c, cpps_to_string(v)));
			fmt.replace(i, take.size(), newstr);
			break;
		default:
			break;
		}
		return newstr.size();
	}

	cpps::cpps_regparentclass* make_parentclass(cpps_cppsclass* _cppsclass)
	{
		return CPPSNEW( cpps_regparentclass)(_cppsclass);
	}

	cpps_classvar_quato::cpps_classvar_quato() {}

	cpps_classvar_quato::~cpps_classvar_quato() {}

	void cpps_classvar_quato::setvarname(std::string name) { varname = name; }

	std::string cpps_classvar_quato::getvarname() { return varname; }

	cpps_cppsclass* cpps_classvar_quato::getclass() { return _cls; }

	void cpps_classvar_quato::setclass(cpps_cppsclass* cls) { _cls = cls; }

	void cpps_classvar_quato::setvalue(cpps_domain* domain, cpps_value _v) {}

	cpps_value cpps_classvar_quato::getvalue(cpps_domain* domain) { return nil; }

	void cpps_classvar_quato::release() { CPPSDELETE(this); }

	cpps_function::cpps_function() { isNeedC = false; nasync = false; _cls = NULL; }

	cpps_function::~cpps_function() {}

	void cpps_function::callfunction(C* c, cpps_value* ret, cpps_domain* domain, cpps_std_vector* o, cpps_stack* stack, std::vector<cpps_regvar*>* lambdastacklist) {}

	void cpps_function::setfuncname(std::string name) { funcname = name; }

	std::string cpps_function::getfuncname() { return funcname; }

	void cpps_function::release() { CPPSDELETE(this); }

	void cpps_function::setIsNeedC(bool b) { isNeedC = b; }

	bool cpps_function::getIsNeedC() { return isNeedC; }

	int8 cpps_function::getparamcount() { return 0; }

	bool cpps_function::isasync() { return nasync; }

	bool cpps_function::isoperator() { return noperator; }

	void cpps_function::setasync(bool b) { nasync = b; }

	void cpps_function::setoperator(bool b) { noperator = b; }

	bool cpps_function::iscppsfunc() { return false; }

	cpps_cppsclass* cpps_function::getclass() { return _cls; }

	void cpps_function::setclass(cpps_cppsclass* cls) { _cls = cls; }

	cpps_reg::cpps_reg()
	{
		next = NULL;
		type = 0;
		isneedC = false;
	}

	cpps_reg::~cpps_reg() {}

	void cpps_reg::release() { CPPSDELETE(this); }

	cpps_regfunction::cpps_regfunction(std::string f, cpps_function* func, bool isasync, bool isoperator)
		:func(func)
	{
		type = cpps_def_regfunction;
		varname = f;
		func->setfuncname(f);
		func->setasync(isasync);
		func->setoperator(isoperator);
	}

	cpps_regfunction::~cpps_regfunction() {}

	void cpps_regfunction::release() { CPPSDELETE(this); }

	cpps_reggvar::cpps_reggvar(std::string n, cpps_value v)
	{
		type = cpps_def_regvar;
		varname = n;
		value = v;
	}

	cpps_reggvar::~cpps_reggvar() {}

	void cpps_reggvar::release() { CPPSDELETE(this); }

	cpps_reg_class_var::cpps_reg_class_var(std::string n, cpps_classvar_quato* v)
	{
		type = cpps_def_regclassvar;
		varname = n;
		value.tt = CPPS_TUSERDATA;
		_v = v;
	}

	cpps_reg_class_var::~cpps_reg_class_var() {}

	void cpps_reg_class_var::release() { CPPSDELETE(this); }

	cpps_regparentclass::cpps_regparentclass(cpps_cppsclass* _cppsclass)
	{
		type = cpps_def_regparentclass;
		__cppsclass = _cppsclass;
	}

	cpps_regparentclass::~cpps_regparentclass() {}

	void cpps_regparentclass::release() { CPPSDELETE(this); }

	cpps_regclass::cpps_regclass(std::string name, cpps_cppsclass* c)
		:cls(c)
	{
		type = cpps_def_regclass;
		varname = name;
	}

	cpps_regclass::~cpps_regclass() {}

	void cpps_regclass::release() { CPPSDELETE(this); }

	cpps_regenum::cpps_regenum(std::string name, cpps_domain* c)
	{
		type = cpps_def_regenum;
		_enum_domain = c;
		varname = name;
	}

	cpps_regenum::~cpps_regenum() {}

	void cpps_regenum::release() { CPPSDELETE(this); }

}