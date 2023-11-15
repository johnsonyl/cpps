

#ifndef CPPS_CSTATE_CPPS_HEAD_
#define CPPS_CSTATE_CPPS_HEAD_

//===================================
//@Author		:	Johnson
//@QQ			:	88481106
//@Email		:	88481106@qq.com
//@Date			:	2015/11/20 (yy/mm/dd)
//@Module		:	CPPS_CSTATE
//@Description	:	CppsState
//@website		:	http://cppscript.org
//==================================
namespace cpps
{
	struct cpps_regfunction;
	struct cpps_regvar;
	struct cpps_reg;
	struct node;
	struct cpps_domain;
	struct cpps_value;
	struct cpps_cppsclassvar;
	struct cpps_symbol;
#ifdef _DEBUG
	typedef	std::map<std::string, cpps_symbol*> CPPS_SYMBOL_MAP;
#else
	typedef	phmap::flat_hash_map<std::string, cpps_symbol*> CPPS_SYMBOL_MAP;
#endif
	struct cpps_stack
	{
		cpps_stack();
		void						init(const char* pf, int32 pl, const char *pfunc);
		cpps_std_vector *			createparams();
		cpps_value *				create_cpps_value();
		char		 f[4096];
		char		 func[128];
		int32			l;
	};
	namespace coroutine {struct Ordinator;}
#ifdef _DEBUG
	typedef std::set<cpps_cppsclassvar*> CLASSVARSET;
#else
	typedef phmap::flat_hash_set<cpps_cppsclassvar*> CLASSVARSET;
#endif
	struct cpps_module_data {}; //interface 
	struct C
	{
		C():C(0,NULL){};
		C(int argc, char** argv);
		virtual ~C();
		CLASSVARSET*																getgen0();
		CLASSVARSET*																getgen1();
		void																		setgen0size(size_t s);
		size_t																		getgen0size();
		void																		setgen1size(size_t s);
		size_t																		getgen1size();
		void																		setlastgensize(size_t s);
		size_t																		getlastgensize();
		void																		push(node *_o);
		void																		push_stack(cpps_stack* stack);
		void																		pop_stack();
		void																		resume();						 //��Ҫ���� �ڽӵ�catch��ʱ��Ҫ����resume() ���������ڴ��쳣����ʱ���������취�������
		void																		debugopen();
		void																		debugclose();
		std::vector<cpps_stack*>*													getcallstack();
		void																		setcallstack(std::vector<cpps_stack*>* ret);
		phmap::flat_hash_set<cpps_regvar*>*											getbarrierlist();
		cpps::cpps_domain*															domain_alloc();
		void																		domain_free(cpps::cpps_domain* domain);
		cpps::cpps_stack*															stack_alloc();
		void																		stack_free(cpps::cpps_stack* stack);
		void																		setmoduledata(std::string modulename, cpps_module_data* data);
		cpps_module_data*															getmoduledata(std::string modulename);
		node* o;
		cpps_domain*																_G;	//���ڵ�
		std::vector<cpps_stack*>													*_callstack; //��ջ
		phmap::flat_hash_set<cpps_regvar*>											barrierList; //�ֽ�List
		CLASSVARSET									gen0; //�ֽ�List
		CLASSVARSET									gen1; //�ֽ�List
		size_t																		gen0size;//��������ǰռ�ڴ��С���ֽڣ�
		size_t																		gen1size;//��������ǰռ�ڴ��С���ֽڣ�
		size_t																		lastgensize;
		std::string																	(*func)(std::string &);
		phmap::flat_hash_map<std::string, HMODULE>									modulelist;
#ifdef _DEBUG
		std::map<void*, cpps_cppsclassvar*>											_class_map_classvar;
#else
		phmap::flat_hash_map<void*, cpps_cppsclassvar*>								_class_map_classvar;
#endif
		bool																		debug;
		cpps_object_pool<cpps::cpps_domain>											domain_pool;
		cpps_object_pool<cpps::cpps_stack>											stack_pool;
		int																			application_argc;
		char**																		application_argv;
		node*																		curnode;
		bool																		buildoffset;
		bool																		disabled_non_def_var;
		phmap::flat_hash_map<std::string, cpps_module_data*>						savemoduledatas;
		coroutine::Ordinator*														ordinator;
		bool																		isterminate;
		bool																		isloadbase;
		memory_allocal_handler*														memory_handler;

		CPPS_SYMBOL_MAP																symbolmap;						//��������
		CPPS_SYMBOL_MAP																leftsymbolmap;					//���������
		bool																		symbolleftasso[MAXSYMBOLPRIO];	//�������Ƿ�����

		void clone(C* clone_c);
	};
}
#endif // CPPS_CSTATE_CPPS_HEAD_