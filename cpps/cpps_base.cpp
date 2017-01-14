#include "cpps.h"

using namespace std;

namespace cpps
{
	void cpps_base_printf(object b)
	{
		if (type(b) == CPPS_TNUMBER)
		{
			double s = object_cast<double>(b);
			cout << s;
		}
		else if (type(b) == CPPS_TINTEGER)
		{
			int32 s = object_cast<int32>(b);
			cout << s;
		}
		else if (type(b) == CPPS_TSTRING)
		{
			std::string s = object_cast<std::string>(b);
			cout << s.c_str();
		}
		else if (type(b) == CPPS_TBOOLEAN)
		{
			bool bl = object_cast<bool>(b);
			cout << (bl ? "true" : "false");
		}
		else if (type(b) == CPPS_TNIL)
		{
			cout << "nil";
		}
		else
		{
			cout << "暂时不支持 '" << type_s(b).c_str() << "' 类型的输出.";
		}
	}
	void cpps_base_printfln(object b)
	{
		if (type(b) == CPPS_TNUMBER)
		{
			double s = object_cast<double>(b);
			cout << s << endl;
		}
		else if (type(b) == CPPS_TINTEGER)
		{
			int32 s = object_cast<int32>(b);
			cout << s << endl;
		}
		else if (type(b) == CPPS_TSTRING)
		{
			std::string s = object_cast<std::string>(b);
			cout << s.c_str() << endl;
		}
		else if (type(b) == CPPS_TBOOLEAN)
		{
			bool bl = object_cast<bool>(b);
			cout << (bl ? "true" : "false") << endl;
		}
		else if (type(b) == CPPS_TNIL)
		{
			cout << "nil" << endl;
		}
		else
		{
			cout << "暂时不支持 '" << type_s(b).c_str() << "' 类型的输出." << endl;
		}
	}

	void cpps_base_sleep(cpps_integer msec)
	{
#ifdef _WIN32
		Sleep((usint32)msec);
#else
		usleep((int32)(msec * 1000));
#endif
	}

	cpps_number cpps_base_tonumber(cpps_value v)
	{
		return cpps_to_number(v);
	}
	cpps_integer cpps_base_tointeger(cpps_value v)
	{
		return cpps_to_integer(v);
	}
	std::string cpps_base_tostring(cpps_value v)
	{
		return cpps_to_string(v);
	}
	void cpps_base_system(std::string v)
	{
		system(v.c_str());
	}
	void cpps_base_setconsoletitle(std::string title)
	{
#ifdef WIN32
		SetConsoleTitleA(title.c_str());
#endif
	}
	void cpps_base_foreach(C *c,cpps_value va, cpps_value func)
	{
		if (va.isDomain() && func.tt == CPPS_TFUNCTION)
		{
			if (va.value.domain->domainName == "vector")
			{
				cpps_vector *v = cpps_converter<cpps_vector*>::apply(va);
				if (v)
				{
					for (v->begin(); v->end();)
					{
						object r = dofunction(c, func, v->it());
						if (type(r) == CPPS_TBOOLEAN)
						{
							bool ret = object_cast<bool>(r);
							v->pop();
						}
						else
							v->next();
					}
				}
			}
			else if (va.value.domain->domainName == "map" )
			{
				cpps_map *v = cpps_converter<cpps_map*>::apply(va);
				if (v)
				{
					for (v->begin(); v->end(); )
					{
						object r = dofunction(c, func, v->key(),v->it());
						if (type(r) == CPPS_TBOOLEAN)
						{
							bool ret = object_cast<bool>(r);
							v->pop();
						}
						else
							v->next();
					}
				}
			}
			else if (va.value.domain->domainName == "unordered_map")
			{
				cpps_unordered_map *v = cpps_converter<cpps_unordered_map*>::apply(va);
				if (v)
				{
					for (v->begin(); v->end(); )
					{
						object r = dofunction(c, func, v->key(), v->it());
						if (type(r) == CPPS_TBOOLEAN)
						{
							bool ret = object_cast<bool>(r);
							v->pop();
						}
						else
							v->next();
					}
				}
			}
		}
	}
	void cpps_regbase(C *c)
	{
		module(c)[
			_class<C>("C"),
			def("printf", cpps_base_printf),
			def("print", cpps_base_printf),
			def("printfln", cpps_base_printfln),
			def("println", cpps_base_printfln),
			def("exit",exit),
			def("sleep", cpps_base_sleep),
			def("Sleep", cpps_base_sleep),
			def("tonumber", cpps_base_tonumber),
			def("toint", cpps_base_tointeger),
			def("tostring", cpps_base_tostring),
			def_inside("foreach", cpps_base_foreach),
			def("system", cpps_base_system),
			def("SetConsoleTitle", cpps_base_setconsoletitle)
		];

	}
}