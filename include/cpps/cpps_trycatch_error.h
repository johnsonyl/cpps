#pragma once

struct cpps_trycatch_error
{
public:
	cpps_trycatch_error() { _erron = 0; _line = 0; }
	cpps_trycatch_error(std::string f, int l, int n, const char *format, ...);
	virtual ~cpps_trycatch_error();

	void					attach(cpps_error e);
	cpps::cpps_value		value() { return _value; }
	int						error() { return _erron; }
	std::string				file() { return _file; }
	int						line() { return _line; }
	std::string				what() { return _s; }
	std::string				callstack() { return _callstackstr; }

	int						_erron;
	std::string				_file;
	int						_line;
	std::string				_s;
	std::string				_callstackstr;
	cpps::cpps_value		_value;
};

