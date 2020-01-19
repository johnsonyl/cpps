#ifndef CPPS_ENUM_PARAM_CPPS_HEAD_
#define CPPS_ENUM_PARAM_CPPS_HEAD_

//===================================
//@Author		:	Johnson
//@QQ			:	88481106
//@Email		:	jiang_4177@163.com
//@Date			:	2015/11/20 (yy/mm/dd)
//@Module		:	CPPS_ENUM_PARAM
//@Description	:	ºê²ÎÊýÆ´½Ó
//@website		:	http://cpps.wiki
//==================================


#ifdef _WIN32

#define CPPS_PP_ENUM_PARAMS(c,x) CPPS_PP_CAT(CPPS_PP_ENUM_PARAMS_,c)(x)

#define CPPS_PP_ENUM_PARAMS_0(d)
#define CPPS_PP_ENUM_PARAMS_1(d) d##1
#define CPPS_PP_ENUM_PARAMS_2(d) CPPS_PP_ENUM_PARAMS_1(d) ##,##d##2
#define CPPS_PP_ENUM_PARAMS_3(d) CPPS_PP_ENUM_PARAMS_2(d) ##,##d##3
#define CPPS_PP_ENUM_PARAMS_4(d) CPPS_PP_ENUM_PARAMS_3(d) ##,##d##4
#define CPPS_PP_ENUM_PARAMS_5(d) CPPS_PP_ENUM_PARAMS_4(d) ##,##d##5
#define CPPS_PP_ENUM_PARAMS_6(d) CPPS_PP_ENUM_PARAMS_5(d) ##,##d##6
#define CPPS_PP_ENUM_PARAMS_7(d) CPPS_PP_ENUM_PARAMS_6(d) ##,##d##7
#define CPPS_PP_ENUM_PARAMS_8(d) CPPS_PP_ENUM_PARAMS_7(d) ##,##d##8
#define CPPS_PP_ENUM_PARAMS_9(d) CPPS_PP_ENUM_PARAMS_8(d) ##,##d##9
#define CPPS_PP_ENUM_PARAMS_10(d) CPPS_PP_ENUM_PARAMS_9(d) ##,##d##10
#define CPPS_PP_ENUM_PARAMS_11(d) CPPS_PP_ENUM_PARAMS_10(d) ##,##d##11

#define CPPS_PP_ENUM_VARS(c,t,v) CPPS_PP_CAT(CPPS_PP_ENUM_VARS_,c)(t,v)


#define CPPS_PP_ENUM_VARS_0(t,v)
#define CPPS_PP_ENUM_VARS_1(t,v) t##1  v##1;
#define CPPS_PP_ENUM_VARS_2(t,v) CPPS_PP_ENUM_VARS_1(t,v) ##t##2  v##2;
#define CPPS_PP_ENUM_VARS_3(t,v) CPPS_PP_ENUM_VARS_2(t,v) ##t##3  v##3;
#define CPPS_PP_ENUM_VARS_4(t,v) CPPS_PP_ENUM_VARS_3(t,v) ##t##4  v##4;
#define CPPS_PP_ENUM_VARS_5(t,v) CPPS_PP_ENUM_VARS_4(t,v) ##t##5  v##5;
#define CPPS_PP_ENUM_VARS_6(t,v) CPPS_PP_ENUM_VARS_5(t,v) ##t##6  v##6;
#define CPPS_PP_ENUM_VARS_7(t,v) CPPS_PP_ENUM_VARS_6(t,v) ##t##7  v##7;
#define CPPS_PP_ENUM_VARS_8(t,v) CPPS_PP_ENUM_VARS_7(t,v) ##t##8  v##8;
#define CPPS_PP_ENUM_VARS_9(t,v) CPPS_PP_ENUM_VARS_8(t,v) ##t##9  v##9;
#define CPPS_PP_ENUM_VARS_10(t,v) CPPS_PP_ENUM_VARS_9(t,v) ##t##10  v##10;
#define CPPS_PP_ENUM_VARS_11(t,v) CPPS_PP_ENUM_VARS_10(t,v) ##t##11  v##11;


#define CPPS_PP_ENUM_VARS_PARAMS(c,t,v) CPPS_PP_CAT(CPPS_PP_ENUM_VARS_PARAMS_,c)(t,v)


#define CPPS_PP_ENUM_VARS_PARAMS_0(t,v)
#define CPPS_PP_ENUM_VARS_PARAMS_1(t,v) t##1  v##1
#define CPPS_PP_ENUM_VARS_PARAMS_2(t,v) CPPS_PP_ENUM_VARS_PARAMS_1(t,v) ,##t##2  v##2
#define CPPS_PP_ENUM_VARS_PARAMS_3(t,v) CPPS_PP_ENUM_VARS_PARAMS_2(t,v) ,##t##3  v##3
#define CPPS_PP_ENUM_VARS_PARAMS_4(t,v) CPPS_PP_ENUM_VARS_PARAMS_3(t,v) ,##t##4  v##4
#define CPPS_PP_ENUM_VARS_PARAMS_5(t,v) CPPS_PP_ENUM_VARS_PARAMS_4(t,v) ,##t##5  v##5
#define CPPS_PP_ENUM_VARS_PARAMS_6(t,v) CPPS_PP_ENUM_VARS_PARAMS_5(t,v) ,##t##6  v##6
#define CPPS_PP_ENUM_VARS_PARAMS_7(t,v) CPPS_PP_ENUM_VARS_PARAMS_6(t,v) ,##t##7  v##7
#define CPPS_PP_ENUM_VARS_PARAMS_8(t,v) CPPS_PP_ENUM_VARS_PARAMS_7(t,v) ,##t##8  v##8
#define CPPS_PP_ENUM_VARS_PARAMS_9(t,v) CPPS_PP_ENUM_VARS_PARAMS_8(t,v) ,##t##9  v##9
#define CPPS_PP_ENUM_VARS_PARAMS_10(t,v) CPPS_PP_ENUM_VARS_PARAMS_9(t,v) ,##t##10  v##10
#define CPPS_PP_ENUM_VARS_PARAMS_11(t,v) CPPS_PP_ENUM_VARS_PARAMS_10(t,v) ,##t##11  v##11

#define CPPS_PP_ENUM_VARS_PARAMS_PUSHBACK(c,l,t,v,d) CPPS_PP_CAT(CPPS_PP_ENUM_VARS_PARAMS_PUSHBACK_,c)(l,t,v,d)


#define CPPS_PP_ENUM_VARS_PARAMS_PUSHBACK_0(l,t,v,d)
#define CPPS_PP_ENUM_VARS_PARAMS_PUSHBACK_1(l,t,v,d) l.push_back(cpps_cpp_to_cpps_converter<t##1>::apply(d,v##1)); 
#define CPPS_PP_ENUM_VARS_PARAMS_PUSHBACK_2(l,t,v,d) CPPS_PP_ENUM_VARS_PARAMS_PUSHBACK_1(l,t,v,d) l.push_back(cpps_cpp_to_cpps_converter<t##2>::apply(d,v##2));
#define CPPS_PP_ENUM_VARS_PARAMS_PUSHBACK_3(l,t,v,d) CPPS_PP_ENUM_VARS_PARAMS_PUSHBACK_2(l,t,v,d) l.push_back(cpps_cpp_to_cpps_converter<t##3>::apply(d,v##3));
#define CPPS_PP_ENUM_VARS_PARAMS_PUSHBACK_4(l,t,v,d) CPPS_PP_ENUM_VARS_PARAMS_PUSHBACK_3(l,t,v,d) l.push_back(cpps_cpp_to_cpps_converter<t##4>::apply(d,v##4));
#define CPPS_PP_ENUM_VARS_PARAMS_PUSHBACK_5(l,t,v,d) CPPS_PP_ENUM_VARS_PARAMS_PUSHBACK_4(l,t,v,d) l.push_back(cpps_cpp_to_cpps_converter<t##5>::apply(d,v##5));
#define CPPS_PP_ENUM_VARS_PARAMS_PUSHBACK_6(l,t,v,d) CPPS_PP_ENUM_VARS_PARAMS_PUSHBACK_5(l,t,v,d) l.push_back(cpps_cpp_to_cpps_converter<t##6>::apply(d,v##6));
#define CPPS_PP_ENUM_VARS_PARAMS_PUSHBACK_7(l,t,v,d) CPPS_PP_ENUM_VARS_PARAMS_PUSHBACK_6(l,t,v,d) l.push_back(cpps_cpp_to_cpps_converter<t##7>::apply(d,v##7));
#define CPPS_PP_ENUM_VARS_PARAMS_PUSHBACK_8(l,t,v,d) CPPS_PP_ENUM_VARS_PARAMS_PUSHBACK_7(l,t,v,d) l.push_back(cpps_cpp_to_cpps_converter<t##8>::apply(d,v##8));
#define CPPS_PP_ENUM_VARS_PARAMS_PUSHBACK_9(l,t,v,d) CPPS_PP_ENUM_VARS_PARAMS_PUSHBACK_8(l,t,v,d) l.push_back(cpps_cpp_to_cpps_converter<t##9>::apply(d,v##9));
#define CPPS_PP_ENUM_VARS_PARAMS_PUSHBACK_10(l,t,v,d) CPPS_PP_ENUM_VARS_PARAMS_PUSHBACK_9(l,t,v,d) l.push_back(cpps_cpp_to_cpps_converter<t##10>::apply(d,v##10));
#define CPPS_PP_ENUM_VARS_PARAMS_PUSHBACK_11(l,t,v,d) CPPS_PP_ENUM_VARS_PARAMS_PUSHBACK_10(l,t,v,d) l.push_back(cpps_cpp_to_cpps_converter<t##11>::apply(d,v##11));


#define CPPS_PP_ENUM_SET_VARS_WITH_VECTOR(c,t,v) CPPS_PP_CAT(CPPS_PP_ENUM_SET_VARS_WITH_VECTOR_,c)(t,v)


#define CPPS_PP_ENUM_SET_VARS_WITH_VECTOR_0(t,v)
#define CPPS_PP_ENUM_SET_VARS_WITH_VECTOR_1(t,v) t##1 = cpps_converter<A1>::apply(v##[0]);
#define CPPS_PP_ENUM_SET_VARS_WITH_VECTOR_2(t,v) CPPS_PP_ENUM_SET_VARS_WITH_VECTOR_1(t,v) ##t##2 = cpps_converter<A2>::apply(v##[1]);
#define CPPS_PP_ENUM_SET_VARS_WITH_VECTOR_3(t,v) CPPS_PP_ENUM_SET_VARS_WITH_VECTOR_2(t,v) ##t##3 = cpps_converter<A3>::apply(v##[2]);
#define CPPS_PP_ENUM_SET_VARS_WITH_VECTOR_4(t,v) CPPS_PP_ENUM_SET_VARS_WITH_VECTOR_3(t,v) ##t##4 = cpps_converter<A4>::apply(v##[3]);
#define CPPS_PP_ENUM_SET_VARS_WITH_VECTOR_5(t,v) CPPS_PP_ENUM_SET_VARS_WITH_VECTOR_4(t,v) ##t##5 = cpps_converter<A5>::apply(v##[4]);
#define CPPS_PP_ENUM_SET_VARS_WITH_VECTOR_6(t,v) CPPS_PP_ENUM_SET_VARS_WITH_VECTOR_5(t,v) ##t##6 = cpps_converter<A6>::apply(v##[5]);
#define CPPS_PP_ENUM_SET_VARS_WITH_VECTOR_7(t,v) CPPS_PP_ENUM_SET_VARS_WITH_VECTOR_6(t,v) ##t##7 = cpps_converter<A7>::apply(v##[6]);
#define CPPS_PP_ENUM_SET_VARS_WITH_VECTOR_8(t,v) CPPS_PP_ENUM_SET_VARS_WITH_VECTOR_7(t,v) ##t##8 = cpps_converter<A8>::apply(v##[7]);
#define CPPS_PP_ENUM_SET_VARS_WITH_VECTOR_9(t,v) CPPS_PP_ENUM_SET_VARS_WITH_VECTOR_8(t,v) ##t##9 = cpps_converter<A9>::apply(v##[8]);
#define CPPS_PP_ENUM_SET_VARS_WITH_VECTOR_10(t,v) CPPS_PP_ENUM_SET_VARS_WITH_VECTOR_9(t,v) ##t##10 = cpps_converter<A10>::apply(v##[9]);
#define CPPS_PP_ENUM_SET_VARS_WITH_VECTOR_11(t,v) CPPS_PP_ENUM_SET_VARS_WITH_VECTOR_10(t,v) ##t##11 = cpps_converter<A11>::apply(v##[10]);

#else

#define CPPS_PP_ENUM_PARAMS(c,x) CPPS_PP_CAT(CPPS_PP_ENUM_PARAMS_,c)(x)


#define CPPS_PP_ENUM_PARAMS_0(d)
#define CPPS_PP_ENUM_PARAMS_1(d) d##1
#define CPPS_PP_ENUM_PARAMS_2(d) CPPS_PP_ENUM_PARAMS_1(d) , d##2
#define CPPS_PP_ENUM_PARAMS_3(d) CPPS_PP_ENUM_PARAMS_2(d) , d##3
#define CPPS_PP_ENUM_PARAMS_4(d) CPPS_PP_ENUM_PARAMS_3(d) , d##4
#define CPPS_PP_ENUM_PARAMS_5(d) CPPS_PP_ENUM_PARAMS_4(d) , d##5
#define CPPS_PP_ENUM_PARAMS_6(d) CPPS_PP_ENUM_PARAMS_5(d) , d##6
#define CPPS_PP_ENUM_PARAMS_7(d) CPPS_PP_ENUM_PARAMS_6(d) , d##7
#define CPPS_PP_ENUM_PARAMS_8(d) CPPS_PP_ENUM_PARAMS_7(d) , d##8
#define CPPS_PP_ENUM_PARAMS_9(d) CPPS_PP_ENUM_PARAMS_8(d) , d##9
#define CPPS_PP_ENUM_PARAMS_10(d) CPPS_PP_ENUM_PARAMS_9(d) , d##10
#define CPPS_PP_ENUM_PARAMS_11(d) CPPS_PP_ENUM_PARAMS_10(d) , d##11

#define CPPS_PP_ENUM_VARS(c,t,v) CPPS_PP_CAT(CPPS_PP_ENUM_VARS_,c)(t,v)


#define CPPS_PP_ENUM_VARS_0(t,v)
#define CPPS_PP_ENUM_VARS_1(t,v) t##1 v##1;
#define CPPS_PP_ENUM_VARS_2(t,v) CPPS_PP_ENUM_VARS_1(t,v) t##2  v##2;
#define CPPS_PP_ENUM_VARS_3(t,v) CPPS_PP_ENUM_VARS_2(t,v) t##3  v##3;
#define CPPS_PP_ENUM_VARS_4(t,v) CPPS_PP_ENUM_VARS_3(t,v) t##4  v##4;
#define CPPS_PP_ENUM_VARS_5(t,v) CPPS_PP_ENUM_VARS_4(t,v) t##5  v##5;
#define CPPS_PP_ENUM_VARS_6(t,v) CPPS_PP_ENUM_VARS_5(t,v) t##6  v##6;
#define CPPS_PP_ENUM_VARS_7(t,v) CPPS_PP_ENUM_VARS_6(t,v) t##7  v##7;
#define CPPS_PP_ENUM_VARS_8(t,v) CPPS_PP_ENUM_VARS_7(t,v) t##8  v##8;
#define CPPS_PP_ENUM_VARS_9(t,v) CPPS_PP_ENUM_VARS_8(t,v) t##9  v##9;
#define CPPS_PP_ENUM_VARS_10(t,v) CPPS_PP_ENUM_VARS_9(t,v) t##10  v##10;
#define CPPS_PP_ENUM_VARS_11(t,v) CPPS_PP_ENUM_VARS_10(t,v) t##11  v##11;



#define CPPS_PP_ENUM_VARS_PARAMS(c,t,v) CPPS_PP_CAT(CPPS_PP_ENUM_VARS_PARAMS_,c)(t,v)


#define CPPS_PP_ENUM_VARS_PARAMS_0(t,v)
#define CPPS_PP_ENUM_VARS_PARAMS_1(t,v) t##1  v##1
#define CPPS_PP_ENUM_VARS_PARAMS_2(t,v) CPPS_PP_ENUM_VARS_PARAMS_1(t,v) , t##2  v##2
#define CPPS_PP_ENUM_VARS_PARAMS_3(t,v) CPPS_PP_ENUM_VARS_PARAMS_2(t,v) , t##3  v##3
#define CPPS_PP_ENUM_VARS_PARAMS_4(t,v) CPPS_PP_ENUM_VARS_PARAMS_3(t,v) , t##4  v##4
#define CPPS_PP_ENUM_VARS_PARAMS_5(t,v) CPPS_PP_ENUM_VARS_PARAMS_4(t,v) , t##5  v##5
#define CPPS_PP_ENUM_VARS_PARAMS_6(t,v) CPPS_PP_ENUM_VARS_PARAMS_5(t,v) , t##6  v##6
#define CPPS_PP_ENUM_VARS_PARAMS_7(t,v) CPPS_PP_ENUM_VARS_PARAMS_6(t,v) , t##7  v##7
#define CPPS_PP_ENUM_VARS_PARAMS_8(t,v) CPPS_PP_ENUM_VARS_PARAMS_7(t,v) , t##8  v##8
#define CPPS_PP_ENUM_VARS_PARAMS_9(t,v) CPPS_PP_ENUM_VARS_PARAMS_8(t,v) , t##9  v##9
#define CPPS_PP_ENUM_VARS_PARAMS_10(t,v) CPPS_PP_ENUM_VARS_PARAMS_9(t,v) , t##10  v##10
#define CPPS_PP_ENUM_VARS_PARAMS_11(t,v) CPPS_PP_ENUM_VARS_PARAMS_10(t,v) , t##11  v##11


#define CPPS_PP_ENUM_VARS_PARAMS_PUSHBACK(c,l,t,v,d) CPPS_PP_CAT(CPPS_PP_ENUM_VARS_PARAMS_PUSHBACK_,c)(l,t,v,d)


#define CPPS_PP_ENUM_VARS_PARAMS_PUSHBACK_0(l,t,v,d)
#define CPPS_PP_ENUM_VARS_PARAMS_PUSHBACK_1(l,t,v,d) l.push_back(cpps_cpp_to_cpps_converter<t##1>::apply(d,v##1)); 
#define CPPS_PP_ENUM_VARS_PARAMS_PUSHBACK_2(l,t,v,d) CPPS_PP_ENUM_VARS_PARAMS_PUSHBACK_1(l,t,v,d) l.push_back(cpps_cpp_to_cpps_converter<t##2>::apply(d,v##2));
#define CPPS_PP_ENUM_VARS_PARAMS_PUSHBACK_3(l,t,v,d) CPPS_PP_ENUM_VARS_PARAMS_PUSHBACK_2(l,t,v,d) l.push_back(cpps_cpp_to_cpps_converter<t##3>::apply(d,v##3));
#define CPPS_PP_ENUM_VARS_PARAMS_PUSHBACK_4(l,t,v,d) CPPS_PP_ENUM_VARS_PARAMS_PUSHBACK_3(l,t,v,d) l.push_back(cpps_cpp_to_cpps_converter<t##4>::apply(d,v##4));
#define CPPS_PP_ENUM_VARS_PARAMS_PUSHBACK_5(l,t,v,d) CPPS_PP_ENUM_VARS_PARAMS_PUSHBACK_4(l,t,v,d) l.push_back(cpps_cpp_to_cpps_converter<t##5>::apply(d,v##5));
#define CPPS_PP_ENUM_VARS_PARAMS_PUSHBACK_6(l,t,v,d) CPPS_PP_ENUM_VARS_PARAMS_PUSHBACK_5(l,t,v,d) l.push_back(cpps_cpp_to_cpps_converter<t##6>::apply(d,v##6));
#define CPPS_PP_ENUM_VARS_PARAMS_PUSHBACK_7(l,t,v,d) CPPS_PP_ENUM_VARS_PARAMS_PUSHBACK_6(l,t,v,d) l.push_back(cpps_cpp_to_cpps_converter<t##7>::apply(d,v##7));
#define CPPS_PP_ENUM_VARS_PARAMS_PUSHBACK_8(l,t,v,d) CPPS_PP_ENUM_VARS_PARAMS_PUSHBACK_7(l,t,v,d) l.push_back(cpps_cpp_to_cpps_converter<t##8>::apply(d,v##8));
#define CPPS_PP_ENUM_VARS_PARAMS_PUSHBACK_9(l,t,v,d) CPPS_PP_ENUM_VARS_PARAMS_PUSHBACK_8(l,t,v,d) l.push_back(cpps_cpp_to_cpps_converter<t##9>::apply(d,v##9));
#define CPPS_PP_ENUM_VARS_PARAMS_PUSHBACK_10(l,t,v,d) CPPS_PP_ENUM_VARS_PARAMS_PUSHBACK_9(l,t,v,d) l.push_back(cpps_cpp_to_cpps_converter<t##10>::apply(d,v##10));
#define CPPS_PP_ENUM_VARS_PARAMS_PUSHBACK_11(l,t,v,d) CPPS_PP_ENUM_VARS_PARAMS_PUSHBACK_10(l,t,v,d) l.push_back(cpps_cpp_to_cpps_converter<t##11>::apply(d,v##11));



#define CPPS_PP_ENUM_SET_VARS_WITH_VECTOR(c,t,v) CPPS_PP_CAT(CPPS_PP_ENUM_SET_VARS_WITH_VECTOR_,c)(t,v)


#define CPPS_PP_ENUM_SET_VARS_WITH_VECTOR_0(t,v)
#define CPPS_PP_ENUM_SET_VARS_WITH_VECTOR_1(t,v) t##1 = cpps_converter<A1>::apply(v[0]);
#define CPPS_PP_ENUM_SET_VARS_WITH_VECTOR_2(t,v) CPPS_PP_ENUM_SET_VARS_WITH_VECTOR_1(t,v) t##2 = cpps_converter<A2>::apply(v[1]);
#define CPPS_PP_ENUM_SET_VARS_WITH_VECTOR_3(t,v) CPPS_PP_ENUM_SET_VARS_WITH_VECTOR_2(t,v) t##3 = cpps_converter<A3>::apply(v[2]);
#define CPPS_PP_ENUM_SET_VARS_WITH_VECTOR_4(t,v) CPPS_PP_ENUM_SET_VARS_WITH_VECTOR_3(t,v) t##4 = cpps_converter<A4>::apply(v[3]);
#define CPPS_PP_ENUM_SET_VARS_WITH_VECTOR_5(t,v) CPPS_PP_ENUM_SET_VARS_WITH_VECTOR_4(t,v) t##5 = cpps_converter<A5>::apply(v[4]);
#define CPPS_PP_ENUM_SET_VARS_WITH_VECTOR_6(t,v) CPPS_PP_ENUM_SET_VARS_WITH_VECTOR_5(t,v) t##6 = cpps_converter<A6>::apply(v[5]);
#define CPPS_PP_ENUM_SET_VARS_WITH_VECTOR_7(t,v) CPPS_PP_ENUM_SET_VARS_WITH_VECTOR_6(t,v) t##7 = cpps_converter<A7>::apply(v[6]);
#define CPPS_PP_ENUM_SET_VARS_WITH_VECTOR_8(t,v) CPPS_PP_ENUM_SET_VARS_WITH_VECTOR_7(t,v) t##8 = cpps_converter<A8>::apply(v[7]);
#define CPPS_PP_ENUM_SET_VARS_WITH_VECTOR_9(t,v) CPPS_PP_ENUM_SET_VARS_WITH_VECTOR_8(t,v) t##9 = cpps_converter<A9>::apply(v[8]);
#define CPPS_PP_ENUM_SET_VARS_WITH_VECTOR_10(t,v) CPPS_PP_ENUM_SET_VARS_WITH_VECTOR_9(t,v) t##10 = cpps_converter<A10>::apply(v[9]);
#define CPPS_PP_ENUM_SET_VARS_WITH_VECTOR_11(t,v) CPPS_PP_ENUM_SET_VARS_WITH_VECTOR_10(t,v) t##11 = cpps_converter<A10>::apply(v[10]);


#endif


#endif // CPPS_ENUM_PARAM_CPPS_HEAD_