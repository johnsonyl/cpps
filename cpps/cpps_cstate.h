

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
	struct Node;
	struct cpps_domain;
	struct cpps_value;
	struct cpps_cppsclassvar;
	struct cpps_stack
	{
		cpps_stack(std::string f, int32 l,std::string func)
		:f(f), l(l), func(func){}

		std::vector<cpps_value> *createparams();
		cpps_value *create_cpps_value();
		std::string f;
		std::string func;
		int32			l;
	};

	struct C
	{
		std::unordered_set<cpps_cppsclassvar *>* getGen0(int tid = 0);
		std::unordered_set<cpps_cppsclassvar *>* getGen1(int tid = 0);
		void setGen0size(size_t s, int tid = 0);
		size_t getGen0size(int tid = 0);
		void setGen1size(size_t s, int tid = 0);
		size_t getGen1size(int tid = 0);
		void setLastgensize(size_t s, int tid = 0);
		size_t getLastgensize(int tid = 0);
		C();
		void																		push(Node *_o);
		void																		push_stack(cpps_stack* stack);
		void																		pop_stack();
		void																		resume();						 //重要！！ 在接到catch的时候要调用resume() 否则会造成内存异常，暂时设置其他办法解决问题
		void																		debugopen();
		void																		debugclose();
		std::vector<cpps_stack*>*													getcallstack();

		std::unordered_set<cpps_regvar*>* getBarrierList(int tid = GetCurrentThreadId());
		Node* o;
		cpps_domain*						_G;	//根节点
		std::unordered_map<int,std::vector<cpps_stack*>*>							_callstack; //堆栈
		std::unordered_map<int, std::unordered_set<cpps_regvar*>*>					barrierList; //分界List
		std::unordered_map<int, std::unordered_set<cpps_cppsclassvar *>*>			gen0; //分界List
		std::unordered_map<int, std::unordered_set<cpps_cppsclassvar *>*>			gen1; //分界List
		std::unordered_map<int, size_t>												gen0size;//新生代当前占内存大小（字节）
		std::unordered_map<int, size_t>												gen1size;//老生代当前占内存大小（字节）
		std::unordered_map<int, size_t>												lastgensize;
		std::string																	(*func)(std::string &);
		std::unordered_map<std::string, HMODULE>									modulelist;
		bool																		debug;
		cpps_lock																	stacklock;
		//cpps_lock							gclock;
	};
}
#endif // CPPS_CSTATE_CPPS_HEAD_