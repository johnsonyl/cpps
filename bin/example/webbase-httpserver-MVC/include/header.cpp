#import "socket"
#import "json"
#import "mysql"
#import "base64"
#import "hashlib"
#import "logging"
#import "encoding"
#import "http"
#include <src/logging/logging.cpp>
#include <src/config/config.cpp>
//controllers
#include <src/controllers/controller.cpp>
#include <src/controllers/homecontroller.cpp>

//dal
#include <src/dal/mysqlhelper.cpp>
#include <src/dal/user.cpp>

//models
#include <src/models/user.cpp>
#include <src/models/project.cpp>

#include <src/webserver/webserver.cpp>
#include <src/program.cpp>