#ifndef CPPS_ARRAY_CPPS_HEAD_
#define CPPS_ARRAY_CPPS_HEAD_

//===================================
//@Author		:	Johnson
//@QQ			:	88481106
//@Email		:	88481106@qq.com
//@Date			:	2015/12/24 (yy/mm/dd)
//@Module		:	CPPS_ARRAY
//@Description	:	vector×¢²áÀà
//@website		:	http://cpps.wiki
//==================================


namespace cpps
{

	struct cpps_vector
	{
		cpps_vector();
		virtual ~cpps_vector();
		void								push_back(cpps_value v);
		void								assign(cpps_vector* v);
		void								pop_back();
		void								push_front(cpps_value v);
		void								pop_front();
		void								insert(cpps_integer idx, cpps_value v);
		cpps_value							at(cpps_integer idx);
		cpps_value&							cpps_at(cpps_integer idx);
		void								erase(cpps_integer idx);
		void								pop();
		void								begin();
		bool								end();
		bool								empty();
		void								next();
		cpps_value							it();
		void								clear();
		bool								has(cpps_value v);
		cpps_integer						size();
		void								resize(cpps_integer s);
		cpps_value							where(C* c, object o);
		cpps_value							select(C* c, object o);
		cpps_std_vector&							realvector();
	private:
		cpps_std_vector							_vec;
		cpps_std_vector::iterator				_begin;

	};
	void	cpps_regarray(C *c);

	template<>
	struct cpps_converter<cpps_std_vector*>
	{
		static bool	match(cpps_value obj)
		{
			if (obj.tt != CPPS_TCLASSVAR) return false;
			cpps::cpps_cppsclass *cls = (cpps::cpps_cppsclass *)obj.value.domain->parent[0];
			if (cls->getclassname() != "vector") return false;

			return true;
		}
		static cpps_std_vector*		apply(cpps_value obj)
		{
			cpps_cppsclassvar *clsvar = (cpps_cppsclassvar *)obj.value.domain;
			cpps::cpps_vector *m = static_cast<cpps::cpps_vector*>(clsvar->getclsptr());

			return &m->realvector();
		}
	};
}

#endif // CPPS_ARRAY_CPPS_HEAD_