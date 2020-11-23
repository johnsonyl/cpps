#include "cpps_mysql.h"
#include "cpps_mysql_record.h"
#include "cpps_mysql_result.h"
namespace cpps
{

	cpps_mysql::cpps_mysql()
	{
		connect_state = false;
		isreconnect = true;
		port = 3306;
		mysql = NULL;
		mysql_stmt = NULL;
	}

	cpps_mysql::~cpps_mysql()
	{
		close();
	}

	void cpps_mysql::setcstate(cpps::C* cstate)
	{
		c = cstate;
	}
	bool cpps_mysql::connect(C *cstate,cpps::object option)
	{
		setcstate(cstate);
		if (cpps::type(option["db"]) == CPPS_TSTRING) db = cpps::object_cast<std::string>(option["db"]);
		if (cpps::type(option["host"]) == CPPS_TSTRING) host = cpps::object_cast<std::string>(option["host"]);
		if (cpps::type(option["user"]) == CPPS_TSTRING) user = cpps::object_cast<std::string>(option["user"]);
		if (cpps::type(option["passwd"]) == CPPS_TSTRING) passwd = cpps::object_cast<std::string>(option["passwd"]);
		if (cpps::type(option["port"]) == CPPS_TINTEGER) port = cpps::object_cast<cpps::usint16>(option["port"]);
		if (cpps::type(option["isreconnect"]) == CPPS_TBOOLEAN) isreconnect = cpps::object_cast<bool>(option["isreconnect"]);

		return real_connect();
	}

	void cpps_mysql::close()
	{
		if (mysql_stmt)
		{
			mysql_stmt_close(mysql_stmt);
			mysql_stmt = NULL;
		}

		if (mysql)
		{
			mysql_close(mysql);
			mysql = NULL;
			mysql_library_end();//free memory; 这里多连接不知道会不会释放其他连接的??
		}
		connect_state = false;
	}

	cpps::cpps_value cpps_mysql::call(std::string spname, cpps::cpps_value params)
	{

		cpps::cpps_value nil;
		std::string proc = "call ";
		proc += spname;
		proc += "(";

		cpps_vector* vec = cpps_to_cpps_vector(params);
	
		std::vector< MYSQL_BIND > paramlist;
		std::vector< MYSQL_BINDDATA > paramDatalist;

		paramlist.resize(vec->size());
		paramDatalist.resize(vec->size());

		memset(paramlist.data(), 0, sizeof(MYSQL_BIND) * vec->size());

		//填充参数数据
		for (size_t i = 0; i < vec->realvector().size(); i++)
		{
			paramlist[i].buffer = paramDatalist[i].buff;
			paramlist[i].length = &paramDatalist[i].length;
			paramlist[i].is_null = &paramDatalist[i].is_null;

			cpps_value& v = vec->realvector()[i];
			paramDatalist[i].is_null = false;
			if (v.tt == CPPS_TBOOLEAN)
			{
				bool n = v.value.b;
				memcpy(paramDatalist[i].buff, (const void*)&n, sizeof(bool));
				paramDatalist[i].length = sizeof(bool);
				paramlist[i].buffer_type = MYSQL_TYPE_TINY;
				paramlist[i].buffer_length = sizeof(bool);
			}
			else if(v.tt == CPPS_TINTEGER)
			{
				cpps_integer n = v.value.integer;
				memcpy(paramDatalist[i].buff, (const void*)&n, sizeof(cpps_integer));
				paramDatalist[i].length = sizeof(cpps_integer);
				paramlist[i].buffer_type = MYSQL_TYPE_LONGLONG;
				paramlist[i].buffer_length = sizeof(cpps_integer);
			}
			else if (v.tt == CPPS_TNUMBER)
			{
				cpps_number n = v.value.number;
				memcpy(paramDatalist[i].buff, (const void*)&n, sizeof(cpps_number));
				paramDatalist[i].length = sizeof(cpps_number);
				paramlist[i].buffer_type = MYSQL_TYPE_DOUBLE;
				paramlist[i].buffer_length = sizeof(cpps_number);
			}
			else if(v.tt == CPPS_TSTRING)
			{
				std::string *s = cpps_get_string(v);
				memcpy(paramDatalist[i].buff, (const void*)s->c_str(), s->size());
				paramDatalist[i].length = (unsigned long)s->size();
				paramlist[i].buffer_type = MYSQL_TYPE_VAR_STRING;
				paramlist[i].buffer_length = (unsigned long)s->size();
			}
			else
			{
				paramDatalist[i].is_null = true;
			}
			if (i != 0)
				proc += ",";

			proc += "?";
		}

		proc += ");";


		if (!checkconnect() && !real_connect())
		{
			return nil;
		}

		if (mysql_stmt_prepare(mysql_stmt, proc.c_str(), (unsigned long)proc.size()))
		{
			//如果断开连接了，那么尝试重连
			if (!checkconnect() && !real_connect())
			{
				return nil;
			}

			//尝试再次转换
			if (mysql_stmt_prepare(mysql_stmt, proc.c_str(), (unsigned long)proc.size()))
			{
				seterror(mysql_stmt_error(mysql_stmt));
				return nil;
			}
		}

		//传入参数
		if (mysql_stmt_bind_param(mysql_stmt, paramlist.data()))
		{
			seterror(mysql_stmt_error(mysql_stmt));
			return nil;
		}

		//执行语句
		if (mysql_stmt_execute(mysql_stmt))
		{
			seterror(mysql_stmt_error(mysql_stmt));
			return nil;
		}

		cpps_create_class_var(cpps_vector, c, cpps_vector_var, cpps_vector_ptr);
		result_build_records(cpps_vector_ptr,false);
		return cpps_vector_var;
	}
	bool cpps_mysql::checkconnect()
	{
		//已经断开过连接了。
		if (mysql_stmt == NULL) return false;

		//检测当前连接状态
		if ((mysql_stmt_errno(mysql_stmt) == CR_SERVER_GONE_ERROR) ||
			(mysql_stmt_errno(mysql_stmt) == CR_SERVER_LOST) ||
			(mysql_stmt_errno(mysql_stmt) == CR_CONN_HOST_ERROR))
		{
			seterror(mysql_stmt_error(mysql_stmt));
			return false;
		}
		return true;
	}
	cpps::cpps_value cpps_mysql::make_cpps_value(enum_field_types type, void* buffer, unsigned long size )
	{
		cpps::cpps_value ret;
		if (type == MYSQL_TYPE_DOUBLE )
		{
			cpps_number n = (cpps_number)(*(cpps_number*)(buffer));
			ret = n;
		}
		else if(type == MYSQL_TYPE_FLOAT)
		{
			cpps_number n = (cpps_number)(*(float*)(buffer));
			ret = n;
		}
		else if (IS_NUM(type))
		{
			cpps_integer n = 0;
			
			n = (cpps_integer)(*(cpps::int64*)(buffer));
			
			ret = n;
		}
		else if (IS_LONGDATA(type))
		{
			std::string string_ptr;
			string_ptr.append((const char *)buffer, size);
			ret = cpps_value(c, string_ptr);
		}
		else
		{
			//what??
		}
		return ret;
	}
	cpps::cpps_value cpps_mysql::execute(std::string sqlcmd)
	{


		cpps::cpps_value nil;
		if (sqlcmd.empty()) return nil; //NIL

		if (!checkconnect() && !real_connect())
		{
			return nil;
		}

		if (mysql_stmt_prepare(mysql_stmt, sqlcmd.c_str(),(unsigned long) sqlcmd.size()))
		{
			//如果断开连接了，那么尝试重连
			if (!checkconnect() && !real_connect())
			{
				return nil;
			}

			//尝试再次转换
			if (mysql_stmt_prepare(mysql_stmt, sqlcmd.c_str(), (unsigned long)sqlcmd.size()))
			{
				seterror(mysql_stmt_error(mysql_stmt));
				return nil;
			}
		}
		//执行语句
		if (mysql_stmt_execute(mysql_stmt))
		{
			seterror(mysql_stmt_error(mysql_stmt));
			return nil;
		}
		cpps_create_class_var(cpps_vector,c,cpps_vector_var, cpps_vector_ptr);
		result_build_records(cpps_vector_ptr,true);
		return cpps_vector_var;
	}

	void cpps_mysql::result_build_records(cpps_vector* vec,bool is_mysql_stmt_store_result)
	{

		
		do 
		{
			MYSQL_RES* m_mysql_result = mysql_stmt_result_metadata(mysql_stmt);
			if (m_mysql_result)
			{
				/*if (!m_mysql_result)
				{
					m_mysql_result = mysql_store_result(mysql);
					if (!m_mysql_result) {

						clear(m_mysql_result);
						break;
					}
					int err = mysql_stmt_next_result(mysql_stmt);
					printf("%d", err);
				}*/


				cpps_create_class_var(cpps_mysql_result, c, cpps_mysql_result_var, cpps_mysql_result_ptr);
				vec->push_back(cpps_mysql_result_var);
				uint64_t	m_num_rows;
				MYSQL_FIELD* m_fields = mysql_fetch_fields(m_mysql_result);
				int m_num_fields = mysql_num_fields(m_mysql_result);
				if (m_fields && m_num_fields != 0)
				{
					std::vector< MYSQL_BIND > resultlist;
					std::vector< MYSQL_BINDDATA > resultDatalist;

					resultlist.resize(m_num_fields);
					resultDatalist.resize(m_num_fields);
					memset(resultlist.data(), 0, sizeof(MYSQL_BIND) * m_num_fields);

					//填充数据BIND
					for (int i = 0; i < m_num_fields; i++)
					{
						resultlist[i].buffer_type = m_fields[i].type;
						resultlist[i].buffer = resultDatalist[i].buff;
						resultlist[i].length = &resultDatalist[i].length;
						resultlist[i].buffer_length = BINDDATA_SIZE;
						resultlist[i].is_null = &resultDatalist[i].is_null;
					}

					if (mysql_stmt_bind_result(mysql_stmt, resultlist.data()))
					{
						seterror(mysql_stmt_error(mysql_stmt));
						clear(m_mysql_result);
						return;
					}

					int err = mysql_stmt_store_result(mysql_stmt);
					if (is_mysql_stmt_store_result && err != 0)
					{
						seterror(mysql_stmt_error(mysql_stmt));
						clear(m_mysql_result);
						return;
					}


					m_num_rows = mysql_stmt_num_rows(mysql_stmt);

					for (uint64_t i = 0; i < m_num_rows; i++)
					{
						memset(resultDatalist.data(), 0, sizeof(MYSQL_BINDDATA) * m_num_fields);
						mysql_stmt_fetch(mysql_stmt);

						cpps_create_class_var(cpps_mysql_record, c, cpps_mysql_record_var, cpps_mysql_record_ptr);
						for (int j = 0; j < m_num_fields; j++)
						{
							cpps::cpps_value v = make_cpps_value(m_fields[j].type, resultlist[j].buffer, resultDatalist[j].length);
							cpps_mysql_record_ptr->add(cpps_value(c,m_fields[j].name), v);
						}
						cpps_mysql_result_ptr->vct_records.push_back(cpps_mysql_record_var);
					}


				}

				mysql_free_result(m_mysql_result);
				m_mysql_result = NULL;
				mysql_stmt_free_result(mysql_stmt);
			}

		} while (mysql_stmt_next_result(mysql_stmt) == 0);
	}

	bool cpps_mysql::real_connect()
	{
		close();
		
		mysql = mysql_init(NULL);
		if (!mysql)
		{
			connect_state = false;
			seterror(mysql_error(mysql));
			return false;
		}

		if (mysql != mysql_real_connect(mysql, host.c_str(), user.c_str(), passwd.c_str(), db.c_str(), port, NULL, (CLIENT_FOUND_ROWS | CLIENT_MULTI_STATEMENTS | CLIENT_MULTI_RESULTS | CLIENT_MULTI_QUERIES)))
		{
			connect_state = false;
			seterror(mysql_error(mysql));
			return false;
		}

		//初始化stmt
		mysql_stmt = mysql_stmt_init(mysql);
		if (!mysql_stmt)
		{
			connect_state = false;
			seterror(mysql_stmt_error(mysql_stmt));
			return false;
		}
		unsigned long true_value = CURSOR_TYPE_READ_ONLY;
		mysql_stmt_attr_set(mysql_stmt, STMT_ATTR_CURSOR_TYPE, (void*)&true_value);
		bool flag = false;
		mysql_stmt_attr_set(mysql_stmt, STMT_ATTR_UPDATE_MAX_LENGTH, &flag);
		execute("set names utf8;");
		execute("set global max_prepared_stmt_count=65536;");

		connect_state = true;

		return true;
	}

	bool cpps_mysql::isconnect()
	{
		return connect_state && checkconnect();
	}

	std::string cpps_mysql::error()
	{
		return errorstr;
	}

	void cpps_mysql::seterror(std::string str)
	{
		errorstr = str;
	}

	void cpps_mysql::clear(MYSQL_RES* m_mysql_result)
	{
		do
		{
			if (!m_mysql_result)
				m_mysql_result = mysql_store_result(mysql);
			

			mysql_free_result(m_mysql_result);
			m_mysql_result = NULL;
			mysql_stmt_free_result(mysql_stmt);
		} while (!mysql_next_result(mysql));
	}

}