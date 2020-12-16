#include "cpps_mysql_record.h"
namespace cpps
{

	cpps_mysql_record::cpps_mysql_record()
	{

	}

	cpps_mysql_record::~cpps_mysql_record()
	{
		record_values.clear();
	}

	void cpps_mysql_record::add(cpps_value k, cpps_value v)
	{
		record_values.insert(k, v);
	}

	cpps::cpps_value cpps_mysql_record::get(cpps_value k)
	{
		return record_values[k];
	}
	cpps_map* cpps_mysql_record::fields()
	{
		return &record_values;
	}
}