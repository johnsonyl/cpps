#ifndef LIBCPPS_CPPS_HEAD_
#define LIBCPPS_CPPS_HEAD_

//===================================
//@Author		:	Johnson
//@QQ			:	88481106
//@Email		:	88481106@qq.com
//@Date			:	2015/11/18 (yy/mm/dd)
//@Module		:	LIBCPPS
//@Description	:	Cpps处理接口
//@website		:	http://cpps.wiki
//==================================


namespace cpps
{
	struct					object;
	C*						create(int argc = 0, char** argv = NULL);
	node* 					loadbuffer(C *c, std::string &str, std::string filename);
	int32					dostring(C *c, std::string str);
	int32					loadfile(C *c, const char* path);
	int32					dofile(C *c, const char* path);
	void					setchartrans(C *c, std::string(*func)(std::string &));
	object					_G(C *c);
	void					collect(C* c);//清理新生代			
	void					collectall(C*c);//清理老生代
	int32					close(C *&c);
	int32					pcall(C * c, int32 retType, cpps_domain* domain, node *o);
	int32					cpps_str2d(const char *s, cpps_number *result);
	int32					cpps_str2i64(const char *s, cpps_integer *result);
	void					cpps_pop_stack_to_here(C* c, cpps_stack* here, bool cleanup = true);
}


#endif // LIBCPPS_CPPS_HEAD_