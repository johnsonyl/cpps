#include <cpps/cpps.h>
#include "xml.hpp"
using namespace cpps;

cpps_export_void cpps_attach(C* c)
{
	cpps_init_cpps_class(c);
	_module(c, "xml")[
		def_inside("load", cpps_xml_load),
		_class<xml_document>("document")
			.def("name",&xml_node::name)
			.def_inside("children", &xml_node::children),
		_class<xml_node>("node")
			.def("name",&xml_node::name)
			.def_inside("children", &xml_node::children)
	];
}

cpps_export_void  cpps_detach(C * c)
{
}

cpps_export_finish
