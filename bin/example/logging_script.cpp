system("chcp 65001"); //use utf8 string

#import "logging"

logging.create({level:logging.DEBUG,
                format:'[%(levelname)s][%(asctime)s][%(filename)s:%(lineno)d]%(message)s',
                filename:'log/myapp.log',
                filemode:'ab+'});
var logger = logging.getlogger('');
println(logger);
var scripthandler = new logging::ScriptHandler(HTTPHandler);
scripthandler.setlevel(logging.DEBUG);
scripthandler.setformatter('[%(levelname)s][%(asctime)s][%(filename)s:%(lineno)d][%(tid)d]%(message)s');


logger.addhandler(scripthandler);



logger.debug("debug");
logger.info("info");
logger.warning("warning");
logger.error("error");
logger.critical("critical");

sleep(10000);