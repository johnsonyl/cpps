system("chcp 65001"); //use utf8 string

#import "logging"

logging.create({level:logging.DEBUG,
                format:'[%(levelname)s][%(asctime)s][%(filename)s:%(lineno)d]%(message)s',
                filename:'log/myapp.log',
                filemode:'ab+'});
var logger = logging.getlogger('');
var streamhandler = new logging::StreamHandler();
streamhandler.setlevel(logging.DEBUG);
streamhandler.setformatter('[%(levelname)s][%(asctime)s][%(filename)s:%(lineno)d]%(message)s');
streamhandler.setcolor([1,2,3,4,5]);

logger.addhandler(streamhandler);



logger.debug("debug");
logger.info("info");
logger.warning("warning");
logger.error("error");
logger.critical("critical");

