#include "cpps/cpps.h"

using namespace std;

namespace cpps
{
	void cpps_console_clearcolor();
	void cpps_console_color(cpps_integer color);
	bool cpps_io_file_exists(std::string path);
	void cpps_load_filebuffer(const char* path, std::string& fileSrc);
	std::string getfilenamenotext(std::string str);
	std::string cpps_rebuild_filepath(std::string path);
	cpps_integer cpps_math_rand();
	bool cpps_isbasevar(const cpps_value& v);
	bool cpps_io_isdir(std::string p);
	void cpps_debug_trace_domain(C* c, int kg, std::string parent, cpps_domain* root);
	std::string cpps_io_getfilepath(std::string str);
	std::string cpps_io_readfile(std::string filepath);
	std::string cpps_getcwd();

	void				cpps_step(C* c, cpps_domain* domain, cpps_domain* root, node* d);
	void				cpps_step_all(C* c, int32 retType, cpps_domain* domain, cpps_domain* root, node* o, bool);
	bool cpps_base_isdebug() {
#ifdef _DEBUG
		return true;
#else
		return false;
#endif
	}
	bool cpps_base_isvalid(cpps_value v)
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
			if (b.is_kindof<cpps_vector>())
			{
				cpps_vector* v = cpps_converter<cpps_vector*>::apply(b.value);
				ret = (cpps_integer)v->size();
			}
			else if (b.is_kindof<cpps_map>())
			{
				cpps_map* v = cpps_converter<cpps_map*>::apply(b.value);
				ret = (cpps_integer)v->size();
			}
			else if (b.is_kindof<cpps_set>())
			{
				cpps_set* v = cpps_converter<cpps_set*>::apply(b.value);
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
	void cpps_base_printf_new(C* c, cpps::cpps_value args, ...) {
		cpps::object _vct = object(args);
		bool isfirst = true;
		for (auto& v : cpps::object::vector(_vct)) {
			if (!isfirst) cout << ","; 
			isfirst = false;
			cpps_base_printf(c, v.real());
		}
	}
	void cpps_base_printf(C*c,object b)
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
		else if (type(b) == CPPS_TUINTEGER)
		{
			usint64 s = object_cast<usint64>(b);
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
			if (b.is_kindof<cpps_vector>())
			{
				cout << "[";
				cpps_vector* v = cpps_converter<cpps_vector*>::apply(b.value);
				if (v)
				{
					bool first = true;
					for (v->begin(); v->end(); v->next())
					{
						if (!first) cout << ",";
						first = false;
						bool bb = cpps_isstring(v->it());
						if(bb) cout << "'";
						cpps_base_printf(c,v->it());
						if (bb) cout << "'";
					}
				}
				cout << "]";
			}
			else if (b.is_kindof<cpps_set>())
			{
				cout << "[";
				bool first = true;
				cpps_set* v = cpps_converter<cpps_set*>::apply(b.value);
				if (v)
				{
					for (v->begin(); v->end(); v->next())
					{
						if(!first) cout << ",";
						first = false;
						bool bb = cpps_isstring(v->it());
						if (bb) cout << "'";
						cpps_base_printf(c, v->it());
						if (bb) cout << "'";
					}
				}
				cout << "]";
			}
			else if (b.is_kindof<cpps_map>())
			{
				cout << "{ ";
				cpps_map* v = cpps_converter<cpps_map*>::apply(b.value);
				if (v)
				{
					bool first = true;
					for (v->begin(); v->end(); v->next())
					{

						if (!first) cout << ",";
						first = false;
						cpps_base_printf(c,v->key());
						cout << ": ";
						bool bb = cpps_isstring(v->it());
						if (bb) {  cpps::cpps_console_color(2); cout << "'"; }
						else if(cpps_isbasevar(v->it())) { cpps::cpps_console_color(3); }
						cpps_base_printf(c,v->it());
						if (bb) { cout << "'";  }
						cpps::cpps_console_clearcolor();
					}
				}
				cout << " }";
			}
			else if (b.is_kindof<cpps_pair>()) {

				cpps_pair* v = cpps_converter<cpps_pair*>::apply(b.value);
				cout << "(";
				cpps_base_printf(c, v->first());
				cout << ",";
				cpps_base_printf(c, v->second());
				cout << ")";
			}
			else
			{
				object tostring = b["tostring"];
				if (tostring.isfunction()) {
					cpps_base_printf(c,doclassfunction(c,b, tostring));
				}
				else {
					cpps::cpps_console_color(4);
					cout << "class <" << b.value.value.domain->domainname << ">" << endl;
					cpps::cpps_console_clearcolor();
					cout << "{" << endl;
					cpps_debug_trace_domain(c, 4, "", b.value.value.domain);
					cout << "}" << endl;
				}
			}
		}
		else if (type(b) == CPPS_TTUPLE)
		{
			cpps_vector* vec = cpps_to_cpps_vector(b.value);
			cout << "[";
			bool first = true;
			for (auto v : vec->realvector())
			{

				if (!first) cout << ",";
				first = false;
				cpps_base_printf(c,v);
			}
			cout << "]";

		}
		else
		{
			cout << "not support  '" << type_s(b).c_str() << "' type..";
		}
	}
	void cpps_base_endl() {
		cout << endl;
	}
	void cpps_base_printfln_new(C* c, cpps::cpps_value args, ...) {
		bool isfirst = true;
		cpps::object _vct = object(args);
		for (auto& v : cpps::object::vector(_vct)) {
			if (!isfirst) cout << ","; 
			isfirst = false;
			cpps_base_printf(c, v.real());
		}
		cout << endl;
	}
	void cpps_base_printfln(C* c, object b) {
		cpps_base_printf(c,b);
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
	cpps_uinteger cpps_base_touinteger(cpps_value v)
	{
		return cpps_to_uinteger(v);
	}
	bool cpps_base_tobool(cpps_value v)
	{
		return cpps::object_cast<bool>(v);
	}
	std::string cpps_base_tostring(cpps_value v)
	{
		return cpps_to_string(v);
	}
	bool cpps_base_isstring(cpps_value v)
	{
		return cpps_isstring(v);
	}
	bool cpps_base_isvector(cpps_value v)
	{
		return cpps_isvector(v);
	}

	bool cpps_base_isrange(cpps_value v)
	{
		return cpps_isrange(v);
	}

	bool cpps_base_ismap(cpps_value v)
	{
		return cpps_ismap(v);
	}
	bool cpps_base_ispair(cpps_value v)
	{
		return cpps_ispair(v);
	}
	bool cpps_base_isset(cpps_value v)
	{
		return cpps_isset(v);
	}
	bool cpps_base_isint(cpps_value v)
	{
		return cpps_isint(v);
	}
	bool cpps_base_isuint(cpps_value v)
	{
		return cpps_isuint(v);
	}
	bool cpps_base_isbool(cpps_value v)
	{
		return cpps_isbool(v);
	}
	bool cpps_base_isnumber(cpps_value v)
	{
		return cpps_isnumber(v);
	}
	bool cpps_base_isnull(cpps_value v)
	{
		return cpps_isnull(v);
	}
	bool cpps_base_isclassvar(cpps_value v)
	{
		return cpps_isclassvar(v);
	}
	bool cpps_base_isclass(cpps_value v)
	{
		return cpps_isclass(v);
	}

	bool cpps_base_isellipsis(cpps_value v)
	{
		return cpps_isellipsis(v);
	}

	bool cpps_base_istuple(cpps_value v)
	{
		return v.tt == CPPS_TTUPLE;
	}
	bool cpps_base_iskindof(cpps_value _cls, cpps_value _var) {
		if (!cpps_base_isclass(_cls) || !cpps_base_isclassvar(_var)) return false;

		auto cls = cpps_to_cpps_cppsclass(_cls);
		auto clsvar = cpps_to_cpps_cppsclassvar(_var);

		return clsvar->getcppsclass() == cls;
	}

	bool cpps_base_isfunction(cpps_value v)
	{
		return cpps_isfunction(v);
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
		case CPPS_TUINTEGER:
			return "unsigned integer";
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
	
	std::string cpps_module_path(std::string libname) {
		std::string path = "lib/" + libname + "/";
		std::string fpath;
#ifdef _WIN32
		fpath = cpps_rebuild_filepath(path + (libname + ".dll"));
		if (!fpath.empty())
		{
			return cpps_io_getfilepath(fpath);
		}

#else

#if defined LINUX
		fpath = cpps_rebuild_filepath(path + "lib" + (libname + ".so"));
#elif defined __APPLE__
		fpath = cpps_rebuild_filepath(path + "lib" + (libname + ".dylib"));
#endif
		if (!fpath.empty())
		{
			return cpps_io_getfilepath(fpath);
		}

#endif
		fpath = cpps_rebuild_filepath(path + "main.cpp");
		if (!fpath.empty())
		{
			return cpps_io_getfilepath(fpath);
		}

		return "";
	}
	bool cpps_checklibrary(std::string libname) {

		std::string path = "lib/" + libname + "/";
		std::string fpath;
#ifdef _WIN32
		fpath = cpps_rebuild_filepath(path + (libname + ".dll"));
		if (!fpath.empty())
		{
			return true;
		}

#else

#if defined LINUX
		fpath = cpps_rebuild_filepath(path + "lib" + (libname + ".so"));
#elif defined __APPLE__
		fpath = cpps_rebuild_filepath(path + "lib" + (libname + ".dylib"));
#endif
		if (!fpath.empty())
		{
			return true;
		}

#endif
		fpath = cpps_rebuild_filepath(path + "main.cpp");
		if (!fpath.empty())
		{
			return true;
		}

		return false;
	}
	
	bool	cpps_loadlibrary(C *c,std::string libname, cppsbuffer& buffer)
	{
		std::string path = "lib/"+ libname + "/";
		std::string fpath;

		bool bexits = false;
		

		if (c->modulelist.find(libname) != c->modulelist.end()) return true;
		bool sv = false;
		std::string libfuncname = "cpps_attach";
#ifdef _WIN32

		fpath = cpps_rebuild_filepath(path + (libname + ".dll"));
		if (!fpath.empty())
		{
			HMODULE module = ::LoadLibraryA(fpath.c_str());
			if (module == NULL)
			{
				printf("Load module [%s] : %d faild.\r\n", libname.c_str(),GetLastError());
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
			bexits = true;

		}
#else
#if defined LINUX
		fpath = cpps_rebuild_filepath(path + "lib" + (libname + ".so"));
#elif defined __APPLE__
		fpath = cpps_rebuild_filepath(path + "lib" + (libname + ".dylib"));
#endif
		if (!fpath.empty())
		{
			HMODULE mod = dlopen(fpath.c_str(), RTLD_LAZY );
			if (mod == NULL)
			{
				printf("dlopen [%s]:%s faild\r\n", libname.c_str(), dlerror());
				return false;
			}
			dlerror();

			cpps_attach_func cpps_attach = NULL;
			*(void**)(&cpps_attach) = dlsym(mod, libfuncname.c_str());
			if (cpps_attach == NULL)
			{
				dlclose(mod);
				printf("dlsym [cpps_attach] faild\r\n");
				return false;
			}
			char *error = dlerror();
			if (error != NULL) {
				cpps::error(c, "%s", error);
				return false;
			}

		/*	CPPS_ST_API* api = (CPPS_ST_API*)dlsym(mod, "LIBAPI");
			if (api == NULL)
			{
				dlclose(mod);
				printf("dlsym [LIBAPI] faild\r\n");
				return false;
			}*/

			c->modulelist.insert(phmap::flat_hash_map<std::string, HMODULE>::value_type(libname, mod));
			sv = true;

			//api->cpps_attach(c);
			(*cpps_attach)(c);
			bexits = true;


		}
#endif
		fpath = cpps_rebuild_filepath(path + "main.cpp");
		if (!fpath.empty())
		{
			bexits = true;
			if(!sv)
				c->modulelist.insert(phmap::flat_hash_map<std::string, HMODULE>::value_type(libname, NULL));
			std::string fileSrc;
			cpps_load_filebuffer(fpath.c_str(), fileSrc);
			buffer.append(fpath, fileSrc.c_str(), (int32)fileSrc.size());
			/*node* o = loadbuffer(c,c->_G,  fileSrc, fpath);
			cpps_stack* stack = c->stack_alloc();
			stack->init("main.cpp", 0, "import");
			c->push_stack(stack);
			cpps_step_all(c, CPPS_MUNITRET, c->_G,c->_G, o);
			c->pop_stack();
			cpps_gc_check_step(c);
			c->stack_free(stack);
			if (o) { cpps_destory_node(o); CPPSDELETE(o); o = NULL; }*/
		}

		if (!bexits)
			throw cpps_error(c->curnode->filename, c->curnode->line, cpps_error_normalerror, "No Found %s module", libname.c_str());

		return true;
	}

	void cpps_detach_library(HMODULE module,const std::string& libname, C* c)
	{
		//printf("cpps_detach_library -> %s\r\n", libname.c_str());
		if (module == NULL) return;
		std::string libfuncname = "cpps_detach";
#ifdef _WIN32

		cpps_detach_func cpps_detach = (cpps_detach_func)GetProcAddress(module, libfuncname.c_str());
		if (cpps_detach == NULL)
		{
			printf("Free Module %s  faild.\r\n", libname.c_str());
		}
		else
		{
			cpps_detach(c);
			FreeLibrary(module);
		}
#else
		dlerror();
		cpps_detach_func cpps_detach = NULL;
		*(void **) (&cpps_detach) = dlsym(module, libfuncname.c_str());
		if (cpps_detach == NULL)
		{
			char* error = dlerror();
			if (error != NULL) {
				cpps::error(c, "%s", error);
				return;
			}
		}
		else
		{
			(* cpps_detach)(c);
#if defined LINUX
			dlclose(module);
#endif
		}

		/*CPPS_ST_API* api = (CPPS_ST_API*)dlsym(module, "LIBAPI");
		if (api == NULL)
		{
			printf("dlsym [LIBAPI] %s faild\r\n", libname.c_str());
		}
		else
		{
			api->cpps_detach(c);
#if defined LINUX
			dlclose(module);
#endif
		}*/
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
		cpps_value ret;
		newclass<cpps_vector>(c, &vct, &ret);
		for (int i = 0; i < c->application_argc; i++)
		{
			vct->push_back(cpps_value(c, c->application_argv[i]));
		}
		return ret;
	}
#if defined _WIN32
	std::string cpps_base_win_execmd(std::string pszCmd)
	{
		// ���������ܵ�
		SECURITY_ATTRIBUTES sa = { sizeof(SECURITY_ATTRIBUTES), NULL, TRUE };
		HANDLE hRead, hWrite;
		if (!CreatePipe(&hRead, &hWrite, &sa, 0))
		{
			return "";
		}

		// ���������н���������Ϣ(�����ط�ʽ���������λ�������hWrite
		STARTUPINFO si = { sizeof(STARTUPINFO) };
		GetStartupInfo(&si);
		si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
		si.wShowWindow = SW_HIDE;
		si.hStdError = hWrite;
		si.hStdOutput = hWrite;

		// ����������
		PROCESS_INFORMATION pi;
		if (!CreateProcessA(NULL, (LPSTR)pszCmd.c_str(), NULL, NULL, TRUE, NULL, NULL, NULL, &si, &pi))
		{
			return "";
		}

		// �����ر�hWrite
		CloseHandle(hWrite);

		// ��ȡ�����з���ֵ
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
	void cpps_base_dostring(C* c, std::string str) {
		c->disabled_non_def_var = true;
		node* o = loadbuffer(c, c->_G, str, "<ScriptIO>");
		if (o)
			cpps_step_all(c, CPPS_MUNITRET, 0, 0, o, false);
		if (o) { cpps_destory_node(o); CPPSDELETE(o); o = NULL; }
		
		c->disabled_non_def_var = false;
	}
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
	bool cpps_base_isprime(cpps_integer n)
	{
		for (cpps_integer i = 2; i < n - 1; i++) {
			if (n % i == 0)
				return false;
		}
		return true;
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
		if (v.tt != CPPS_TCLASSVAR) return v;
		cpps_integer serializer_type = 2;//1. serializer.vector 2.serializer.map
		if (cpps_isint(tp)) serializer_type = tp.value.integer;
		cpps_cppsclassvar* classvar = (cpps_cppsclassvar*)v.value.domain;


		cpps_value ret;
		if (serializer_type == 1 && classvar->stacklist) {
			cpps_vector* vct;
			newclass<cpps_vector>(c, &vct,&ret);
			for (auto var : *classvar->stacklist) {

				vct->push_back(var->getval());
			}
		}
		else if(serializer_type == 2){
			cpps_map* m;
			newclass<cpps_map>(c, &m,&ret);
			if(classvar->hasVar)
				for (auto var : *classvar->varList)
					m->insert(cpps_value(c,var.first),var.second->getval());
		}
		return ret;

	}
	cpps_value cpps_base_serializer_decode(C* c, cpps_value cls, cpps_value v)
	{
		cpps_value ret;

		if (cls.tt != CPPS_TCLASS) return v;
		cpps_cppsclass* cppsclass = (cpps_cppsclass*)cls.value.domain;

		if (cpps_base_isvector(v)) {
			cpps_vector* vct = cpps_to_cpps_vector(v);
			cpps_value v2;
			newcppsclasvar(c, cppsclass,&v2);
			if (v2.value.domain->stacklist && vct->realvector().size() == v2.value.domain->stacklist->size())
			{
				size_t size = (size_t)vct->size();
				
				for (size_t i = 0; i < size; i++) {
					cpps_regvar* regv = (*(v2.value.domain->stacklist))[i];
					regv->setval(vct->realvector()[i]);
				}
				ret = v2;
			}
		}
		else if(cpps_base_ismap(v)){
			cpps_map* m = cpps_to_cpps_map(v);
			cpps_value v2;
			newcppsclasvar(c, cppsclass,&v2);
			//if (m->realmap().size() == v2.value.domain->stacklist->size())
			{
				for (auto it : m->realmap()) {
					cpps_domain* leftdomain = NULL;
					cpps_regvar * regv = v2.value.domain->getvar(cpps_to_string(it.first), leftdomain, false);
					if (regv)
					{
						regv->setval(it.second);
					}
				}
				ret = v2;
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
		if (boundary > 0) quickSort(v, begin, boundary - 1);
		quickSort(v, boundary + 1, end);
	}
	void cpps_base_sort(object v)
	{
		if (v.isvector()) {
			cpps_vector* vec = cpps_to_cpps_vector(v.value);
			quickSort(vec->realvector(), 0,size_t( vec->size() - 1));
		}
	}
	void cpps_base_real_shuffle(cpps_std_vector& vec)
	{
		size_t n = vec.size();
		if (n <= 0)
			return;

		for (size_t i = 0; i < n; i++)
		{
			//��֤ÿ�ε�iλ��ֵ�����漰����iλ��ǰ
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
	void cpps_base_reverse(object v)
	{
		if (v.isvector()) {
			cpps_vector* vec = cpps_to_cpps_vector(v.value);
			reverse(vec->realvector().begin(),vec->realvector().end());
		}
	}
	void cpps_base_exit(cpps_integer exitcode)
	{
		exit((int)exitcode);
	}
	void cpps_setechofunc(C*c,object echofunc,object echoleft)
	{
		cpps::object::define(c, "__echofunc", echofunc);
		cpps::object::define(c, "__echoleft", echoleft);
	}
	cpps_value cpps_base_parse(C* c, cpps_value buffer) {
		
		if (!cpps_isstring(buffer)) return nil;

		std::string* str = cpps_get_string(buffer);
		bool _buildoffset = c->buildoffset;
		c->buildoffset = false;
		node* o = loadbuffer(c, c->_G, *str, c->curnode->filename);
		c->buildoffset = _buildoffset;
	

		node *copynode = NULL;
		cpps_value ret;
		newclass(c, &copynode, &ret);
		copynode->clone(o);
		cpps_destory_node(o); o->release(); o = NULL;

		return ret;
	}
	cpps_value cpps_base_compile(C* c, cpps_value src_path) {
		
		if (!cpps_isstring(src_path)) return nil;

		std::string* _src_path = cpps_get_string(src_path);
		std::string _src_content = cpps_io_readfile(*_src_path);

		std::string _old_work_path = cpps_getcwd();

#ifdef WIN32
		if (SetCurrentDirectoryA(cpps_io_getfilepath(*_src_path).c_str())) {}
#else
		if (chdir(cpps_io_getfilepath(*_src_path).c_str())) {}
#endif


		C* _c = cpps::create(c);

		_c->buildoffset = false;
		try {
			node* o = loadbuffer(_c, _c->_G, _src_content, _src_path->c_str(), CPPS_RUN_COMPILE);
#ifdef WIN32
			if (SetCurrentDirectoryA(_old_work_path.c_str())) {}
#else
			if (chdir(_old_work_path.c_str())) {}
#endif

			cpps_newclass(c,Buffer, _out_buffer,cpps_value, _out_buffer_value) ;

			o->write(_out_buffer);
			
			cpps_destory_node(o); o->release(); o = NULL;
			cpps::close(_c);

			return _out_buffer_value;

		}
		catch (...) {
			cpps::close(_c);
#ifdef WIN32
			if (SetCurrentDirectoryA(_old_work_path.c_str())) {}
#else
			if (chdir(_old_work_path.c_str())) {}
#endif
			throw;
		}
		return nil;
	}
	std::string base_BinToHex(std::string strBin)
	{
		std::string strHex;
		size_t len = strBin.size();
		strHex.resize(strBin.size() * 2);
		for (size_t i = 0; i < len; i++)
		{
			uint8_t cTemp = strBin[i];
			for (size_t j = 0; j < 2; j++)
			{
				uint8_t cCur = (cTemp & 0x0f);
				if (cCur < 10)
				{
					cCur += '0';
				}
				else
				{
					cCur += (('a') - 10);
				}
				strHex[2 * i + 1 - j] = cCur;
				cTemp >>= 4;
			}
		}

		return strHex;
	}
	std::string base_HexToBin(std::string strHex)
	{
		if (strHex.size() % 2 != 0)
		{
			return "";
		}

		std::string strBin;
		strBin.resize(strHex.size() / 2);
		for (size_t i = 0; i < strBin.size(); i++)
		{
			uint8_t cTemp = 0;
			for (size_t j = 0; j < 2; j++)
			{
				char cCur = strHex[2 * i + j];
				if (cCur >= '0' && cCur <= '9')
				{
					cTemp = uint8_t((cTemp << 4) + (cCur - '0'));
				}
				else if (cCur >= 'a' && cCur <= 'f')
				{
					cTemp = uint8_t((cTemp << 4) + (cCur - 'a' + 10));
				}
				else if (cCur >= 'A' && cCur <= 'F')
				{
					cTemp = uint8_t((cTemp << 4) + (cCur - 'A' + 10));
				}
				else
				{
					return "";
				}
			}
			strBin[i] = cTemp;
		}

		return strBin;
	}
	void cpps_regbase(C *c)
	{
		
		cpps::_module(c)[
			_class<C>("C_STATE"),
			_class<node>("cpps_node")
				.def("release",&node::cpps_release),
			def_inside("parse", cpps_base_parse),
			def_inside("__compile", cpps_base_compile),
			def("printf", cpps_base_printf_new),
			def("print", cpps_base_printf_new),
			def("printfln", cpps_base_printfln_new),
			def("println", cpps_base_printfln_new),
			//def_inside("endl", cpps_base_endl),
			def_inside("dump", cpps_base_dump),
			def("exit", cpps_base_exit),
			def("sleep", cpps_base_sleep),
			def("Sleep", cpps_base_sleep),
			def("usleep", cpps_base_sleep),
			def("hex2bin", base_HexToBin),
			def("bin2hex", base_BinToHex),
			def("tonumber", cpps_base_tonumber),
			def("double", cpps_base_tonumber),
			def("toint", cpps_base_tointeger),
			def("touint", cpps_base_touinteger),
			def("int", cpps_base_tointeger),
			def("uint", cpps_base_touinteger),
			def("bool", cpps_base_tobool),
			def("tostring", cpps_base_tostring),
			def("str", cpps_base_tostring),
			def("isstring", cpps_base_isstring),
			def("isint", cpps_base_isint),
			def("isuint", cpps_base_isuint),
			def("isbool", cpps_base_isbool),
			def("isvector", cpps_base_isvector),
			def("isrange", cpps_base_isrange),
			def("ismap", cpps_base_ismap),
			def("isnull", cpps_base_isnull),
			def("isnumber", cpps_base_isnumber),
			def("isfunction", cpps_base_isfunction),
			def("isclassvar", cpps_base_isclassvar),
			def("isclass", cpps_base_isclass),
			def("isellipsis", cpps_base_isellipsis),
			def("istuple", cpps_base_istuple),
			def("iskindof", cpps_base_iskindof),
			def("objtype", cpps_base_objtype),
			def("type", cpps_base_type),
			def("system", cpps_base_system),
			def("len", cpps_base_len),
			def("sort", cpps_base_sort),
			def("shuffle", cpps_base_shuffle),
			def("reverse", cpps_base_reverse),
			def("isvalid", cpps_base_isvalid),
			def("isdebug", cpps_base_isdebug),
			def("SetConsoleTitle", cpps_base_setconsoletitle),
			def("abort", abort),
			def_inside("setechofunc", cpps_setechofunc),
			defvar(c,"_VERSION", CPPS_VER),
			defvar(c,"_VERSIONNO", CPPS_VERN),
			def("check_module", cpps_checklibrary),
			def("module_path", cpps_module_path),
			//def_inside("loadlibrary", cpps_loadlibrary),
			//def_inside("freelibrary", cpps_freelibrary),
			def_inside("getargs", cpps_getargs),
			def_inside("execmd",cpps_base_execmd),
			def_inside("g_dostring", cpps_base_dostring),
			def("isprime",cpps_base_isprime)
		];
		cpps::_module(c, "ot")[
			defvar(c,"int", CPPS_TINTEGER),
			defvar(c,"uint", CPPS_TUINTEGER),
			defvar(c,"bool", CPPS_TBOOLEAN),
			defvar(c,"string", CPPS_TSTRING),
			defvar(c,"classvar", CPPS_TCLASSVAR),
			defvar(c,"function", CPPS_TFUNCTION),
			defvar(c,"domain", CPPS_TDOMAIN),
			defvar(c,"ptr", CPPS_TREF),
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
