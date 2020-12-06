//system("chcp 65001"); //use utf8 string

#import "encoding"


var s = "我是中文";
println(s); //鎴戞槸涓枃

var utf8 = encoding.encode(s,encoding.UTF8);
println(utf8); //鎴戞槸涓枃
var gbk = encoding.encode(s,encoding.GBK);
println(gbk);//我是中文
utf8 = encoding.encode(gbk,encoding.UTF8);
println(utf8);//鎴戞槸涓枃
s = encoding.encode(utf8,encoding.GBK);
println(s);//我是中文


