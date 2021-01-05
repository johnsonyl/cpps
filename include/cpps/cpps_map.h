#ifndef cpps_map_CPPS_HEAD_
#define cpps_map_CPPS_HEAD_

//===================================
//@Author		:	Johnson
//@QQ			:	88481106
//@Email		:	88481106@qq.com
//@Date			:	2015/11/25 (yy/mm/dd)
//@Module		:	CPPS_DOMAIN
//@Description	:	创建红黑树接口
//@website		:	http://cpps.wiki
//==================================


namespace cpps
{
	void cpps_base_printf(object b);

	typedef phmap::flat_hash_map<cpps_value, cpps_value, cpps_value::hash> cpps_hash_map;

	struct cpps_map
	{

		virtual ~cpps_map();
		void			insert(cpps_value k, cpps_value v);
		cpps_value		find(cpps_value k);
		cpps_value&		cpps_find(const cpps_value & k);
		void			erase(cpps_value k);
		void			begin();
		bool			has(cpps_value k);
		bool			end();
		bool			empty();
		void			next();
		cpps_value		it();
		void			pop();
		cpps_value		key();
		void			clear();
		cpps_integer	size();
		void			merge(cpps_value right);
		void			set(cpps_value key_val, cpps_value value);

		cpps_value		where(C* c, object o);
		cpps_value		select(C* c, object o);
		cpps_value& operator [] (cpps_value k)
		{
			return _map[k];
		}
		cpps_hash_map& realmap();
	private:
		cpps_hash_map _map;
		cpps_hash_map::iterator _begin;
	public:
	};

	void	cpps_regmap(C *c);

	class cpps_map_node
	{
	public:
		cpps_map_node(){}
		cpps_value first() {
			return _first;
		}
		cpps_value second() {
			return _second;
		}
		cpps_value _first;
		cpps_value _second;
	};

	template<>
	struct cpps_converter<cpps_hash_map*>
	{
		static bool	match(cpps_value obj)
		{
			if (obj.tt != CPPS_TCLASSVAR) return false;
			cpps::cpps_cppsclass *cls = (cpps::cpps_cppsclass *)obj.value.domain->parent[0];
			if (cls->getclassname() != "map") return false;

			return true;
		}
		static cpps_hash_map*		apply(cpps_value obj)
		{
			cpps_cppsclassvar *clsvar = (cpps_cppsclassvar *)obj.value.domain;
			cpps::cpps_map *m = static_cast<cpps::cpps_map*>(clsvar->getclsptr());

			return &m->realmap();
		}
	};
}

#endif