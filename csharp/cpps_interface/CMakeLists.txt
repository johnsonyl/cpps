# CMakeList.txt: libcpps 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.5)

project(cpps_interface)

# 将源代码添加到此项目的可执行文件。
message(${PROJECT_SOURCE_DIR})

if(WINDOWS)
	INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/../../include)
	LINK_DIRECTORIES(${PROJECT_SOURCE_DIR}/../../bin ${PROJECT_SOURCE_DIR}/../../deps/deps/lib)
else()
	INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/../../include /usr/local/include)
	LINK_DIRECTORIES(${PROJECT_SOURCE_DIR}/../../lib ${PROJECT_SOURCE_DIR}/../../bin /lib /usr/lib /usr/local/lib /usr/lib64 /usr/local/lib64)
endif()
AUX_SOURCE_DIRECTORY(${PROJECT_SOURCE_DIR}  SRC_LIST)

file(GLOB_RECURSE CURRENT_HEADERS  *.h)
source_group("Include" FILES ${CURRENT_HEADERS}) 


IF(WINDOWS)
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/../../bin)
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/../../bin)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/../../bin)


	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/../../bin)
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/../../bin)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/../../bin)
ELSE()
	SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../../bin) 
ENDIF()


ADD_LIBRARY (${PROJECT_NAME} SHARED ${SRC_LIST} ${CURRENT_HEADERS})
IF(WINDOWS)
	target_link_libraries(cpps_interface libcpps libtcmalloc_minimal)
	target_link_options(${PROJECT_NAME} PRIVATE /INCLUDE:__tcmalloc)
ELSE()
	target_link_libraries(cpps_interface libcpps -ltcmalloc)
ENDIF()
