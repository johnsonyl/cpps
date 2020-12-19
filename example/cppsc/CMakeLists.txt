# CMakeList.txt: libcpps 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)

project(cppsc)

# 将源代码添加到此项目的可执行文件。


if(WINDOWS)
	INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/../../include)
	LINK_DIRECTORIES(${PROJECT_SOURCE_DIR}/../../bin)
else()
	message(${PROJECT_SOURCE_DIR}/../../include)
	INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/../../include /usr/local/include)
	LINK_DIRECTORIES(${PROJECT_SOURCE_DIR}/../../bin  /lib /usr/lib /usr/local/lib /usr/lib64)
endif()

AUX_SOURCE_DIRECTORY(${PROJECT_SOURCE_DIR}  SRC_LIST)


IF(WINDOWS)
	
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/../../bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/../../bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/../../bin)


set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/../../bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/../../bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/../../bin)

ELSE()
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../../bin)
ENDIF()

add_executable (${PROJECT_NAME} ${SRC_LIST})

if(WINDOWS)
	target_link_libraries(${PROJECT_NAME} cpps)
elseif(LINUX)
	target_link_libraries(${PROJECT_NAME} -lrt -ldl -lm -lpthread cpps)
elseif(MACOS)
	target_link_libraries(${PROJECT_NAME} -ldl -lm -lpthread cpps)
endif()
if(WINDOWS)
else()
	add_custom_command(TARGET cppsc
	  POST_BUILD 
	  COMMAND rm -rf /usr/local/bin/cppsc
	  COMMAND ln -s ${PROJECT_SOURCE_DIR}/../../bin/cppsc /usr/local/bin/cppsc
	 )
#	不在安装 cppsc 到系统目录,需要手动建立软连接.
# 	install(TARGETS cppsc
#	RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/local/bin/)
endif()
