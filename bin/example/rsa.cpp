#import "rsa"

var [prikey,pubkey] = rsa.generate_rsakey(1024);//build a 1024 PRI and PUB RSAKEY 
println(pubkey);
pubkey = rsa.pub_to_pkcs8(pubkey);   //pubkey pkcs1 -> pkcs8 
println(pubkey);
pubkey = rsa.pub_to_pkcs1(pubkey);   //pubkey pkcs8 -> pkcs1
println(pubkey);
println("----");

println(prikey);
prikey = rsa.pri_to_pkcs8(prikey);   //prikey pkcs1 -> pkcs8
println(prikey);
prikey = rsa.pri_to_pkcs1(prikey);   //prikey pkcs8 -> pkcs1
println(prikey);

var source = "123456";

var encode = rsa.pri_encrypt(source,prikey);    //rsa(pkcs1) prikey encrypt.
println(encode);

var decode = rsa.pub_decrypt(encode,pubkey);    //rsa(pkcs1) pubkey decrypt.
println(decode);

var encode = rsa.pub_encrypt(source,pubkey);    //rsa(pkcs1) pubkey encrypt.
println(encode);

var decode = rsa.pri_decrypt(encode,prikey);    //rsa(pkcs1) prikey decrypt.
println(decode);