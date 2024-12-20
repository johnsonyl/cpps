#ifndef CPPS_ARRAY_CPPS_HEAD_
#define CPPS_ARRAY_CPPS_HEAD_

//===================================
//@Author		:	Johnson
//@QQ			:	88481106
//@Email		:	88481106@qq.com
//@Date			:	2015/12/24 (yy/mm/dd)
//@Module		:	CPPS_ARRAY
//@Description	:	vectorע����
//@website		:	http://cppscript.org
//==================================


namespace cpps
{
	struct cpps_vector
	{
		cpps_vector();
		virtual ~cpps_vector();
		void								constructor(object v);
		void								emplace_back(object v);
		void								push_back(object v);
		void								assign(cpps_vector* v);
		void								extend(cpps_vector* v);
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
		void								remove(C* c, object o);
		cpps_value							reverse(C* c);
		cpps_value							select(C* c, object o);
		cpps_value							andfunc (C* c, object o);
		cpps_value							multiplication(C* c, object o);
		cpps_integer						indexof(cpps_value o);
		void								swap(cpps_vector* v);
		void								shrink_to_fit();
		cpps_std_vector&					realvector();
		void								remove_vec(cpps_vector* v);
		cpps_value							orderby_call(C* c, object& func, cpps_value& v);
		size_t								partition(C* c, cpps_std_vector& v, size_t begin, size_t end, object& func);
		void								quick_sort(C* c, cpps_std_vector& v, size_t begin, size_t end,object& func);
		cpps_vector*						orderby(C* c, object o);
	private:
		cpps_std_vector						_vec;
		cpps_std_vector::iterator			_begin;
	};
	void	cpps_regarray(C *c);

	template<>
	struct cpps_converter<cpps_std_vector*>
	{
		static bool	match(cpps_value obj)
		{
			if (obj.tt != CPPS_TCLASSVAR) return false;
			if (obj.is_kindof<cpps_vector>()) return false;

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