#include <cpps.h>

using namespace cpps;

int main()
{
	cpps::C* c = cpps::create();

	cpps::dofile(c,"script/main.cpp");

	cpps::close(c);
	return 0;
}