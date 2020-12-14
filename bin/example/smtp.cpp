#import "smtp"

//基础版本 够用
var code = smtp.sendmail({
			host:"smtp.aliyun.com",                     //必填
            port:25,                                    //必填
			username:"********@aliyun.com",            //必填
			pwd:"********",                            //必填
			content:"<a>hello</a>",                    //必填
            subject:"hello",                           //必填
			messageid:"{time.gettime()}@aliyun.com",      //按这个格式换下
			to:[
                ["88481106@qq.com","名字"],             //2种格式
            ],
            onread:[](var s){
                println(s);
            }
		});
println(code);


//旗舰版本
// smtp.sendmail({
// 			host:"smtp.aliyun.com",                     //必填
//             port:25,                                    //必填
// 			username:"********@aliyun.com",            //必填
// 			pwd:"********",                            //必填
//             ssl:false,
//             timeout:30,
// 			mailfrom:"********@aliyun.com",
// 			from:"********@aliyun.com",
// 			sender:"johnson",
// 			content:"<a>hello</a>",                    //必填
//             subject:"hello",                           //必填
//             verbose:true,
//             encoding:2,
// 			boundary:smtp.makeboundary(),
// 			messageid:"{time.gettime()}@aliyun.com",      //按这个格式换下
// 			charset:"UTF-8",
// 			tls:false,
// 			to:[
//                 ["88481106@qq.com","名字"],             //2种格式
//                 "14827@qq.com"
//             ],
// 			cc:[],
// 			bcc:[],
// 			headers:{
// 				GUID:"MakeGUID",
// 			},
// 			attachments:{
// 				aaa.txt:{
// 					type:"text/plain", //非附件
// 					id:"content-id-test",
// 					content:"aaaa",
// 				},
//                 bbb.txt:{
// 					type:"application/x-msdownload",  //附件
// 					id:"content-id-test",
// 					content:"bbbb",
// 				}
// 			}
// 		});

