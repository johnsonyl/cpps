system("chcp 65001"); //use utf8 string

#import "base64"


var s = "abc";
println("source:{s}");
var encode_s = base64.encode(s);
println("encode_s:{encode_s}");
var deocde_s = base64.decode(encode_s);
println("deocde_s:{deocde_s}");