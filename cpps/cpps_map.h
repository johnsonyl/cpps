#ifndef CPPS_MAP_CPPS_HEAD_
#define CPPS_MAP_CPPS_HEAD_

//===================================
//@Author		:	Johnson
//@QQ			:	88481106
//@Email		:	jiang_4177@163.com
//@Date			:	2015/11/25 (yy/mm/dd)
//@Module		:	CPPS_DOMAIN
//@Description	:	创建红黑树接口
//@website		:	http://cpps.wiki
//==================================


namespace cpps
{
	void cpps_base_printf(object b);

	struct cpps_map
	{
		void			insert(cpps_value k, cpps_value v)
		{
			_map.insert(std::map<cpps_value, cpps_value>::value_type(k, v));
		}
		cpps_value		find(cpps_value k)
		{
			cpps_value ret;
			std::map<cpps_value, cpps_value>::iterator it = _map.find(k);
			if (it != _map.end())
			{
				ret = it->second;
			}
			if (false)
			{
				printf("=================================================:");

				printf("notfind:");
				cpps_base_printf(k);
				printf("\n");

				std::map<cpps_value, cpps_value>::iterator it = _map.begin();
				for (;it != _map.end(); ++it)
				{
					printf("key:");
					cpps_base_printf(it->first);
					printf("value:");
					cpps_base_printf(it->second);
					printf("\n");
				}
			}
			return ret;
		}
		cpps_value&		cpps_find(cpps_value k)
		{
			return _map[k];
		}
		void			erase(cpps_value k)
		{
			_map.erase(k);
		}
		void			begin()
		{
			_begin = _map.begin();
		}
		bool			end()
		{
			return _begin != _map.end();
		}
		void			next()
		{
			if (_begin != _map.end())
				++_begin;
		}
		cpps_value			it()
		{
			cpps_value ret;
			if (_begin != _map.end())
			{
				ret = _begin->second;
			}
			return ret;
		}
		void			pop()
		{
			_begin = _map.erase(_begin);
		}
		cpps_value			key()
		{
			cpps_value ret;
			if (_begin != _map.end())
			{
				ret = _begin->first;
			}
			return ret;
		}
		void			clear()
		{
			_map.clear();
		}
		cpps_integer	size()
		{
			return (cpps_integer)_map.size();
		}
		cpps_value&		operator [] (cpps_value k)
		{
			return _map[k];
		}
	private:
		std::map<cpps_value, cpps_value> _map;
		std::map<cpps_value, cpps_value>::iterator _begin;
	};

	
	

	struct cpps_unordered_map
	{
		void			insert(cpps_value k, cpps_value v);
		cpps_value		find(cpps_value k);
		cpps_value&		cpps_find(const cpps_value & k);
		void			erase(cpps_value k);
		void			begin();
		bool			end();
		void			next();
		cpps_value			it();
		void			pop();
		cpps_value			key();
		void			clear();
		cpps_integer	size();

		cpps_value&		operator [] (cpps_value k)
		{
			return _map[k];
		}
	private:
		std::unordered_map<cpps_value, cpps_value, cpps_value::hash> _map;
		std::unordered_map<cpps_value, cpps_value, cpps_value::hash>::iterator _begin;
	};

	void	cpps_regmap(C *c);
}

#endif