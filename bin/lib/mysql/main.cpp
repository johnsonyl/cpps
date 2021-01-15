class MysqlOption
{
	var db;
	var host;
	var user;
	var passwd;
	var port;
	var isreconnect;
}
module mysql{

	//var ret = mysql.unpack( my.execute("select * from xxx;") );
	var unpack(var mysql_results){
		var ret = [];
		foreach(var result : mysql_results){
			var ret_result = [];
			var records = result.records();
			foreach(var record: records){
				ret_result.push_back( record.fields() );
			}
			ret.push_back(ret_result);
		}
		return ret;
	}
}
