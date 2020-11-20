#pragma once

class cpps_trycatch_error
{
public:
	cpps_trycatch_error(){}
	cpps_trycatch_error(std::string f, int l, int n, const char *format, ...);
	cpps_trycatch_error(cpps_error e);
	~cpps_trycatch_error();

	cpps::cpps_value	getvalue() { return value; }

	cpps::cpps_value value;

	int		geterrno() { return erron; }
	std::string getfile() { return file; }
	int		getline() { return line; }
	std::string geterrstr() { return s; }

	int erron;
	std::string file;
	int line;
	std::string s;
};

