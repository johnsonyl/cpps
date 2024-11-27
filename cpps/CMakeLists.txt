# CMakeList.txt: libcpps 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.5)

project(cpps)

# 将源代码添加到此项目的可执行文件。


if(WINDOWS)
	INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/../include ${PROJECT_SOURCE_DIR}/../deps/deps/include)
	LINK_DIRECTORIES(${PROJECT_SOURCE_DIR}/../lib ${PROJECT_SOURCE_DIR}/../deps/deps/lib)
ELSEIF(MACOS)
	INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/../include /usr/include /usr/local/include /usr/local/opt/zlib/include)
	LINK_DIRECTORIES(${PROJECT_SOURCE_DIR}/../lib  /lib /usr/lib /usr/local/lib /usr/lib64 )
elseif(LINUX)
	INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/../include /usr/local/include)
	LINK_DIRECTORIES(${PROJECT_SOURCE_DIR}/../lib /lib /usr/lib)
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

if(CPPS_ENABLE_ZLIB STREQUAL "ON" )
	add_definitions(-DUSE_ZLIB_CPPS)
	if(WINDOWS)
		target_link_libraries(${PROJECT_NAME} zlibstatic)
	else()
		target_link_libraries(${PROJECT_NAME} -lz)
	endif()
endif()


if(CPPS_ENABLE_TCMALLOC STREQUAL "ON" )
	add_definitions(-DUSE_TCMALLOC)
	if(WINDOWS)
		target_link_libraries(${PROJECT_NAME} libtcmalloc_minimal)
		target_link_options(${PROJECT_NAME} PRIVATE /INCLUDE:__tcmalloc)
	else()
		target_link_libraries(${PROJECT_NAME} -ltcmalloc_minimal)
	endif()
endif()

if(WINDOWS)
elseif(LINUX)
	add_custom_command(TARGET cpps
	  POST_BUILD 
	  COMMAND rm -rf /usr/local/bin/cpps
	  COMMAND ln -s ${PROJECT_SOURCE_DIR}/../bin/cpps /usr/local/bin/cpps
	 )
elseif(MACOS)
	install(TARGETS cpps
    ARCHIVE DESTINATION bin)
endif()
