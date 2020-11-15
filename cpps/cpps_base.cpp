#include "cpps.h"

using namespace std;

namespace cpps
{
	bool cpps_io_file_exists(std::string path);
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
		else if (type(b) == CPPS_TCLASSVAR)
		{
			if (b.value.value.domain->domainName == "vector")
			{
				cout << "[";
				cpps_vector *v = cpps_converter<cpps_vector*>::apply(b.value);
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
			else if (b.value.value.domain->domainName == "map")
			{
				cout << "{";
				cpps_map *v = cpps_converter<cpps_map*>::apply(b.value);
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
			else if (b.value.value.domain->domainName == "unordered_map")
			{
				cout << "{";
				cpps_unordered_map *v = cpps_converter<cpps_unordered_map*>::apply(b.value);
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
		else if (type(b) == CPPS_TCLASSVAR)
		{
			
			if (b.value.value.domain->domainName == "vector")
			{
				cout << "[";
				cpps_vector *v = cpps_converter<cpps_vector*>::apply(b.value);
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
			else if (b.value.value.domain->domainName == "map")
			{
				cout << "{";
				cpps_map *v = cpps_converter<cpps_map*>::apply(b.value);
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
			else if (b.value.value.domain->domainName == "unordered_map")
			{
				cout << "{";
				cpps_unordered_map *v = cpps_converter<cpps_unordered_map*>::apply(b.value);
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
	bool cpps_base_isstring(cpps_value v)
	{
		return v.tt == CPPS_TSTRING;
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
		return v.tt == CPPS_TFUNCTION;
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
						object func_object = func;
						func_object.value.parentLambdaVar = func.parentLambdaVar;
						object r = dofunction(c, func_object, v->it());

						if (func.parentLambdaVar->isbreak == true)
						{
							func.parentLambdaVar->isbreak = false;
							break;
						}


						if (type(r) == CPPS_TBOOLEAN)
						{
							bool ret = object_cast<bool>(r);
							if(!ret) v->pop();
							else v->next();
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
						object func_object = func;
						func_object.value.parentLambdaVar = func.parentLambdaVar;
						object r = dofunction(c, func_object, v->key(),v->it());

						if (func.parentLambdaVar->isbreak == true)
						{
							func.parentLambdaVar->isbreak = false;
							break;
						}

						if (type(r) == CPPS_TBOOLEAN)
						{
							bool ret = object_cast<bool>(r);
							if (!ret) v->pop();
							else v->next();
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
						object func_object = func;
						func_object.value.parentLambdaVar = func.parentLambdaVar;
						object r = dofunction(c, func_object, v->key(), v->it());


						if (func.parentLambdaVar->isbreak == true)
						{
							func.parentLambdaVar->isbreak = false;
							break;
						}

						if (type(r) == CPPS_TBOOLEAN)
						{
							bool ret = object_cast<bool>(r);
							if (!ret) v->pop();
							else v->next();
						}
						else
							v->next();
					}
				}
			}
		}
	}
	void cpps_assert(bool b)
	{
		assert(b);
	}
	bool cpps_decodecpps(std::string file, std::string outfile)
	{
		FILE *pfile = fopen(file.c_str(), "rb+");
		if (pfile)
		{
			long cur = ftell(pfile);
			fseek(pfile, 0, SEEK_END);
			cpps_integer size = ftell(pfile);
			fseek(pfile, cur, SEEK_SET);

			char *pBuffer = new char[size + 1];
			memset(pBuffer, 0, size + 1);
			fread(pBuffer, size, 1, pfile);
			fclose(pfile);
			std::string fileSrc = pBuffer;
			if (fileSrc.substr(0, 11) == "cpps_encode")
			{
				std::string decode;
				for (size_t i = 11; i < fileSrc.size(); i++)
				{
					decode.append(1, fileSrc[i] - CPPS_ENCODE_CPPS_KEY);
				}
				fileSrc = decode;
			}

			FILE *poutfile = fopen(outfile.c_str(), "wb+");
			if (poutfile)
			{
				fwrite(fileSrc.c_str(), fileSrc.size(), 1, poutfile);
				fclose(poutfile);
			}

			delete[] pBuffer;
			return true;
		}
		return false;

	}
	bool cpps_encodecpps(std::string file,std::string outfile)
	{
		FILE *pfile = fopen(file.c_str(), "rb+");
		if(pfile)
		{
			long cur = ftell(pfile);
			fseek(pfile, 0, SEEK_END);
			cpps_integer size = ftell(pfile);
			fseek(pfile, cur, SEEK_SET);

			char *pBuffer = new char[size + 1];
			fread(pBuffer, size, 1, pfile);
			fclose(pfile);

			std::string outbuffer = "cpps_encode";
			for (cpps_integer i = 0; i < size; i++)
			{
				outbuffer.append(1,pBuffer[i] + CPPS_ENCODE_CPPS_KEY);
			}
			FILE *poutfile = fopen(outfile.c_str(), "wb+");
			if (poutfile)
			{
				fwrite(outbuffer.c_str(), outbuffer.size(), 1, poutfile);
				fclose(poutfile);
			}

			delete[] pBuffer;
			return true;
		}
		return false;
	}
	struct ThreadParameter
	{
		C*c;
		cpps_value func;
		cpps_value value;
#ifdef WIN32
		HANDLE eventFinish;
#endif

	};
	void		gc_cleanup(C *c, int tid);
	static unsigned int threadFunction(void *p)
	{
		ThreadParameter*param = (ThreadParameter*)p;
		C*c = param->c;
		cpps_value func = param->func;
		cpps_value value = param->value;
		func.parentLambdaVar = NULL;
#ifdef WIN32
		SetEvent(param->eventFinish);
#endif
		cpps_try
		object ret = dofunction(c, func, value);
		cpps_catch
		gc_cleanup(c, GetCurrentThreadId());
		return 0;
	}
	cpps_integer cpps_createthread(C*c, cpps_value func,cpps_value value)
	{
#ifdef WIN32
		unsigned int threadID = 0;
		ThreadParameter param;
		param.c = c;
		param.func = func;
		param.value = value;
#ifdef WIN32
		param.eventFinish = CreateEvent(NULL, FALSE, FALSE, NULL);
		if (param.eventFinish == NULL) return 0;
#endif

		::_beginthreadex(NULL, 0, (_beginthreadex_proc_type) threadFunction, &param, 0, &threadID);
		
		//等待事件
		WaitForSingleObject(param.eventFinish, INFINITE);

		//关闭事件
		CloseHandle(param.eventFinish);


		return threadID;
#endif
	}
	std::string getfilenamenotext(std::string str);

	bool	cpps_loadlibrary(C *c,std::string libname)
	{
		std::string path = "lib/"+ libname + "/";
#ifdef WIN32
		HMODULE module = ::LoadLibraryA((path+(libname + ".dll")).c_str());
		std::string libfuncname = "cpps_attach";
		if (module == NULL)
		{
			printf("Load module【%s】 faild.\r\n", libname.c_str());
			FreeLibrary(module);
			return false;
		}

		cpps_attach_func cpps_attach = (cpps_attach_func)GetProcAddress(module, libfuncname.c_str());
		if (cpps_attach == NULL)
		{
			FreeLibrary(module);
			printf("Load module 【%s】 faild\r\n", libname.c_str());
			return false;
		}

		c->modulelist.insert(std::unordered_map<std::string, HMODULE>::value_type(libname, module));

		cpps_attach(c);

		
#elif LINUX
		HMODULE module = dlopen((path + (libname + ".so")).c_str(), RTLD_LAZY);
		if (module == NULL)
		{
			printf("dlopen [%s] faild\r\n", libname.c_str());
			dlclose(module);
			return false;
		}
		dlerror();
		CPPS_ST_API *api = dlsym(module, "LIBAPI");
		if(api == NULL)
		{
			dlclose(module);
			printf("dlsym [LIBAPI] faild\r\n");
			return false;
		}

		c->modulelist.insert(std::unordered_map<std::string, HMODULE>::value_type(libname, module));

		api->cpps_attach(c);


#endif

		std::string fpath = path + "main.cpp";
		bool b = cpps_io_file_exists(fpath);
		if (b)
		{

#ifdef _WIN32
			FILE* file;
			fopen_s(&file, fpath.c_str(), "rb+");
#else
			FILE* file = fopen(fpath.c_str(), "rb+");
#endif
			std::string fileSrc;
			if (file)
			{
				char s[4097];
				memset(s, 0, 4097);
				fseek(file, 0, SEEK_END);
				int32 size = ftell(file);
				fseek(file, 0, SEEK_SET);
				while (size != 0)
				{
					int32 rs = size < 4096 ? size : 4096;
					fread(s, rs, 1, file);
					s[rs] = '\0';
					size -= rs;
					fileSrc += s;
				}
				fclose(file);
			}
			if (fileSrc.substr(0, 11) == "cpps_encode")
			{
				std::string decode;
				for (size_t i = 11; i < fileSrc.size(); i++)
				{
					decode.append(1, fileSrc[i] - CPPS_ENCODE_CPPS_KEY);
				}
				fileSrc = decode;
			}
			Node* o = loadbuffer(c, c->_G, fileSrc, fpath);
			cpps_stack* stack = new cpps_stack("main.cpp", 0, "import");

			c->push_stack(stack);
			cpps_step_all(c, CPPS_SINGLERET, c->_G, o);
			c->pop_stack();


			cpps_gc_check_step(c);

			delete stack;
		}
		return true;
	}
	bool	cpps_freelibrary(C*c, std::string libname)
	{
#ifdef WIN32

		bool ret = false;
		std::unordered_map<std::string, HMODULE>::iterator it = c->modulelist.find(libname);
		if (it != c->modulelist.end())
		{
			HMODULE module = it->second;

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
			c->modulelist.erase(it);
			ret = true;
		}
		return ret;

#elif LINUX
		bool ret = false;
		std::unordered_map<std::string, HMODULE>::iterator it = c->modulelist.find(libname);
		if (it != c->modulelist.end())
		{
			HMODULE module = it->second;



			dlerror();
			CPPS_ST_API* api = dlsym(module, "LIBAPI");
			if (api == NULL)
			{
				printf("dlsym 【LIBAPI】 faild\r\n");
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
	void cpps_regbase(C *c)
	{
		module(c)[
			_class<std::string>("String")
		];
		module(c)[
			_class<C>("C_STATE"),
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
			def("isstring", cpps_base_isstring),
			def("isint", cpps_base_isint),
			def("isbool", cpps_base_isbool),
			def("isnull", cpps_base_isnull),
			def("isnumber", cpps_base_isnumber),
			def("isfunction", cpps_base_isfunction),
			def("objtype", cpps_base_objtype),
			def("system", cpps_base_system),
			def("SetConsoleTitle", cpps_base_setconsoletitle),
			def("assert", cpps_assert),
			defvar(c,"_VERSION", CPPS_VER),
			def("encodecpps", cpps_encodecpps),
			def("decodecpps", cpps_decodecpps),
			def_inside("createthread", cpps_createthread),
			def_inside("loadlibrary", cpps_loadlibrary),
			def_inside("freelibrary", cpps_freelibrary)
		];

	}
}
