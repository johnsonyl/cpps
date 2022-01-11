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
	std::string				tostring() {
		char* fmt = new char[40960]; //40K´íÎó×Ü¹»ÁË°É
		memset(fmt, 0, 40960);
		std::string ret;
		sprintf(fmt, "ERROR INFOMATION: %s  file:%s line:%d\r\n%s", what().c_str(), file().c_str(), line(), callstack().c_str());
		ret.append(fmt);
		delete[] fmt;
		return ret;
	}

	int						_erron;
	std::string				_file;
	int						_line;
	std::string				_s;
	std::string				_callstackstr;
	cpps::cpps_value		_value;
};

