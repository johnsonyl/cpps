#ifndef CPPS_MAKE_VECTOR_CPPS_HEAD_
#define CPPS_MAKE_VECTOR_CPPS_HEAD_

//===================================
//@Author		:	Johnson
//@QQ			:	88481106
//@Email		:	88481106@qq.com
//@Date			:	2015/11/20 (yy/mm/dd)
//@Module		:	CPPS_MAKE_VECTOR
//@Description	:	创建参数列表
//@website		:	http://cppscript.org
//==================================


template <class R>
vector1<R> make_vector(R(*)())
{
	return vector1<R>();
}
template <class C,class R>
vector1<R> make_vector(R(C::*)())
{
	return vector1<R>();
}

#define CPPS_MAKE_VECTOR_ITER_C 1
#include "cpps_make_vector.h"
#undef CPPS_MAKE_VECTOR_ITER_C

#define CPPS_MAKE_VECTOR_ITER_C 2
#include "cpps_make_vector.h"
#undef CPPS_MAKE_VECTOR_ITER_C

#define CPPS_MAKE_VECTOR_ITER_C 3
#include "cpps_make_vector.h"
#undef CPPS_MAKE_VECTOR_ITER_C

#define CPPS_MAKE_VECTOR_ITER_C 4
#include "cpps_make_vector.h"
#undef CPPS_MAKE_VECTOR_ITER_C

#define CPPS_MAKE_VECTOR_ITER_C 5
#include "cpps_make_vector.h"
#undef CPPS_MAKE_VECTOR_ITER_C


#define CPPS_MAKE_VECTOR_ITER_C 6
#include "cpps_make_vector.h"
#undef CPPS_MAKE_VECTOR_ITER_C

#define CPPS_MAKE_VECTOR_ITER_C 7
#include "cpps_make_vector.h"
#undef CPPS_MAKE_VECTOR_ITER_C

#define CPPS_MAKE_VECTOR_ITER_C 8
#include "cpps_make_vector.h"
#undef CPPS_MAKE_VECTOR_ITER_C

#define CPPS_MAKE_VECTOR_ITER_C 9
#include "cpps_make_vector.h"
#undef CPPS_MAKE_VECTOR_ITER_C

#define CPPS_MAKE_VECTOR_ITER_C 10
#include "cpps_make_vector.h"
#undef CPPS_MAKE_VECTOR_ITER_C

#define CPPS_MAKE_VECTOR_ITER_C 11
#include "cpps_make_vector.h"
#undef CPPS_MAKE_VECTOR_ITER_C



#endif // CPPS_MAKE_VECTOR_CPPS_HEAD_

#ifdef CPPS_MAKE_VECTOR_ITER_C

template<class R, CPPS_PP_ENUM_PARAMS(CPPS_MAKE_VECTOR_ITER_C, class A) >
CPPS_PP_CAT(cpps::vector, CPPS_PP_CAT(VECTOR_I_, CPPS_MAKE_VECTOR_ITER_C))<R, CPPS_PP_ENUM_PARAMS(CPPS_MAKE_VECTOR_ITER_C, A) >
make_vector(R(*)(CPPS_PP_ENUM_PARAMS(CPPS_MAKE_VECTOR_ITER_C, A)))
{
	return CPPS_PP_CAT(cpps::vector, CPPS_PP_CAT(VECTOR_I_, CPPS_MAKE_VECTOR_ITER_C))<R, CPPS_PP_ENUM_PARAMS(CPPS_MAKE_VECTOR_ITER_C, A)>();
}
template<class C, class R, CPPS_PP_ENUM_PARAMS(CPPS_MAKE_VECTOR_ITER_C, class A) >
CPPS_PP_CAT(cpps::vector, CPPS_PP_CAT(VECTOR_I_, CPPS_MAKE_VECTOR_ITER_C))<R, CPPS_PP_ENUM_PARAMS(CPPS_MAKE_VECTOR_ITER_C, A) >
make_vector(R(C::*)(CPPS_PP_ENUM_PARAMS(CPPS_MAKE_VECTOR_ITER_C, A)))
{
	return CPPS_PP_CAT(cpps::vector, CPPS_PP_CAT(VECTOR_I_, CPPS_MAKE_VECTOR_ITER_C))<R, CPPS_PP_ENUM_PARAMS(CPPS_MAKE_VECTOR_ITER_C, A)>();
}


#endif