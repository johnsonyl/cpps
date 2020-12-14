#include "cpps_mysql_result.h"
namespace cpps {

	cpps_mysql_result::cpps_mysql_result()
	{

	}

	cpps_mysql_result::~cpps_mysql_result()
	{

	}

	cpps::cpps_vector* cpps_mysql_result::records()
	{
		return &vct_records;
	}

}