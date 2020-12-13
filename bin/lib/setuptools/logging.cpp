#import "logging"

var log = null;
var initlogging()
{
	var simple_format = '[%(levelname)s][%(asctime)s][%(filename)s:%(lineno)d]%(message)s';
	

	var LOGGING_CONFIG = {
	    version: 1,  // 版本号
	    disable_existing_loggers: false,  //　固定写法
	    formatters: {
	        simple: {
	            format: simple_format
	        },
	    },
	    filters: {},
	    handlers: {
	        //打印到终端的日志
	        sh: {
	            level: 'INFO',
	            class: 'logging.StreamHandler',  // 打印到屏幕
	            color: [4,2,3,1,5] , // DEBUG,INFO,WARNING,ERROR,CRITICAL 颜色
	            formatter: 'simple'
	        }
	    },
	    loggers: {        
	        root: { //root 为默认
	            handlers: ['sh'],  //这里把上面定义的两个handler都加上，即log数据既写入文件又打印到屏幕
	            level: 'INFO',
	            propagate: true,  // 向上（更高level的logger）传递
	        },
	    },
	}

	logging.create_with_config(LOGGING_CONFIG);
	log = logging.getlogger("root");
}
initlogging();