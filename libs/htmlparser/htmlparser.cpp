

#include <cpps/cpps.h>
#include <iostream>
#include <string>
#include <fstream>

#include "cpps_htmlparser_element.h"
#include "cpps_htmlparser_parser.h"


using namespace cpps;
using namespace std;


cpps_export_void cpps_attach(cpps::C* c)
{

	cpps::cpps_init_cpps_class(c);

	cpps::_module(c, "htmlparser")[
		_class<cpps_htmlparser_parser>("parser")
			.def_inside("parse",&cpps_htmlparser_parser::parse),
		_class<cpps_htmlparser_element>("element")
			.def("attr",&cpps_htmlparser_element::attr)
			.def_inside("children",&cpps_htmlparser_element::children)
			.def_inside("childrens_by_classname",&cpps_htmlparser_element::childrens_by_classname)
			.def_inside("childrens_by_tagname",&cpps_htmlparser_element::childrens_by_tagname)
			.def_inside("childrens",&cpps_htmlparser_element::childrens)
			.def_inside("select",&cpps_htmlparser_element::select)
			.def_inside("parent",&cpps_htmlparser_element::parent)
			.def("value",&cpps_htmlparser_element::value)
			.def("name",&cpps_htmlparser_element::name)
			.def("text",&cpps_htmlparser_element::text)
			.def("html",&cpps_htmlparser_element::html)
	];



   
}
cpps_export_void  cpps_detach(cpps::C * c)
{
}

cpps_export_finish