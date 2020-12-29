# CMakeList.txt: libcpps 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.5)

project(curl)

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
	INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/../../include /usr/include /usr/local/opt/curl/include )
	LINK_DIRECTORIES(${PROJECT_SOURCE_DIR}/../../lib  /lib /usr/local/opt/curl/lib)
	SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../../bin/lib/${PROJECT_NAME}) 
ELSEIF(LINUX)
	INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/../../include /usr/local/include)
	LINK_DIRECTORIES(${PROJECT_SOURCE_DIR}/../../lib  /lib /usr/lib)
	SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../../bin/lib/${PROJECT_NAME}) 
ENDIF()

file(GLOB_RECURSE CURRENT_HEADERS  *.h)
source_group("Include" FILES ${CURRENT_HEADERS}) 

AUX_SOURCE_DIRECTORY(${PROJECT_SOURCE_DIR}  SRC_LIST)



ADD_LIBRARY (${PROJECT_NAME} SHARED ${SRC_LIST} ${CURRENT_HEADERS})
IF(WINDOWS)
	target_link_libraries(${PROJECT_NAME} libcpps libcrypto libcurl libssl zdll zlib )
ELSEIF(MACOS)
	target_link_libraries(${PROJECT_NAME} libcpps -lcurl -ldl -lm -lpthread)
ELSEIF(LINUX)
	target_link_libraries(${PROJECT_NAME} libcpps -lcurl -lrt -ldl -lm -lpthread -lstdc++fs)
ENDIF()
