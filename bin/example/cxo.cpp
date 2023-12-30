#import "cpps_compile"

var b = cpps_compile.compile("cxo_test.cpp","cxo_test.cxo");

println(b);

/*

#include <cxo_test.cxo>

var a = new A();

println(g);

println(g_json);

var c = a.add(1,2)

println(c);
*/