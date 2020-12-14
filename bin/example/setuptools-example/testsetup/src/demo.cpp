
#include <cpps.h>


using namespace cpps;


void demotest()
{
	printf("hit demo function.\r\n");
}

cpps_export_void  cpps_attach(cpps::C * c)
{
	printf("attach demo\r\n");

	cpps::cpps_init_cpps_class(c);

	cpps::_module(c)[
		def("demotest", demotest)
	];
}
cpps_export_void  cpps_detach(cpps::C * c)
{
	printf("detach demo\r\n");
}

cpps_export_finish