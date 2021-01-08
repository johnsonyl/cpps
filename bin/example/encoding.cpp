system("chcp 65001"); //use utf8 string

#import "encoding"


var s = "我是中文";
println(s); //我是中文

var utf8 = encoding.encode(s,encoding.UTF8);
println(utf8); //我是中文
var gbk = encoding.encode(s,encoding.GBK);
println(gbk);//无法显示
utf8 = encoding.encode(gbk,encoding.UTF8);
println(utf8);//我是中文


var utf16 = encoding.utf8_to_utf16(s);
println(utf16);//b/f-Ne
var utf32 = encoding.utf16_to_utf32(utf16);
println(utf32);//b
utf8 = encoding.utf32_to_utf8(utf32);
println(utf8);//我是中文
