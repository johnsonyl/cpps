#ifndef CPPS_CLASSVAR_QUATO_CPPS_HEAD_
#define CPPS_CLASSVAR_QUATO_CPPS_HEAD_

//===================================
//@Author		:	Johnson
//@QQ			:	88481106
//@Email		:	88481106@qq.com
//@Date			:	2023/12/19 (yy/mm/dd)
//@Module		:	CLASSVAR_QUATO
//@Description	:	类变量引用
//@website		:	http://cppscript.org
//==================================

namespace cpps
{
	struct cpps_classvar_quato
	{
		cpps_classvar_quato();
		virtual ~cpps_classvar_quato();
		virtual void setvarname(std::string name);
		virtual std::string getvarname();
		virtual cpps_cppsclass* getclass();
		virtual void setclass(cpps_cppsclass* cls);
		virtual void setvalue(cpps_domain* domain, cpps_value _v);
		virtual cpps_value getvalue(cpps_domain* domain);
		virtual void release();

		std::string		varname;
		cpps_cppsclass* _cls;
	};

	template<class _C>
	struct cpps_classvar_quato_template : public cpps_classvar_quato
	{

		cpps_classvar_quato_template(std::string name, object _C::* _v) { setvarname(name); v = _v; }
		virtual ~cpps_classvar_quato_template() {}
		virtual void setvalue(cpps_domain* domain, cpps_value _v) {
			cpps_classvar<_C>* cls = static_cast<cpps_classvar<_C>*>(domain);
			object& val = (cls->__class->*v);
			val = _v.real();
		}
		virtual cpps_value getvalue(cpps_domain* domain) {
			cpps_classvar<_C>* cls = static_cast<cpps_classvar<_C>*>(domain);
			object& val = (cls->__class->*v);
			return val.ref();
		}
		virtual void release() { CPPSDELETE(this); }

		object _C::* v;
	};
	template<typename _C>
	cpps_reg_class_var* make_regclassvar(std::string name, object _C::* v)
	{
		return CPPSNEW(cpps_reg_class_var)(name, CPPSNEW(cpps_classvar_quato_template<_C>)(name, v));
	}
}
#endif