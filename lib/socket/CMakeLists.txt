# CMakeList.txt: libcpps 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)

project(socket)

# 将源代码添加到此项目的可执行文件。
message(${PROJECT_SOURCE_DIR})

IF(WIN32)
	INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/../../src ${PROJECT_SOURCE_DIR}/../../deps/deps/include)
	LINK_DIRECTORIES(${PROJECT_SOURCE_DIR}/../../bin ${PROJECT_SOURCE_DIR}/../../deps/deps/lib)


	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/../../bin/lib/${PROJECT_NAME})
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/../../bin/lib/${PROJECT_NAME})
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/../../bin/lib/${PROJECT_NAME})


	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/../../bin/lib/${PROJECT_NAME})
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/../../bin/lib/${PROJECT_NAME})
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/../../bin/lib/${PROJECT_NAME})



ELSE()
	INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/../../src /usr/include)
	LINK_DIRECTORIES(${PROJECT_SOURCE_DIR}/../../bin  /lib /usr/lib /usr/lib64)
	SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../../bin/lib/${PROJECT_NAME}) 
ENDIF()

AUX_SOURCE_DIRECTORY(${PROJECT_SOURCE_DIR}  SRC_LIST)



ADD_LIBRARY (${PROJECT_NAME} SHARED ${SRC_LIST})
IF(WIN32)
	target_link_libraries(${PROJECT_NAME} ws2_32 winmm wldap32 wsock32 Iphlpapi cpps libcrypto libevent libevent_core libevent_extras libevent_openssl libssl zdll zlib )
ELSE()
	target_link_libraries(${PROJECT_NAME} -lcpps -levent -levent_pthreads -lrt -ldl -lm -lpthread)
ENDIF()
