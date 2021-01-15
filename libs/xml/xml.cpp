#include <cpps/cpps.h>
#include "xml.hpp"


cpps_export_void cpps_attach(cpps::C* c)
{
	cpps::cpps_init_cpps_class(c);
	cpps::_module(c, "xml")[
		cpps::_class<cpps::xml_document>("document")
			.def_inside("load",&cpps::xml_document::load)
            .def("save", &cpps::xml_document::save),
		cpps::_class<cpps::xml_node>("node")
            .def_inside("document", &cpps::xml_node::document)
			.def("name",&cpps::xml_node::name)
	];
}

cpps_export_void  cpps_detach(cpps::C * c)
{
}

cpps_export_finish
