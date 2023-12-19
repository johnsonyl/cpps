#pragma once

struct cpps_trycatch_error
{
public:
	cpps_trycatch_error();
	cpps_trycatch_error(std::string f, int l, int n, const char *format, ...);
	virtual ~cpps_trycatch_error();

	void					attach(cpps_error e);
	cpps::cpps_value		value();
	int						error();
	std::string				file();
	int						line();
	std::string				what();
	std::string				callstack();
	std::string				tostring();

	int						_erron;
	std::string				_file;
	int						_line;
	std::string				_s;
	std::string				_callstackstr;
	cpps::cpps_value		_value;
};

