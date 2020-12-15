#include "cpps.h"

using namespace std;

namespace cpps
{
	bool cpps_io_file_exists(std::string path);
	void cpps_load_filebuffer(const char* path, std::string& fileSrc);
	std::string getfilenamenotext(std::string str);
	std::string cpps_rebuild_filepath(std::string path);
	bool cpps_base_isdebug() {
#ifdef NDEBUG
		return false;
#else
		return true;
#endif
	}
	bool cpps_base_isset(cpps_value v)
	{
		return v.tt != CPPS_TNIL;
	}
	cpps_integer cpps_base_len(object b)
	{
		cpps_integer ret = 0;
		if (type(b) == CPPS_TSTRING)
		{
			std::string* s = cpps_get_string(b.value);
			ret = (cpps_integer)s->size();
		}
		else if (type(b) == CPPS_TCLASSVAR)
		{
			if (b.value.value.domain->domainname == "vector")
			{
				cpps_vector* v = cpps_converter<cpps_vector*>::apply(b.value);
				ret = (cpps_integer)v->size();
			}
			else if (b.value.value.domain->domainname == "map")
			{
				cpps_map* v = cpps_converter<cpps_map*>::apply(b.value);
				ret = (cpps_integer)v->size();
			}
			else if (b.value.value.domain->domainname == "unordered_map")
			{
				cpps_unordered_map* v = cpps_converter<cpps_unordered_map*>::apply(b.value);
				ret = (cpps_integer)v->size();
			}
		}
		return ret;
	}
	void cpps_base_printf(object b)
	{
		if (type(b) == CPPS_TNUMBER)
		{
			double s = object_cast<double>(b);
			cout << s;
		}
		else if (type(b) == CPPS_TINTEGER)
		{
			int64 s = object_cast<int64>(b);
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
		else if (type(b) == CPPS_TCLASSVAR)
		{
			if (b.value.value.domain->domainname == "vector")
			{
				cout << "[";
				cpps_vector* v = cpps_converter<cpps_vector*>::apply(b.value);
				if (v)
				{
					for (v->begin(); v->end(); v->next())
					{
						cpps_base_printf(v->it());
						cout << ",";
					}
				}
				cout << "]";
			}
			else if (b.value.value.domain->domainname == "map")
			{
				cout << "{";
				cpps_map* v = cpps_converter<cpps_map*>::apply(b.value);
				if (v)
				{
					for (v->begin(); v->end(); v->next())
					{
						cpps_base_printf(v->key());
						cout << ":";
						cpps_base_printf(v->it());
						cout << ",";
					}
				}
				cout << "}";
			}
			else if (b.value.value.domain->domainname == "unordered_map")
			{
				cout << "{";
				cpps_unordered_map* v = cpps_converter<cpps_unordered_map*>::apply(b.value);
				if (v)
				{
					for (v->begin(); v->end(); v->next())
					{
						cpps_base_printf(v->key());
						cout << ":";
						cpps_base_printf(v->it());
						cout << ",";
					}
				}
				cout << "}";
			}

		}
		else
		{
			cout << "not support  '" << type_s(b).c_str() << "' type..";
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
			int64 s = object_cast<int64>(b);
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
		else if (type(b) == CPPS_TCLASSVAR)
		{

			if (b.value.value.domain->domainname == "vector")
			{
				cout << "[";
				cpps_vector* v = cpps_converter<cpps_vector*>::apply(b.value);
				if (v)
				{
					for (v->begin(); v->end(); v->next())
					{
						cpps_base_printf(v->it());
						cout << ",";
					}
				}
				cout << "]";
			}
			else if (b.value.value.domain->domainname == "map")
			{
				cout << "{";
				cpps_map* v = cpps_converter<cpps_map*>::apply(b.value);
				if (v)
				{
					for (v->begin(); v->end(); v->next())
					{
						cpps_base_printf(v->key());
						cout << ":";
						cpps_base_printf(v->it());
						cout << ",";
					}
				}
				cout << "}";
			}
			else if (b.value.value.domain->domainname == "unordered_map")
			{
				cout << "{";
				cpps_unordered_map* v = cpps_converter<cpps_unordered_map*>::apply(b.value);
				if (v)
				{
					for (v->begin(); v->end(); v->next())
					{
						cpps_base_printf(v->key());
						cout << ":";
						cpps_base_printf(v->it());
						cout << ",";
					}
				}
				cout << "}";
			}
			cout << endl;
		}
		else
		{
			cout << "not support   '" << type_s(b).c_str() << "' type." << endl;
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
	bool cpps_base_isstring(cpps_value v)
	{
		return v.tt == CPPS_TSTRING;
	}
	bool cpps_base_isvector(cpps_value v)
	{
		return (v.isdomain() && v.value.domain->domainname == "vector");
	}
	bool cpps_base_ismap(cpps_value v)
	{
		return (v.isdomain() && (v.value.domain->domainname == "map" || v.value.domain->domainname == "unordered_map"));
	}
	bool cpps_base_isint(cpps_value v)
	{
		return v.tt == CPPS_TINTEGER;
	}
	bool cpps_base_isbool(cpps_value v)
	{
		return v.tt == CPPS_TBOOLEAN;
	}
	bool cpps_base_isnumber(cpps_value v)
	{
		return v.tt == CPPS_TNUMBER;
	}
	bool cpps_base_isnull(cpps_value v)
	{
		return v.tt == CPPS_TNIL;
	}
	bool cpps_base_isfunction(cpps_value v)
	{
		return v.tt == CPPS_TFUNCTION || v.tt == CPPS_TLAMBDAFUNCTION;
	}
	cpps_integer cpps_base_objtype(cpps_value v)
	{
		return v.tt;
	}
	void cpps_base_system(std::string v)
	{
		system(v.c_str());
	}
	void cpps_base_setconsoletitle(std::string title)
	{
#ifdef _WIN32
		SetConsoleTitleA(title.c_str());
#endif
	}
	
	void cpps_assert(bool b)
	{
		assert(b);
	}

	
	
	bool	cpps_loadlibrary(C *c,std::string libname)
	{
		std::string path = "lib/"+ libname + "/";
		std::string fpath;

		if (c->modulelist.find(libname) != c->modulelist.end()) return true;
		bool sv = false;
#ifdef _WIN32

		fpath = cpps_rebuild_filepath(path + (libname + ".dll"));
		if (!fpath.empty())
		{
			HMODULE module = ::LoadLibraryA(fpath.c_str());
			std::string libfuncname = "cpps_attach";
			if (module == NULL)
			{
				printf("Load module [%s] faild.\r\n", libname.c_str());
				FreeLibrary(module);
				return false;
			}

			cpps_attach_func cpps_attach = (cpps_attach_func)GetProcAddress(module, libfuncname.c_str());
			if (cpps_attach == NULL)
			{
				FreeLibrary(module);
				printf("Load module [%s] faild\r\n", libname.c_str());
				return false;
			}

			c->modulelist.insert(std::unordered_map<std::string, HMODULE>::value_type(libname, module));
			sv = true;

			cpps_attach(c);

		}
#else
#if defined LINUX
		fpath = cpps_rebuild_filepath(path + "lib" + (libname + ".so"));
#elif defined __APPLE__
		fpath = cpps_rebuild_filepath(path + "lib" + (libname + ".dylib"));
#endif
		if (!fpath.empty())
		{
			HMODULE mod = dlopen(fpath.c_str(), RTLD_LAZY);
			if (mod == NULL)
			{
				printf("dlopen [%s]:%s faild\r\n", libname.c_str(), dlerror());
				return false;
			}
			dlerror();
			CPPS_ST_API* api = (CPPS_ST_API*)dlsym(mod, "LIBAPI");
			if (api == NULL)
			{
				dlclose(mod);
				printf("dlsym [LIBAPI] faild\r\n");
				return false;
			}

			c->modulelist.insert(std::unordered_map<std::string, HMODULE>::value_type(libname, mod));
			sv = true;

			api->cpps_attach(c);


		}
#endif
		fpath = cpps_rebuild_filepath(path + "main.cpp");
		if (!fpath.empty())
		{
			if(!sv)
				c->modulelist.insert(std::unordered_map<std::string, HMODULE>::value_type(libname, NULL));
			std::string fileSrc;
			cpps_load_filebuffer(fpath.c_str(), fileSrc);
			node* o = loadbuffer(c,  fileSrc, fpath);
			cpps_stack* stack = c->stack_alloc();
			stack->init("main.cpp", 0, "import");

			c->push_stack(stack);
			cpps_step_all(c, CPPS_SINGLERET, c->_G,c->_G, o);
			c->pop_stack();


			cpps_gc_check_step(c);

			c->stack_free(stack);
		}
		return true;
	}
	bool	cpps_freelibrary(C*c, std::string libname)
	{

		if (c->modulelist.find(libname) == c->modulelist.end()) return true;

#ifdef _WIN32

		bool ret = false;
		std::unordered_map<std::string, HMODULE>::iterator it = c->modulelist.find(libname);
		if (it != c->modulelist.end())
		{
			HMODULE module = it->second;
			if (module) {
				std::string libfuncname = "cpps_detach";

				cpps_detach_func cpps_detach = (cpps_detach_func)GetProcAddress(module, libfuncname.c_str());
				if (cpps_detach == NULL)
				{
					printf("Free Module【%s】 faild.\r\n", libname.c_str());
				}
				else
				{
					cpps_detach(c);
				}


				FreeLibrary(module);
			}
			c->modulelist.erase(it);
			ret = true;
		}
		return ret;

#else
		bool ret = false;
		std::unordered_map<std::string, HMODULE>::iterator it = c->modulelist.find(libname);
		if (it != c->modulelist.end())
		{
			HMODULE module = it->second;



			dlerror();
			CPPS_ST_API* api = (CPPS_ST_API*)dlsym(module, "LIBAPI");
			if (api == NULL)
			{
				printf("dlsym [LIBAPI] faild\r\n");
			}
			else
			{
				api->cpps_detach(c);
			}


			dlclose(module);
			c->modulelist.erase(it);
			ret = true;
		}
		return ret;

#endif
	}
	cpps_value cpps_getargs(C* c)
	{
		cpps_vector* vct;
		cpps_value ret = newclass<cpps_vector>(c, &vct);
		for (int i = 0; i < c->application_argc; i++)
		{
			vct->push_back(cpps_value(c, c->application_argv[i]));
		}
		return ret;
	}
#if defined _WIN32
	std::string cpps_base_win_execmd(std::string pszCmd)
	{
		// 创建匿名管道
		SECURITY_ATTRIBUTES sa = { sizeof(SECURITY_ATTRIBUTES), NULL, TRUE };
		HANDLE hRead, hWrite;
		if (!CreatePipe(&hRead, &hWrite, &sa, 0))
		{
			return "";
		}

		// 设置命令行进程启动信息(以隐藏方式启动命令并定位其输出到hWrite
		STARTUPINFO si = { sizeof(STARTUPINFO) };
		GetStartupInfo(&si);
		si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
		si.wShowWindow = SW_HIDE;
		si.hStdError = hWrite;
		si.hStdOutput = hWrite;

		// 启动命令行
		PROCESS_INFORMATION pi;
		if (!CreateProcessA(NULL, (LPSTR)pszCmd.c_str(), NULL, NULL, TRUE, NULL, NULL, NULL, &si, &pi))
		{
			return "";
		}

		// 立即关闭hWrite
		CloseHandle(hWrite);

		// 读取命令行返回值
		std::string strRetTmp;
		char buff[4096] = { 0 };
		DWORD dwRead = 0;
		strRetTmp = buff;
		while (memset(buff, 0, sizeof(buff)),ReadFile(hRead, buff, 4095, &dwRead, NULL))
		{
			strRetTmp += buff;
		}
		CloseHandle(hRead);

		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);



		return strRetTmp;
	}
#endif
	std::string cpps_base_execmd(C* c, std::string cmd)
	{
#if defined _WIN32
		return cpps_base_win_execmd(cmd);
#else
		FILE *fp = popen(cmd.c_str(), "r");
		if (!fp) {
			return "";
		}
		std::string strRetTmp;
		char buf[1024] = { 0 };

		while (memset(buf, 0, sizeof(buf)), fgets(buf, sizeof(buf) - 1, fp) != 0) {
			strRetTmp += buf;
		}
		pclose(fp);
		return strRetTmp;
#endif
	}
	void cpps_base_environ_set(std::string k,::string v)
	{
#if defined _WIN32
		std::string setv = k + "=" + v;
		_putenv(setv.c_str());
#else
		setenv(k.c_str(), v.c_str(), 1);
#endif
	}
	std::string cpps_base_environ_get(std::string k)
	{
		char* pathvar;
		pathvar = getenv(k.c_str());
		if (!pathvar) return "";
		return pathvar;
	}
	cpps_value cpps_base_serializer_encode(C*c,cpps_value v,cpps_value tp)
	{
		if (v.tt != CPPS_TCLASSVAR) return nil;
		cpps_integer serializer_type = 1;//1. serializer.vector 2.serializer.map
		if (tp.tt == CPPS_TINTEGER) serializer_type = tp.value.integer;
		cpps_cppsclassvar* classvar = (cpps_cppsclassvar*)v.value.domain;


		cpps_value ret;
		if (serializer_type == 1) {
			cpps_vector* vct;
			ret = newclass<cpps_vector>(c, &vct);
			for (auto var : *classvar->stacklist) 
				vct->push_back(var->getval());
		}
		else if(serializer_type == 2){
			cpps_map* m;
			ret = newclass<cpps_map>(c, &m);
			for (auto var : classvar->varList)
				if(var.first != "this")
					m->insert(cpps_value(c,var.first),var.second->getval());
		}
		return ret;

	}
	cpps_value cpps_base_serializer_decode(C* c, cpps_value cls, cpps_value v)
	{
		cpps_value ret;

		if (cls.tt != CPPS_TCLASS) return nil;
		cpps_cppsclass* cppsclass = (cpps_cppsclass*)cls.value.domain;

		if (cpps_base_isvector(v)) {
			cpps_vector* vct = cpps_to_cpps_vector(v);
			cpps_cppsclassvar *v = newcppsclasvar(c, cppsclass);
			if (vct->realvector().size() == v->stacklist->size())
			{
				size_t c = vct->size();
				
				for (size_t i = 0; i < c; i++) {
					cpps_regvar* regv = (*(v->stacklist))[i];
					regv->setval(vct->realvector()[i]);
				}
				ret = v;
			}
		}
		else if(cpps_base_ismap(v)){
			cpps_map* m = cpps_to_cpps_map(v);
			cpps_cppsclassvar* v = newcppsclasvar(c, cppsclass);
			if (m->realmap().size() == v->stacklist->size())
			{
				for (auto it : m->realmap()) {
					cpps_domain* leftdomain = NULL;
					cpps_regvar * regv = v->getvar(cpps_to_string(it.first), leftdomain, false);
					if (regv)
					{
						regv->setval(it.second);
					}
				}
				ret = v;
			}

		}
		return ret;
	}
	void cpps_regbase(C *c)
	{
		cpps::_module(c)[
			_class<std::string>("String")
		];
		cpps::_module(c)[
			_class<C>("C_STATE"),
			def("printf", cpps_base_printf),
			def("print", cpps_base_printf),
			def("printfln", cpps_base_printfln),
			def("println", cpps_base_printfln),
			def("exit",exit),
			def("sleep", cpps_base_sleep),
			def("Sleep", cpps_base_sleep),
			def("tonumber", cpps_base_tonumber),
			def("double", cpps_base_tonumber),
			def("toint", cpps_base_tointeger),
			def("int", cpps_base_tointeger),
			def("tostring", cpps_base_tostring),
			def("isstring", cpps_base_isstring),
			def("isint", cpps_base_isint),
			def("isbool", cpps_base_isbool),
			def("isvector", cpps_base_isvector),
			def("ismap", cpps_base_ismap),
			def("isnull", cpps_base_isnull),
			def("isnumber", cpps_base_isnumber),
			def("isfunction", cpps_base_isfunction),
			def("objtype", cpps_base_objtype),
			def("system", cpps_base_system),
			def("len", cpps_base_len),
			def("isset", cpps_base_isset),
			def("isdebug", cpps_base_isdebug),
			def("SetConsoleTitle", cpps_base_setconsoletitle),
			def("assert", cpps_assert),
			defvar(c,"_VERSION", CPPS_VER),
			defvar(c,"_VERSIONNO", CPPS_VERN),
			def_inside("loadlibrary", cpps_loadlibrary),
			def_inside("freelibrary", cpps_freelibrary),
			def_inside("getargs", cpps_getargs),
			def_inside("execmd",cpps_base_execmd)
		];
		cpps::_module(c, "ot")[
			defvar(c,"int", CPPS_TINTEGER),
			defvar(c,"bool", CPPS_TBOOLEAN),
			defvar(c,"string", CPPS_TSTRING),
			defvar(c,"classvar", CPPS_TCLASSVAR),
			defvar(c,"function", CPPS_TFUNCTION),
			defvar(c,"domain", CPPS_TDOMAIN),
			defvar(c,"ptr", CPPS_TREGVAR),
			defvar(c,"number", CPPS_TNUMBER),
			defvar(c,"nil", CPPS_TNIL),
			defvar(c,"lambda", CPPS_TLAMBDAFUNCTION)
		];

		cpps::_module(c, "environ")[
			def("get",cpps_base_environ_get),
			def("set",cpps_base_environ_set)
		];
		cpps::_module(c, "serializer")[
			def_inside("encode",cpps_base_serializer_encode),
			def_inside("decode",cpps_base_serializer_decode),
			defvar(c,"vector",1),
			defvar(c,"map",2)
		];
		cpps::_module(c, "sys")[
			defvar(c, "platform", CPPS_CURRENT_PLANTFORM),
			defvar(c, "builder_version", CPPS_BUILDER_VERSION),
			defvar(c, "version", CPPS_VER),
			defvar(c, "versionno", CPPS_VERN)
		];

	}
}
