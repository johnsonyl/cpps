
#ifndef CPPS_OBJ_CPPS_HEAD_
#define CPPS_OBJ_CPPS_HEAD_

//===================================
//@Author		:	Johnson
//@QQ			:	88481106
//@Email		:	88481106@qq.com
//@Date			:	2015/11/20 (yy/mm/dd)
//@Module		:	CPPS_NODE
//@Description	:	Cpps�ڵ�
//@website		:	http://cppscript.org
//==================================


namespace cpps
{
	struct C;
	struct cpps_node_domain;
	typedef phmap::flat_hash_map<std::string, node*> var_list_type;
	struct cpps_value;
	enum node_var_type
	{
		node_var_type_var,
		node_var_type_constvar,
		node_var_type_asyncvar,
		node_var_type_externvar,
	};
	struct node
	{
		node();
		node(std::string f);
		node(node *n,std::string f,int32 ln);
		node(std::string f, int32 ln);
		~node();
		void release();
		void add(node *o);
		void clone(node * v);
		void setparent(node* p);
		void addtoleft(node *p);
		node* getleft();
		void addtoright(node *p);
		node* getright();
		node* getthird();
		void setdomain(cpps_node_domain* d);
		void regnode(std::string& s, node* n);
		void unregnode(std::string& s);
		node* getnode(std::string& s, bool b = false);
		void cpps_release();
		inline bool empty() { return l.empty(); }
		std::string s;
		std::vector<node*> l;
		int32 type;
		std::string filename;
		cpps_symbol* symbol;
		node *parent;
		cpps_node_domain* domain;
		int32 line;
		bool closure;
		bool quote;

		//����ת��
		union Value
		{
			cpps_number			number;		// double float 
			cpps_integer		integer;	// int
			cpps_uinteger		uinteger;	// int
			int32				b;			// bool
			cpps_value*			val;		// value
		};
		Value					value;		//ֵ��
		bool					needdelete;

		//���Ͳ㶯̬�����ڴ��ַ.=========
		int8 varsize;	//��������� (ֻ��������) ǰ������
		int16 offset;	//����ƫ��
		int8 offsettype; //ƫ������ 0 global 1 left 2 self
		int16 size;	//�ӽڵ����� ( ֻ������ ���ռ� ���� , ��������°�������) 65535���ڵ㻹������?

		var_list_type *varlist; //Ϊ����������,���������ٶ�.
	};
}
#endif // CPPS_OBJ_CPPS_HEAD_
