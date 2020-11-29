

#ifndef CPPS_CSTATE_CPPS_HEAD_
#define CPPS_CSTATE_CPPS_HEAD_

//===================================
//@Author		:	Johnson
//@QQ			:	88481106
//@Email		:	jiang_4177@163.com
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
		cpps_stack(){
			memset(f, 0, 64);
			memset(func, 0, 64);
			l = 0;
		}
		void	init(const char* pf, int32 pl, const char *pfunc)
		{
			strcpy(f, pf);
			strcpy(func, pfunc);
			l = pl;
		}

		std::vector<cpps_value> *createparams();
		cpps_value *create_cpps_value();
		char		 f[64];
		char		 func[64];
		int32			l;
	};

	struct C
	{
		C();
		std::unordered_set<cpps_cppsclassvar *>*									getgen0();
		std::unordered_set<cpps_cppsclassvar *>*									getgen1();
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
		std::unordered_set<cpps_regvar*>*											getbarrierlist();
		cpps::cpps_domain*															domain_alloc();
		void																		domain_free(cpps::cpps_domain* domain);
		cpps::cpps_stack*															stack_alloc();
		void																		stack_free(cpps::cpps_stack* stack);
		node* o;
		cpps_domain*																_G;	//根节点
		std::vector<cpps_stack*>													_callstack; //堆栈
		std::unordered_set<cpps_regvar*>											barrierList; //分界List
		std::unordered_set<cpps_cppsclassvar *>										gen0; //分界List
		std::unordered_set<cpps_cppsclassvar *>										gen1; //分界List
		size_t																		gen0size;//新生代当前占内存大小（字节）
		size_t																		gen1size;//老生代当前占内存大小（字节）
		size_t																		lastgensize;
		std::string																	(*func)(std::string &);
		std::unordered_map<std::string, HMODULE>									modulelist;
		std::unordered_map<void*, cpps_cppsclassvar*>								_class_map_classvar;
		bool																		debug;
		cpps_object_pool<cpps::cpps_domain>											domain_pool;
		cpps_object_pool<cpps::cpps_stack>											stack_pool;
	};
}
#endif // CPPS_CSTATE_CPPS_HEAD_