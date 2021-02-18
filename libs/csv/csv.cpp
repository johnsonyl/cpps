
#include <cpps/cpps.h>
#include <iostream>
#include <string>
#include <fstream>

#include "csv.h"


using namespace cpps;
using namespace std;

object cpps_csv_open(C*c,std::string csvpath)
{
	cpps_csvreader* reader = NULL;
	object ret = object::create_with_classvar(c, &reader);
	reader->open(csvpath);
	return ret;
}

cpps_export_void  cpps_attach(cpps::C* c)
{

	cpps::cpps_init_cpps_class(c);

    cpps::_module(c, "csv")[
		_class<cpps_csvreader>("csv_reader")
		.def("open", &cpps_csvreader::open)
		.def_inside("read_header", &cpps_csvreader::read_header)
		.def("has_column", &cpps_csvreader::has_column)
		.def_inside("write_header", &cpps_csvreader::write_header)
		.def("get_file_line", &cpps_csvreader::get_file_line)
		.def("set_file_line", &cpps_csvreader::set_file_line)
		.def_inside("read_row", &cpps_csvreader::read_row)
		.def("close", &cpps_csvreader::close),
		def_inside("open",cpps_csv_open),
		defvar(c,"ignore_missing_column", cpps::ignore_missing_column),
		defvar(c,"ignore_extra_column", cpps::ignore_extra_column),
		defvar(c,"ignore_no_column", cpps::ignore_no_column)
	];



   
}
cpps_export_void  cpps_detach(cpps::C * c)
{
	cpps::_unmodule(c, "csv");
}

cpps_export_finish