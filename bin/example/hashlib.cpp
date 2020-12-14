system("chcp 65001"); //use utf8 string

#import "hashlib"


var md5 = new hashlib::md5();
md5.update("123456");
println(md5.hexdigest()); //e10adc3949ba59abbe56e057f20f883e


var sha1 = new hashlib::sha1();
sha1.update("123456");
println(sha1.hexdigest()); //7c4a8d09ca3762af61e59520943dc26494f8941b


var sha224 = new hashlib::sha224();
sha224.update("123456");
println(sha224.hexdigest());//f8cdb04495ded47615258f9dc6a3f4707fd2405434fefc3cbf4ef4e6


var sha256 = new hashlib::sha256();
sha256.update("123456");
println(sha256.hexdigest()); //8d969eef6ecad3c29a3a629280e686cf0c3f5d5a86aff3ca12020c923adc6c92


var sha384 = new hashlib::sha384();
sha384.update("123456");
println(sha384.hexdigest());//0a989ebc4a77b56a6e2bb7b19d995d185ce44090c13e2984b7ecc6d446d4b61ea9991b76a4c2f04b1b4d244841449454


var sha512 = new hashlib::sha512();
sha512.update("123456");
println(sha512.hexdigest());//ba3253876aed6bc22d4a6ff53d8406c6ad864195ed144ab5c87621b6c233b548baeae6956df346ec8c17f5ea10f35ee3cbc514797ed7ddd3145464e2a0bab413