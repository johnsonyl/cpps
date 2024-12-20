#include <iostream>
#include <cpps/cpps.h>

using namespace cpps;
using namespace std;

namespace cpps { std::string cpps_io_getfilepath(std::string str);std::string cpps_rebuild_filepath(std::string path);std::string cpps_real_path(); }

class CppParentClassTest
{
public:
	int i;
	void	parentTestFunc()
	{
		printf("call CppParentClassTest::parentTestFunc() : %d\n",i);
	}
};

class CppClassTest : public CppParentClassTest
{
public:
	object	a;
	object	b;
	void	testFunc(cpps::object val)
	{
		i = 100;
		if (val.isint())
		{
			printf("val:%d\r\n", object_cast<int32>(val));
		}
		printf("%s:%s\r\n", "CppClassTest::testFunc",type_s(val).c_str());
	}
	object test_operator(cpps::object right)
	{
		return a.ref();//返回它的引用.,
	}
	void testcb(C*c,object cb) {
		b = cb;

		dofunction(c, b);
	}
};


void globalfunc(CppClassTest *pself)
{
	printf("globalfunc %lld\r\n",pself->a.toint());
}

enum TESTENUM
{
	ENUM_ONE = 1,
	ENUM_TWO,
};
int32 main(int argc,char **argv)
{
	std::string path = "main.cpp";

	if (argc >= 2) {
		path = argv[1];
	}



	path = cpps_rebuild_filepath(path);
#ifdef WIN32
	SetCurrentDirectoryA(cpps_io_getfilepath(path).c_str());
#else
	if (chdir(cpps_io_getfilepath(path).c_str())) {}
#endif


	C* c = cpps::create(argc,argv);

	cpps::_module(c)[
		_class<CppClassTest>("CppClassTest")
			.base<CppParentClassTest>()
			.def_operator("[]",&CppClassTest::test_operator)
			.def("testFunc",&CppClassTest::testFunc)
			.def("globalfunc", globalfunc)
			.def_inside("testcb",&CppClassTest::testcb)
			.def_classvar("a",&CppClassTest::a)
			.def_classvar("b",&CppClassTest::b),
		_class<CppParentClassTest>("CppParentClassTest")
			.def("parentTestFunc", &CppParentClassTest::parentTestFunc),
		_enum(c,"TESTENUM")
			.value("ENUM_ONE", TESTENUM::ENUM_ONE)
			.value("ENUM_TWO", TESTENUM::ENUM_TWO)
	];

	_CPPS_TRY
	cpps::dofile(c, path.c_str());
	_CPPS_CATCH;

	/*
	// script page

	class Agent
	{
		var val;
		var test()
		{
			println(val);
		}
	}

	var a = [1,2,3,4,1000];

	var b = { a:1 , b:2 ,c:3};
	
	*/
	{
		//loop vector
		cpps::object a = cpps::object::globals(c)["a"];
		for (cpps::object v : cpps::object::vector(a))
		{
			cpps_integer val = v.toint();
			cout << val << endl;
		}
		//print vector a value
		cpps::object vv = a[4];
		cout << "a[4] = " << object_cast<cpps_integer>(vv) << endl;

		cpps::object::vector vec(a);
		vec[4] = cpps::object::create(c, 3000);
		cout << "a[4] = " << object_cast<cpps_integer>(vec[4]) << endl;


		//////////////////////////////////////////////////////////////////////////

		//loop map
		cpps::object b = cpps::object::globals(c)["b"];
		for (auto v : cpps::object::map(c, b))
		{
			std::string key = object_cast<std::string>(v.first); // object_cast or std::string key = object(v.first).tostring();
			cpps_integer val = cpps::object(v.second).toint();

			cout << key << ":" << val << endl;
		}
		//print map a value.
		cpps::object bb = b["b"];
		cout << "b['b'] = " << object_cast<cpps_integer>(bb) << endl;



		cpps::object key = cpps::object::string(c, "z");



		cpps::object::map bmap(c, b);
		bmap["z00"] = cpps::object::string(c, "hello world");
		cout << "bmap-> It's Work bmap['z00'] = " << object_cast<std::string>(bmap["z00"]).c_str() << endl;

		bmap[1] = cpps::object::string(c, "interge key it's work too");
		cout << "bmap-> It's Work bmap[1] = " << object_cast<std::string>(bmap[1]).c_str() << endl;

		//////////////////////////////////////////////////////////////////////////

		//defined a global var;
		cpps::object::define(c, "x", cpps::object::string(c, "hello world"));

		cout << "x = " << object_cast<std::string>(cpps::object::globals(c)["x"]).c_str() << endl;
		cpps::object::globals(c)["x"] = cpps::object::string(c, "wonderful world");
		cout << "x = " << object_cast<std::string>(cpps::object::globals(c)["x"]).c_str() << endl;

		_G(c)["x"] = cpps::object::string(c, "_G get var it's work.");// work too
		cout << "x = " << object_cast<std::string>(_G(c)["x"]).c_str() << endl;

		//////////////////////////////////////////////////////////////////////////

		//new script class var ,and set dofunction.
		cpps::object Agent = cpps::object::globals(c)["Agent"];
		if (Agent.isclass())
		{
			cpps::object Agentvar = cpps::object::create_with_cppsclassvar(c, Agent);
			Agentvar["val"] = cpps::object::string(c, "this is string.");
			cpps::object testfunc = Agentvar["test"];
			if (testfunc.isfunction())
			{
				cpps::doclassfunction(c, Agentvar, testfunc);
			}
			Agentvar["val"] = cpps::object::string(c, "change string value.");
			if (testfunc.isfunction())
			{
				cpps::doclassfunction(c, Agentvar, testfunc);
			}
		}

		//////////////////////////////////////////////////////////////////////////

		//new cpp class var
		CppClassTest* ptr = NULL; //You don't need to delete it. it has GC to delete it.
		cpps::object cppclassvar = cpps::object::create_with_classvar< CppClassTest >(c, &ptr);
		//ptr->testFunc(xxx); //you can do something in cpp .

		cpps::object testFunc = cppclassvar["testFunc"];
		if (testFunc.isfunction()) {
			cpps::doclassfunction(c, cppclassvar, testFunc); //or get function and call it.
		}

		if (testFunc.isfunction()) {
			cpps::doclassfunction(c, cppclassvar, testFunc,  123456); //or get function and call it.
		}

		if (testFunc.isfunction()) {
			cpps::doclassfunction(c, cppclassvar, testFunc, cpps::object::string(c, "asdfsadf")); //or get function and call it.
		}

		cpps::object parentTestFunc = cppclassvar["parentTestFunc"];
		if (parentTestFunc.isfunction()) {
			cpps::doclassfunction(c, cppclassvar, parentTestFunc); //or get function and call it.
		}
		cpps::object _a = cppclassvar["a"];
		_a = 10;
		cpps::println(c,_a);
		cpps::println(c,ptr->a);



	}
	cpps::close(c);

	return 0;
}