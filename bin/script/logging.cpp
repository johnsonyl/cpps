system("chcp 65001"); //use utf8 string

#import "logging"

var simple_format = '[%(levelname)s][%(asctime)s][%(filename)s:%(lineno)d]%(message)s';
var standard_format = '[%(asctime)s][task_id:%(name)s][%(filename)s:%(lineno)d][%(levelname)s][%(message)s]';

var LOGGING_CONFIG = {
    version: 1,  // 版本号
    disable_existing_loggers: false,  //　固定写法
    formatters: {
        standard: {
            format: standard_format
        },
        simple: {
            format: simple_format
        },
    },
    filters: {},
    handlers: {
        //打印到终端的日志
        sh: {
            level: 'DEBUG',
            class: 'logging.StreamHandler',  // 打印到屏幕
            color: [5,4,3,2,1] , // DEBUG,INFO,WARNING,ERROR,CRITICAL 颜色
            formatter: 'simple'
        },
        fh: {
            level: 'DEBUG',
            class: 'logging.handlers.TimedRotatingFileHandler',  // 打印到屏幕
            formatter: 'standard',
            filename: "log/log.txt",
            mode: "ab+",
            backupCount:5,   //保留5个
            when:"S",		//秒
            interval:5     //5秒
        },
    },
    loggers: {        
        root: { //root 为默认
            handlers: ['sh','fh'],  //这里把上面定义的两个handler都加上，即log数据既写入文件又打印到屏幕
            level: 'DEBUG',
            propagate: true,  // 向上（更高level的logger）传递
        },
    },
}

logging.create_with_config(LOGGING_CONFIG);

while(true){
	logging.debug("debug");
	logging.info("info");
	logging.warning("warning");
	logging.error("error");
	logging.critical("critical");
	sleep(2000);
}
