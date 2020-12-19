#ifndef cpps_mysql_h__
#define cpps_mysql_h__

#include <cpps/cpps.h>
#include <mysql/mysql.h>

namespace cpps {

	#define BINDDATA_SIZE 1 * 1024 * 1024 
	struct MYSQL_BINDDATA
	{
		bool is_null;
		unsigned long length;
		char buff[BINDDATA_SIZE];
	};

	class cpps_mysql
	{
	public:
		cpps_mysql();
		virtual ~cpps_mysql();

		void						setcstate(cpps::C* cstate);
		bool						connect(C* cstate, cpps::object option);
		void						close();
		cpps_value					call(std::string spname, cpps::cpps_value params);
		bool						checkconnect();
		cpps::cpps_value			make_cpps_value(enum_field_types type, void* buffer, unsigned long size);
		cpps_value					execute(std::string sqlcmd);
		void						result_build_records(cpps_vector* vec, bool is_mysql_stmt_store_result);

		bool						real_connect();
		bool						isconnect();
		std::string					error();
		void						seterror(std::string str);
		cpps_integer				affected_rows();
		void						clear(MYSQL_RES* m_mysql_result);

	public:
		MYSQL*						mysql;
		MYSQL_STMT*					mysql_stmt;
		std::string					db;
		std::string					host;
		std::string					user;
		std::string					passwd;
		cpps::usint16				port;
		bool						isreconnect;
		bool						connect_state;
		std::string					errorstr;
		C*							c;
		cpps_integer				affectedrows;
	};

}
#endif // cpps_mysql_h__
