

#ifndef CPPS_CSTATE_CPPS_HEAD_
#define CPPS_CSTATE_CPPS_HEAD_

//===================================
//@Author		:	Johnson
//@QQ			:	88481106
//@Email		:	88481106@qq.com
//@Date			:	2015/11/20 (yy/mm/dd)
//@Module		:	CPPS_CSTATE
//@Description	:	CppsState
//@website		:	http://cpps.wiki
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
	struct cpps_stack
	{
		cpps_stack();
		void						init(const char* pf, int32 pl, const char *pfunc);
		cpps_std_vector *			createparams();
		cpps_value *				create_cpps_value();
		char		 f[64];
		char		 func[64];
		int32			l;
	};
	struct cpps_module_data {}; //interface 
	struct C
	{
		C():C(0,NULL){};
		C(int argc, char** argv);
		virtual ~C();
		phmap::flat_hash_set<cpps_cppsclassvar *>*									getgen0();
		phmap::flat_hash_set<cpps_cppsclassvar *>*									getgen1();
		void																		setgen0size(size_t s);
		size_t																		getgen0size();
		void																		setgen1size(size_t s);
		size_t																		getgen1size();
		void																		setlastgensize(size_t s);
		size_t																		getlastgensize();
		void																		push(node *_o);
		void																		push_stack(cpps_stack* stack);
		void																		pop_stack();
		void																		resume();						 //重要！！ 在接到catch的时候要调用resume() 否则会造成内存异常，暂时设置其他办法解决问题
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
		cpps_domain*																_G;	//根节点
		std::vector<cpps_stack*>													*_callstack; //堆栈
		phmap::flat_hash_set<cpps_regvar*>											barrierList; //分界List
		phmap::flat_hash_set<cpps_cppsclassvar *>									gen0; //分界List
		phmap::flat_hash_set<cpps_cppsclassvar *>									gen1; //分界List
		size_t																		gen0size;//新生代当前占内存大小（字节）
		size_t																		gen1size;//老生代当前占内存大小（字节）
		size_t																		lastgensize;
		std::string																	(*func)(std::string &);
		phmap::flat_hash_map<std::string, HMODULE>									modulelist;
		phmap::flat_hash_map<void*, cpps_cppsclassvar*>								_class_map_classvar;
		bool																		debug;
		cpps_object_pool<cpps::cpps_domain>											domain_pool;
		cpps_object_pool<cpps::cpps_stack>											stack_pool;
		int																			application_argc;
		char**																		application_argv;
		node*																		curnode;
		bool																		buildoffset;
		bool																		disabled_non_def_var;
		phmap::flat_hash_map<std::string, cpps_module_data*>						savemoduledatas;
	};
}
#endif // CPPS_CSTATE_CPPS_HEAD_