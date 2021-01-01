#include "cpps/cpps.h"

using namespace std;

namespace cpps
{
	bool cpps_io_file_exists(std::string path);
	void cpps_load_filebuffer(const char* path, std::string& fileSrc);
	std::string getfilenamenotext(std::string str);
	std::string cpps_rebuild_filepath(std::string path);
	cpps_integer cpps_math_rand();
	bool cpps_base_isdebug() {
#ifdef _DEBUG
		return true;
#else
		return false;
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
		}
		return ret;
	}
	object cpps_base_dump(C*c,object o) {
		if (type(o) == CPPS_TCLASSVAR)
		{
			object dump = o["dump"];
			if (dump.isfunction()){
				return doclassfunction(c, o, dump);
			}
		}
		return nil;
	}
	void cpps_base_printf(object b)
	{
		if (type(b) == CPPS_TNUMBER)
		{
			cpps_number s = object_cast<cpps_number>(b);
			cout.precision(20);
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
			else
			{
				cout << "class <" << b.value.value.domain->domainname << ">";
			}
		}
		else if (type(b) == CPPS_TMULTIRETURN)
		{
			cpps_vector* vec = cpps_to_cpps_vector(b.value);
			cout << "[";
			for (auto v : vec->realvector())
			{
				cpps_base_printf(v);
				cout << ",";
			}
			cout << "]";

		}
		else
		{
			cout << "not support  '" << type_s(b).c_str() << "' type..";
		}
	}
	void cpps_base_printfln(object b)
	{
		cpps_base_printf(b);
		cout << endl;
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
		return (v.isdomain() && (v.value.domain->domainname == "map"));
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
	bool cpps_base_isclassvar(cpps_value v)
	{
		return v.tt == CPPS_TCLASSVAR;
	}
	bool cpps_base_isclass(cpps_value v)
	{
		return v.tt == CPPS_TCLASS;
	}
	bool cpps_base_isfunction(cpps_value v)
	{
		return v.tt == CPPS_TFUNCTION || v.tt == CPPS_TLAMBDAFUNCTION;
	}
	cpps_integer cpps_base_objtype(cpps_value v)
	{
		if (v.tt == CPPS_TLAMBDAFUNCTION) return CPPS_TFUNCTION;
		return v.tt;
	}
	std::string cpps_base_type(cpps_value v) {
		std::string ret = "unknow type";
		switch (cpps_base_objtype(v))
		{
		case CPPS_TNIL:
			return "nil";
		case CPPS_TINTEGER:
			return "integer";
		case CPPS_TNUMBER:
			return "number";
		case CPPS_TBOOLEAN:
			return "boolean";
		case CPPS_TSTRING:
			return "string";
		case CPPS_TCLASS:
		{
			return std::string("class ") + v.value.domain->domainname;
		}
		case CPPS_TCLASSVAR:
		{
			return v.value.domain->domainname;
		}
		case CPPS_TFUNCTION:
			return "function";
		default:
			break;
		}
		return ret;
	}
	int32 cpps_base_system(std::string v)
	{
		return system(v.c_str());
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

			c->modulelist.insert(phmap::flat_hash_map<std::string, HMODULE>::value_type(libname, module));
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

			c->modulelist.insert(phmap::flat_hash_map<std::string, HMODULE>::value_type(libname, mod));
			sv = true;

			api->cpps_attach(c);


		}
#endif
		fpath = cpps_rebuild_filepath(path + "main.cpp");
		if (!fpath.empty())
		{
			if(!sv)
				c->modulelist.insert(phmap::flat_hash_map<std::string, HMODULE>::value_type(libname, NULL));
			std::string fileSrc;
			cpps_load_filebuffer(fpath.c_str(), fileSrc);
			node* o = loadbuffer(c,  fileSrc, fpath);
			cpps_stack* stack = c->stack_alloc();
			stack->init("main.cpp", 0, "import");

			c->push_stack(stack);
			cpps_step_all(c, CPPS_MUNITRET, c->_G,c->_G, o);
			c->pop_stack();


			cpps_gc_check_step(c);

			c->stack_free(stack);
		}
		return true;
	}

	void cpps_detach_library(HMODULE module,const std::string& libname, C* c)
	{
		//printf("cpps_detach_library -> %s\r\n", libname.c_str());
		if (module == NULL) return;
#ifdef _WIN32
		std::string libfuncname = "cpps_detach";

		cpps_detach_func cpps_detach = (cpps_detach_func)GetProcAddress(module, libfuncname.c_str());
		if (cpps_detach == NULL)
		{
			printf("Free Module【%s】 faild.\r\n", libname.c_str());
		}
		else
		{
			cpps_detach(c);
			FreeLibrary(module);
		}
#else
		dlerror();
		CPPS_ST_API* api = (CPPS_ST_API*)dlsym(module, "LIBAPI");
		if (api == NULL)
		{
			printf("dlsym [LIBAPI] faild\r\n");
		}
		else
		{
			api->cpps_detach(c);
#if defined LINUX
			dlclose(module);
#endif
		}
#endif
	}

	bool	cpps_freelibrary(C* c, std::string libname)
	{

		if (c->modulelist.find(libname) == c->modulelist.end()) return true;

#ifdef _WIN32

		bool ret = false;
		phmap::flat_hash_map<std::string, HMODULE>::iterator it = c->modulelist.find(libname);
		if (it != c->modulelist.end())
		{
			HMODULE module = it->second;
			if (module) {
				cpps_detach_library(module, libname, c);
			}
			c->modulelist.erase(it);
			ret = true;
		}
		return ret;

#else
		bool ret = false;
		phmap::flat_hash_map<std::string, HMODULE>::iterator it = c->modulelist.find(libname);
		if (it != c->modulelist.end())
		{
			HMODULE module = it->second;
			if (module) {
				cpps_detach_library(module, libname, c);
			}
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
		if (serializer_type == 1 && classvar->stacklist) {
			cpps_vector* vct;
			ret = newclass<cpps_vector>(c, &vct);
			for (auto var : *classvar->stacklist) 
				vct->push_back(var->getval());
		}
		else if(serializer_type == 2){
			cpps_map* m;
			ret = newclass<cpps_map>(c, &m);
			for (auto var : classvar->varList)
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
			cpps_value v = newcppsclasvar(c, cppsclass);
			if (vct->realvector().size() == v.value.domain->stacklist->size())
			{
				size_t c = vct->size();
				
				for (size_t i = 0; i < c; i++) {
					cpps_regvar* regv = (*(v.value.domain->stacklist))[i];
					regv->setval(vct->realvector()[i]);
				}
				ret = v;
			}
		}
		else if(cpps_base_ismap(v)){
			cpps_map* m = cpps_to_cpps_map(v);
			cpps_value v = newcppsclasvar(c, cppsclass);
			if (m->realmap().size() == v.value.domain->stacklist->size())
			{
				for (auto it : m->realmap()) {
					cpps_domain* leftdomain = NULL;
					cpps_regvar * regv = v.value.domain->getvar(cpps_to_string(it.first), leftdomain, false);
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
	size_t partition(cpps_std_vector& v, size_t begin, size_t end)
	{
		cpps_value pivot = v[begin];
		size_t left = begin + 1;
		size_t right = end;
		while (true)
		{
			while (left < right && v[right] >= pivot)
				right--;
			while (left < right && v[left] < pivot)
				left++;
			if (left == right)
				break;
			std::swap(v[left], v[right]);
		}
		if (v[left] >= pivot)
			return begin;
		v[begin] = v[left];
		v[left] = pivot;
		return left;
	}
	void quickSort(cpps_std_vector& v, size_t begin, size_t end)
	{
		if (begin >= end)
			return;
		size_t boundary = partition(v, begin, end);
		quickSort(v, begin, boundary - 1);
		quickSort(v, boundary + 1, end);
	}
	void cpps_base_sort(object v)
	{
		if (v.isvector()) {
			cpps_vector* vec = cpps_to_cpps_vector(v.value);
			quickSort(vec->realvector(), 0, vec->size() - 1);
		}
	}
	void cpps_base_real_shuffle(cpps_std_vector& vec)
	{
		size_t n = vec.size();
		if (n <= 0)
			return;

		for (size_t i = 0; i < n; i++)
		{
			//保证每次第i位的值不会涉及到第i位以前
			size_t index = i + ((size_t)cpps_math_rand()) % (n - i);
			std::swap(vec[index], vec[i]);
		}
	}
	void cpps_base_shuffle(object v)
	{
		if (v.isvector()) {
			cpps_vector* vec = cpps_to_cpps_vector(v.value);
			cpps_base_real_shuffle(vec->realvector());
		}
	}
	void cpps_base_exit(cpps_integer exitcode)
	{
		exit((int)exitcode);
	}
	void cpps_regbase(C *c)
	{
		
		cpps::_module(c)[
			_class<C>("C_STATE"),
			def("printf", cpps_base_printf),
			def("print", cpps_base_printf),
			def("printfln", cpps_base_printfln),
			def("println", cpps_base_printfln),
			def_inside("dump", cpps_base_dump),
			def("exit", cpps_base_exit),
			def("sleep", cpps_base_sleep),
			def("Sleep", cpps_base_sleep),
			def("tonumber", cpps_base_tonumber),
			def("double", cpps_base_tonumber),
			def("toint", cpps_base_tointeger),
			def("int", cpps_base_tointeger),
			def("tostring", cpps_base_tostring),
			def("str", cpps_base_tostring),
			def("isstring", cpps_base_isstring),
			def("isint", cpps_base_isint),
			def("isbool", cpps_base_isbool),
			def("isvector", cpps_base_isvector),
			def("ismap", cpps_base_ismap),
			def("isnull", cpps_base_isnull),
			def("isnumber", cpps_base_isnumber),
			def("isfunction", cpps_base_isfunction),
			def("isclassvar", cpps_base_isclassvar),
			def("isclass", cpps_base_isclass),
			def("objtype", cpps_base_objtype),
			def("type", cpps_base_type),
			def("system", cpps_base_system),
			def("len", cpps_base_len),
			def("sort", cpps_base_sort),
			def("shuffle", cpps_base_shuffle),
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
			defvar(c, "easyplatform", CPPS_CURRENT_EASYPLANTFORM),
			defvar(c, "os", CPPS_CURRENT_EASYPLANTFORM),
			defvar(c, "arch", CPPS_CURRENT_ARCH),
			defvar(c, "builder_version", CPPS_BUILDER_VERSION),
			defvar(c, "version", CPPS_VER),
			defvar(c, "versionno", CPPS_VERN),
#ifdef _DEBUG
			defvar(c, "debug", true)
#else
			defvar(c, "debug", false)
#endif
		];


	}
}
