class mysqlhelper
{
    var conn = null;
   mysqlhelper()
   {
       
   }
   var mysql_option = new MysqlOption()
    {
        db = globalconfig["mysql"]["db"],
        host = globalconfig["mysql"]["host"],
        user = globalconfig["mysql"]["user"],
        passwd = globalconfig["mysql"]["passwd"],
        port = globalconfig["mysql"]["port"],
        isreconnect = globalconfig["mysql"]["isreconnect"]
    }
   //打开数据库连接
   var open()
    {
        if(conn == null)
        {
            conn = mysql.connect(mysql_option);
        }
        
    }
    var row()
    {
        return conn.affected_rows();
    }
    //执行Update,Delete,Insert操作
   var executequery(var sqlstr)
   {
       open();
       var results = conn.execute(sqlstr);
       close();
       return results;
   }
   //执行返回结果集数据
   var executetabel(var sqlstr,var paras)
   {

   }
   //执行返回分页数据
   var executepage(var sqlstr,var page,var limt)
   {

   }
   //执行事务
   var executecommandbytran(var sqlparas)
   {

   }
    //关闭数据库连接
   var close()
    {
        conn.close();
    }


}
