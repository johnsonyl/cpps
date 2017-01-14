#ifndef CPPS_NEWPTR_HEAD_
#define CPPS_NEWPTR_HEAD_

//===================================
//@Author		:	Johnson
//@Email		:	jiang_4177@163.com
//@Date			:	2015/11/18 (yy/mm/dd)
//@Module		:	LIBCPPS
//@Description	:	Cpps处理接口
//@website		:	http://cpps.wiki
//==================================


namespace cpps
{
	//可以增加到GC的PTR
	template<class T>
	cpps_cppsclassvar*		newClassPtr(C *c, T ** ret)
	{
		cpps_cppsclassvar *var = cpps_class_singleton<T*>::getSingletonPtr()->getcls()->create(true);

		//将新创建出来的添加到新生区稍后检测要不要干掉
		cpps_gc_add_gen0(c, var);

		*ret = (T *)var->getclsptr();

		return var;
	}
}


#endif // CPPS_NEWPTR_HEAD_