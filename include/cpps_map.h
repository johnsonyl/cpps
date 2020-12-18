#ifndef CPPS_MAP_CPPS_HEAD_
#define CPPS_MAP_CPPS_HEAD_

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

	typedef std::map<cpps_value, cpps_value> std_map;

	struct cpps_map
	{
		virtual ~cpps_map();
		void				insert(cpps_value k, cpps_value v);
		cpps_value			find(cpps_value k);
		cpps_value&			cpps_find(cpps_value k);
		void				erase(cpps_value k);
		void				begin();
		bool				has(cpps_value k);
		bool				end();
		bool				empty();
		void				next();
		cpps_value			it();
		void				pop();
		cpps_value			key();
		void				clear();
		cpps_integer		size();
		cpps_value&		operator[](cpps_value k)
		{
			return _map[k];
		}
		std::map<cpps_value, cpps_value>& realmap();
	private:
		std::map<cpps_value, cpps_value> _map;
		std::map<cpps_value, cpps_value>::iterator _begin;
	public:
		void set(cpps_value key_val, cpps_value value);
	};

	
	typedef std::unordered_map<cpps_value, cpps_value, cpps_value::hash> std_unordered_map;

	struct cpps_unordered_map
	{

		virtual ~cpps_unordered_map();
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
		void			set(cpps_value key_val, cpps_value value);

		cpps_value&		operator [] (cpps_value k)
		{
			return _map[k];
		}
		std::unordered_map<cpps_value, cpps_value, cpps_value::hash>& realmap();
	private:
		std::unordered_map<cpps_value, cpps_value, cpps_value::hash> _map;
		std::unordered_map<cpps_value, cpps_value, cpps_value::hash>::iterator _begin;
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
	struct cpps_converter<std::map<cpps_value, cpps_value>*>
	{
		static bool	match(cpps_value obj)
		{
			if (obj.tt != CPPS_TCLASSVAR) return false;
			cpps::cpps_cppsclass *cls = (cpps::cpps_cppsclass *)obj.value.domain->parent[0];
			if (cls->getclassname() != "map") return false;

			return true;
		}
		static std::map<cpps_value, cpps_value>*		apply(cpps_value obj)
		{
			cpps_cppsclassvar *clsvar = (cpps_cppsclassvar *)obj.value.domain;
			cpps::cpps_map *m = static_cast<cpps::cpps_map*>(clsvar->getclsptr());

			return &m->realmap();
		}
	};

	template<>
	struct cpps_converter<std::unordered_map<cpps_value, cpps_value, cpps_value::hash>*>
	{
		static bool	match(cpps_value obj)
		{
			if (obj.tt != CPPS_TCLASSVAR) return false;
			cpps::cpps_cppsclass *cls = (cpps::cpps_cppsclass *)obj.value.domain->parent[0];
			if (cls->getclassname() != "unordered_map") return false;

			return true;
		}
		static std::unordered_map<cpps_value, cpps_value, cpps_value::hash>*		apply(cpps_value obj)
		{
			cpps_cppsclassvar *clsvar = (cpps_cppsclassvar *)obj.value.domain;
			cpps::cpps_unordered_map *m = static_cast<cpps::cpps_unordered_map*>(clsvar->getclsptr());

			return &m->realmap();
		}
	};
}

#endif