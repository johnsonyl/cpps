# CMakeList.txt: libcpps 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.5)

project(cpps)

# 将源代码添加到此项目的可执行文件。


if(WINDOWS)
	INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/../include)
	LINK_DIRECTORIES(${PROJECT_SOURCE_DIR}/../lib)
else()
	message(${PROJECT_SOURCE_DIR}/../include)
	INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/../include /usr/local/include)
	LINK_DIRECTORIES(${PROJECT_SOURCE_DIR}/../lib  /lib /usr/lib /usr/local/lib /usr/lib64)
endif()

AUX_SOURCE_DIRECTORY(${PROJECT_SOURCE_DIR}  SRC_LIST)


IF(WINDOWS)
	
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/../bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/../bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/../bin)


set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/../bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/../bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/../bin)

ELSE()
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../bin)
ENDIF()


if(WINDOWS)
	add_executable (${PROJECT_NAME} ${SRC_LIST} ${PROJECT_SOURCE_DIR}/cpps.rc)
	target_link_libraries(${PROJECT_NAME} libcpps)
elseif(LINUX)
	add_executable (${PROJECT_NAME} ${SRC_LIST})
	target_link_libraries(${PROJECT_NAME} libcpps -lrt -ldl -lm -lpthread -lstdc++fs)
elseif(MACOS)
	add_executable (${PROJECT_NAME} ${SRC_LIST})
	target_link_libraries(${PROJECT_NAME} libcpps -ldl -lm -lpthread)
endif()
if(WINDOWS)
else()
	add_custom_command(TARGET cpps
	  POST_BUILD 
	  COMMAND rm -rf /usr/local/bin/cpps
	  COMMAND ln -s ${PROJECT_SOURCE_DIR}/../bin/cpps /usr/local/bin/cpps
	 )
#	不在安装 cppsc 到系统目录,需要手动建立软连接.
# 	install(TARGETS cppsc
#	RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/local/bin/)
endif()
