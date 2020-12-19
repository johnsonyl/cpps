#ifndef cpps_mysql_result_h__
#define cpps_mysql_result_h__


#include <cpps/cpps.h>

namespace cpps {

	class cpps_mysql_result
	{
	public:
		cpps_mysql_result();
		virtual ~cpps_mysql_result();

		cpps_vector*		records();

	public:
		cpps_vector			vct_records;
	};
}


#endif // cpps_mysql_result_h__
