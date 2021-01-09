#include "cpps_logging_streamhandler.h"


namespace cpps
{
	void cpps_console_clearcolor();
	void cpps_console_color(cpps_integer color);
	cpps_logging_streamhandler::cpps_logging_streamhandler()
		:cpps_logging_handler( cpps_logging_handler_type_streamhandler)
	{
		memset(color, 0, sizeof(int8) * 5);
		for (int32 i = 0; i < 5; i++)
			color[i] = -1;
	}

	cpps_logging_streamhandler::~cpps_logging_streamhandler()
	{

	}

	void cpps_logging_streamhandler::pop(usint8 level, std::string &msg)
	{

		level = level / 10 - 1;
		level = level < 0 ? 0 : level;
		if (color[level] != -1)
		{
			cpps_console_color(color[level]);
		}
		puts(msg.c_str());
		if (color[level] != -1)
		{
			cpps_console_clearcolor();
		}
	}

	void cpps_logging_streamhandler::setcolor(cpps_value v)
	{
		cpps_vector* vct = cpps_to_cpps_vector(v);
		size_t c = vct->size() < 5 ? vct->size() : 5;
		for (size_t i = 0; i < c; i++)
			color[i] = (int8)cpps_to_integer( vct->at(i) );
	}
}