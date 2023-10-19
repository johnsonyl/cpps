#ifndef cpps_mysql_record_h__
#define cpps_mysql_record_h__

#include <cpps/cpps.h>
#include <unordered_map>

namespace cpps
{
	typedef std::unordered_map<std::string, cpps_value> RECORD_VALUES;
	class cpps_mysql_record
	{
	public:
		cpps_mysql_record();
		virtual ~cpps_mysql_record();

		void			add(cpps_value k, cpps_value v);
		cpps_value		get(cpps_value k);
		cpps_map*		fields();

		cpps_map		record_values;
	};
}


#endif // cpps_mysql_record_h__
