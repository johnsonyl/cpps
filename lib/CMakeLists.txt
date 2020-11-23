# CMakeList.txt: 顶层 CMake 项目文件，在此处执行全局配置
# 并包含子项目。
#
cmake_minimum_required (VERSION 3.8)

project ("cppslibs")

IF (CMAKE_SYSTEM_NAME MATCHES "Windows")
ELSE (CMAKE_SYSTEM_NAME MATCHES "Windows")
	#default release.
	if (NOT EXISTS ${CMAKE_BINARY_DIR}/CMakeCache.txt)
	  if (NOT CMAKE_BUILD_TYPE)
	    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "" FORCE)
	  endif()
	endif()

	if(CMAKE_BUILD_TYPE STREQUAL "Release" )
		SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -Wfatal-errors -std=c++17 -Wno-format-y2k -fpermissive -std=gnu++0x -m64 -fPIC -fpermissive -Wno-unused-function -O3 -Wall")
		add_definitions(-DLINUX -D_FILE_OFFSET_BITS=64 -DNDEBUG)
		message("release build")
	else()
		SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -Wfatal-errors -std=c++17 -Wno-format-y2k -fpermissive -std=gnu++0x -m64 -fPIC -fpermissive -Wno-unused-function -O0 -Wall -g -ggdb")
		add_definitions(-DLINUX -D_FILE_OFFSET_BITS=64 -DDEBUG)
		message("debug build")
	endif()
	
ENDIF(CMAKE_SYSTEM_NAME MATCHES "Windows")


# 包含子项目。
add_subdirectory ("curl")
add_subdirectory ("json")
add_subdirectory ("socket")
add_subdirectory ("http")
add_subdirectory ("base64")
add_subdirectory ("mysql")
