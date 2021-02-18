#include <cpps/cpps.h>
#include "xml.hpp"
using namespace cpps;

cpps_export_void cpps_attach(C* c)
{
	cpps_init_cpps_class(c);
	_module(c, "xml")[
		_class<xml_attribute>("attribute")
			.def("constructor", &xml_attribute::create)
			.def("name", &xml_attribute::name)
			.def("value", &xml_attribute::value)
			.def("set_name", &xml_attribute::set_name)
			.def("set_value", &xml_attribute::set_value),
		_class<xml_node>("node")
			.def("constructor", &xml_node::create)
			.def("to_string", &xml_node::to_string)
			.def("type", &xml_node::type)
			.def("name", &xml_node::name)
			.def("value", &xml_node::value)
			.def_inside("first_node", &xml_node::first_node)
			.def_inside("last_node", &xml_node::last_node)
			.def_inside("previous_node", &xml_node::previous_node)
			.def_inside("next_node", &xml_node::next_node)
			.def_inside("first_attribute", &xml_node::first_attribute)
			.def_inside("last_attribute", &xml_node::last_attribute)
			.def_inside("previous_attribute", &xml_node::previous_attribute)
			.def_inside("next_attribute", &xml_node::next_attribute)
			.def_inside("children", &xml_node::children)
			.def_inside("attributes", &xml_node::attributes)
			.def("set_type", &xml_node::set_type)
			.def("set_name", &xml_node::set_name)
			.def("set_value", &xml_node::set_value)
			.def_inside("prepend_node", &xml_node::prepend_node)
			.def_inside("append_node", &xml_node::append_node)
			.def_inside("insert_node", &xml_node::insert_node)
			.def_inside("remove_first_node", &xml_node::remove_first_node)
			.def_inside("remove_last_node", &xml_node::remove_last_node)
			.def_inside("remove_node", &xml_node::remove_node)
			.def("remove_all_nodes", &xml_node::remove_all_nodes)
			.def_inside("prepend_attribute", &xml_node::prepend_attribute)
			.def_inside("append_attribute", &xml_node::append_attribute)
			.def_inside("insert_attribute", &xml_node::insert_attribute)
			.def_inside("remove_first_attribute", &xml_node::remove_first_attribute)
			.def_inside("remove_last_attribute", &xml_node::remove_last_attribute)
			.def_inside("remove_attribute", &xml_node::remove_attribute)
			.def("remove_all_attributes", &xml_node::remove_all_attributes),
		_class<xml_document>("document")
			.base<xml_node>()
			.def("parse", &xml_document::parse)
			.def("clear", &xml_document::clear),
		def_inside("xml_check", xml_check)
	];
}

cpps_export_void  cpps_detach(C * c)
{
}

cpps_export_finish
