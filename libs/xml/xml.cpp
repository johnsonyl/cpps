#include <cpps/cpps.h>
#include "xml.hpp"
using namespace cpps;

cpps_export_void cpps_attach(C* c)
{
	cpps_init_cpps_class(c);
	_module(c, "xml")[
		_class<xml_document>("document")
			.def("parse",&xml_document::parse)
			.def("clear",&xml_document::clear)
			.def_inside("children", &xml_document::children)
			.def_inside("children_by_name", &xml_document::children_by_name),
		_class<xml_node>("node")
			.def("type",&xml_node::type)
			.def("name",&xml_node::name)
			.def("value",&xml_node::value)
			.def_inside("children", &xml_node::children)
			.def_inside("children_by_name", &xml_node::children_by_name)
			.def_inside("attributes", &xml_node::attributes)
			.def_inside("attributes_by_name", &xml_node::attributes_by_name),
		_class<xml_attribute>("attribute")
			.def("name",&xml_attribute::name)
			.def("value",&xml_attribute::value)
	];
}

cpps_export_void  cpps_detach(C * c)
{
}

cpps_export_finish
