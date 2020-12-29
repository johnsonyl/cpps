# CMakeList.txt: libcpps 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.5)

project(mysql)

# 将源代码添加到此项目的可执行文件。

IF(WINDOWS)
	INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/../../include ${PROJECT_SOURCE_DIR}/../../deps/deps/include)
	LINK_DIRECTORIES(${PROJECT_SOURCE_DIR}/../../lib ${PROJECT_SOURCE_DIR}/../../deps/deps/lib)
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/../../bin/lib/${PROJECT_NAME})
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/../../bin/lib/${PROJECT_NAME})
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/../../bin/lib/${PROJECT_NAME})


	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/../../bin/lib/${PROJECT_NAME})
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/../../bin/lib/${PROJECT_NAME})
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/../../bin/lib/${PROJECT_NAME})

	
ELSEIF(MACOS)
	INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/../../include /usr/include /usr/local/opt/mysql-client/include )
	LINK_DIRECTORIES(${PROJECT_SOURCE_DIR}/../../lib  /lib /usr/local/opt/mysql-client/lib)
	SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../../bin/lib/${PROJECT_NAME}) 
ELSEIF(LINUX)
	INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/../../include /usr/local/include)
	LINK_DIRECTORIES(${PROJECT_SOURCE_DIR}/../../lib  /lib /usr/lib /usr/local/lib /usr/local/lib64 /usr/lib64 /usr/lib64/mysql)
	SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../../bin/lib/${PROJECT_NAME}) 
ENDIF()

file(GLOB_RECURSE CURRENT_HEADERS  *.h)
source_group("Include" FILES ${CURRENT_HEADERS}) 

AUX_SOURCE_DIRECTORY(${PROJECT_SOURCE_DIR}  SRC_LIST)



ADD_LIBRARY (${PROJECT_NAME} SHARED ${SRC_LIST} ${CURRENT_HEADERS})
IF(WINDOWS)
	target_link_libraries(${PROJECT_NAME} libcpps libmysql mysqlclient mysqlservices)
ELSEIF(MACOS)
	target_link_libraries(${PROJECT_NAME} libcpps -lmysqlclient -lpthread -lz -lm -lssl -lcrypto -ldl)
ELSEIF(LINUX)
	target_link_libraries(${PROJECT_NAME} libcpps -lmysqlclient -lpthread -lz -lm -lrt -lssl -lcrypto -ldl -lstdc++fs)
ENDIF()
