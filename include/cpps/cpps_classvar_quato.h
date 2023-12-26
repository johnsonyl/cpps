#ifndef CPPSCLSLASSVAR_QUATOCLSPPS_HEAD_
#define CPPSCLSLASSVAR_QUATOCLSPPS_HEAD_

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

	template<class CLS>
	struct cpps_classvar_quato_template : public cpps_classvar_quato
	{

		cpps_classvar_quato_template(std::string name, object CLS::* _v) { setvarname(name); v = _v; }
		virtual ~cpps_classvar_quato_template() {}
		virtual void setvalue(cpps_domain* domain, cpps_value _v) {
			cpps_classvar<CLS>* cls = static_cast<cpps_classvar<CLS>*>(domain);
			object& val = (cls->__class->*v);
			val = _v.real();
		}
		virtual cpps_value getvalue(cpps_domain* domain) {
			cpps_classvar<CLS>* cls = static_cast<cpps_classvar<CLS>*>(domain);
			object& val = (cls->__class->*v);
			return val.ref();
		}
		virtual void release() { CPPSDELETE(this); }

		object CLS::* v;
	};
	template<typename CLS>
	cpps_reg_class_var* make_regclassvar(std::string name, object CLS::* v)
	{
		return CPPSNEW(cpps_reg_class_var)(name, CPPSNEW(cpps_classvar_quato_template<CLS>)(name, v));
	}
}
#endif