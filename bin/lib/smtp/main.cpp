#import "htmlparser"
#import "base64"
#import "quotedprintable"

module smtp
{
    var makeboundary()
    {
        return "__{math.random(100,999)}_NextPart_{math.random(10000000,99999999)}_=----";
    }
    var encodeutf8text(var s)
    {
        var str = "=?UTF-8?B?{base64.encode(s)}?=";
        return str;
    }
    var sendmail(var opt)
    {
        var content = "";
        var times = time.strftime("%a,%d %b %Y-%m-%d %H:%M:%S %z ",time.gettime());
        content ..= "Date: {times}\r\n";

        if(isvalid(opt["messageid"]))
            content ..= "Message-ID: <{opt["messageid"]}>\r\n";
        
        var mailfrom ;
        if(isvalid(opt["mailfrom"]))
            mailfrom = opt["mailfrom"];
        else
            mailfrom = opt["username"];

        var sender;
        if(isvalid(opt["sender"]))
            sender = opt["sender"];
        else
            sender = string.split(mailfrom,"@",1)[0];
        var from;
        if(isvalid(opt["from"]))
            from = opt["from"];
        else
            from = mailfrom;

        content ..= "From: {sender} <{from}>\r\n";
        
        var tos = "";
        var ccs = "";
        var to = opt["to"];
        var cc = opt["cc"];
        var bcc = opt["bcc"];
        var i = 0;

        foreach(var rcpt: to){
            if(i > 0) tos ..= ",";

            if(isvector(rcpt)){
                if(rcpt.size() >= 2)
                {
                    tos ..= '"{encodeutf8text(rcpt[1])}" ';
                }
                tos ..= "<{rcpt[0]}>";
            }
            else if(isstring(rcpt))
                tos ..= "<{rcpt}>";
            i++;
        }
        i = 0;
        foreach(var rcpt: cc){
            if(i > 0) ccs ..= ",";

            if(isvector(rcpt)){
                if(rcpt.size() >= 2)
                {
                    ccs ..= '"{encodeutf8text(rcpt[1])}" ';
                }
                ccs ..= "<{rcpt[0]}>";
            }
            else if(isstring(rcpt))
                ccs ..= "<{rcpt}>";
            i++;
        }

        if(to && !to.empty()) content ..= "To: {tos}\r\n";
        if(cc && !cc.empty()) content ..= "Cc: {ccs}\r\n";
        
        var subject;
        if(isvalid(opt["subject"]))
            subject = opt["subject"];
        
        content ..= "Subject: {subject}\r\n";
        var charset = "UTF-8";
        if(isvalid(opt["charset"]))
            charset = opt["charset"];

        var mixed_smtp_boundary = makeboundary();
        content ..= 'Content-Type: multipart/alternative; charset={charset}; boundary="{mixed_smtp_boundary}"\r\n';
        content ..= "MIME-Version: 1.0\r\n";

        if(isvalid(opt["xmailer"]))
            content ..= "X-Mailer: {opt["xmailer"]}\r\n";

        if(isvalid(opt["headers"]))
            foreach(var header : opt["headers"])
                content ..= "{header.first()}: {header.second()}\r\n";
            

        content ..= "\r\nThis is a multi-part message in MIME format.\r\n\r\n";

        var boundary;
        
        if(isvalid(opt["boundary"]))
            boundary = opt["boundary"];
        else
            boundary = makeboundary();


        content ..= "--{mixed_smtp_boundary}\r\nContent-Type: multipart/alternative;\n	boundary=\"{boundary}\"\r\n\r\n";

        var mail_content = "";
        if(isvalid(opt["content"]))
            mail_content = opt["content"];

        var mail_content_text_plain = "";
        var parser1 = new htmlparser::parser();
        var element = parser1.parse(mail_content);
        if(element)
            mail_content_text_plain = element.text();
        
        var mail_encoding_type = ["8bit","base64","quoted-printable"];
        var mail_encoding_type_index = 1; //default base64;
        if(isvalid(opt["encoding"]))
            mail_encoding_type_index = opt["encoding"];
        
        content ..= "--{boundary}\r\n";
        content ..= 'Content-type: text/plain; charset="{charset}"\r\n';
        content ..= "Content-Transfer-Encoding: {mail_encoding_type[mail_encoding_type_index]}\r\n";
        if(mail_encoding_type_index == 2) content ..= "Content-Disposition: inline\r\n";
        content ..= "\r\n";

        if(mail_encoding_type_index == 0)
            content ..= "{mail_content_text_plain}\r\n\r\n";
        else if(mail_encoding_type_index == 1){
            var l = string.cut(mail_content_text_plain,54);
            foreach(var s : l)
                content ..= "{base64.encode(s)}\r\n";
            content ..= "\r\n";
        }
        else if(mail_encoding_type_index == 2)
            content ..= "{quotedprintable.encode(mail_content_text_plain,73)}\r\n\r\n";

        content ..= "--{boundary}\r\n";
        content ..= 'Content-type: text/html; charset="{charset}"\r\n';
        content ..= "Content-Transfer-Encoding: {mail_encoding_type[mail_encoding_type_index]}\r\n";
        content ..= "Content-Disposition: inline\r\n";
        content ..= "\r\n";

        if(mail_encoding_type_index == 0)
            content ..= "{mail_content}\r\n\r\n";
        else if(mail_encoding_type_index == 1){
            var l = string.cut(mail_content,54);
            foreach(var s : l)
                content ..= "{base64.encode(s)}\r\n";
            content ..= "\r\n";
        }
        else if(mail_encoding_type_index == 2)
            content ..= "{quotedprintable.encode(mail_content)}\r\n\r\n";
        

        if(isvalid(opt["attachments"])){
            foreach(var file: opt["attachments"]){
                var filename = encodeutf8text(file.first());
                var file_content = file.second();
                var file_content_content = " ";
                if(isvalid(file_content["content"]))
                    file_content_content = file_content["content"];

                var file_content_type = "application/x-msdownload"; //默认附件
                var file_encoding = "base64";
                var file_id ;
                if(isvalid(file_content["id"]))
                    file_id = file_content["id"];
                if(isvalid(file_content["type"]))
                    file_content_type = file_content["type"];
                

                //附件模式
                if(file_content_type == "application/x-msdownload")
                {
                    content ..= "--{boundary}\r\n";
                    content ..= 'Content-Type: application/x-msdownload; name="{filename}"\r\n';
                    content ..= "Content-Transfer-Encoding: {file_encoding}\r\n";
                    content ..= 'Content-Disposition: attachment; filename="{filename}"\r\n\r\n';

                    var l = string.cut(file_content_content,54);
                    foreach(var s : l)
                        content ..= "{base64.encode(s)}\r\n";
                }
                else //非附件模式 
                {
                    content ..= "--{boundary}\r\n";
                    var file_content_boundary = makeboundary();
                    content ..= 'Content-Type: multipart/related;\r\n	boundary="{file_content_boundary}"\r\n\r\n';
                    content ..= "--{file_content_boundary}\r\n";
                    content ..= 'Content-Type: {file_content_type}; name="{filename}"\r\n';
                    content ..= "Content-Transfer-Encoding: base64\r\n";
                    if(file_id != null) content ..= "Content-ID: <{file_id}>\r\n";
                    content ..= "\r\n";

                    var l = string.cut(file_content_content,54);
                    foreach(var s : l)
                        content ..= "{base64.encode(s)}\r\n";

                    content ..= "\r\n--{file_content_boundary}--\r\n";
                }

            }
        }

        var mail = new smtpclient();
        mail.setsmtp(opt["host"],opt["port"]);
        if(opt["username"] != null)
            mail.setuserinfo(opt["username"],opt["pwd"]);
        mail.setmailfrom(mailfrom);
        if(opt["ssl"] != null) 
            mail.openssl(opt["ssl"]);
        foreach(var rcpt: to){
             if(isvector(rcpt)){
                mail.addrecipient(rcpt[0]);
            }
            else if(isstring(rcpt))
                mail.addrecipient(rcpt);
        }
        foreach(var rcpt: cc){
             if(isvector(rcpt)){
                mail.addrecipient(rcpt[0]);
            }
            else if(isstring(rcpt))
                mail.addrecipient(rcpt);
        }
        foreach(var rcpt: bcc){
             if(isvector(rcpt)){
                mail.addrecipient(rcpt[0]);
            }
            else if(isstring(rcpt))
                mail.addrecipient(rcpt);
        }
        if(opt["verbose"] != null)
            mail.setverbose(opt["verbose"]);
        if(opt["timeout"] != null)
            mail.settimeout(opt["timeout"]);
        if(opt["onread"] != null)
            mail.setreadcallback(opt["onread"]);
        
        mail.setcontent(content);
        return mail.send();
    }
}