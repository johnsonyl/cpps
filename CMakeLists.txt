# CMakeList.txt: 顶层 CMake 项目文件，在此处执行全局配置
# 并包含子项目。
#
cmake_minimum_required (VERSION 3.5)

project ("libcpps")

if (CMAKE_HOST_WIN32)
    set(WINDOWS 1)
elseif (CMAKE_HOST_APPLE)
    set(MACOS 1)
elseif (CMAKE_HOST_UNIX)
    set(LINUX 1)
endif ()

if (WINDOWS)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
	add_compile_options("/std:c++17")
elseif (MACOS)
	#default release.
	if (NOT EXISTS ${CMAKE_BINARY_DIR}/CMakeCache.txt)
	  if (NOT CMAKE_BUILD_TYPE)
	    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "" FORCE)
	  endif()
	endif()
	set(CMAKE_CXX_STANDARD 17)
	set(CMAKE_CXX_STANDARD_REQUIRED ON)
	set(CMAKE_CXX_EXTENSIONS OFF)
	if(CMAKE_BUILD_TYPE STREQUAL "Release" )
		SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -Wfatal-errors -std=c++17 -Wno-format-y2k -fpermissive -m64 -arch x86_64 -Wimplicit-int-conversion -std=gnu++0x -fPIC -fpermissive -Wno-unused-function -O3 -Wall ")
		add_definitions(-D__APPLE__ -D_GLIBCXX_USE_CXX11_ABI=1 -D_FILE_OFFSET_BITS=64 -DNDEBUG)
		message("release build")
	else()
		SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -Wfatal-errors -std=c++17 -Wno-format-y2k -Wimplicit-int-conversion -fpermissive -m64 -arch x86_64 -std=gnu++0x -Wdeprecated-declarations -Wunused-function -fPIC -fpermissive -Wno-unused-function -O0 -Wall -g -ggdb ")
		add_definitions(-D__APPLE__ -D_GLIBCXX_USE_CXX11_ABI=1 -D_FILE_OFFSET_BITS=64 -DDEBUG)
		message("debug build")
	endif()

	
	# TODO: 如有需要，请添加测试并安装目标。
	set(CMAKE_INSTALL_PREFIX /usr/local/opt/cpps)

elseif (LINUX)
	set(CMAKE_CXX_STANDARD 17)
	set(CMAKE_CXX_STANDARD_REQUIRED ON)
	set(CMAKE_CXX_EXTENSIONS OFF)
	#default release.
	if (NOT EXISTS ${CMAKE_BINARY_DIR}/CMakeCache.txt)
	  if (NOT CMAKE_BUILD_TYPE)
	    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "" FORCE)
	  endif()
	endif()

	if(CMAKE_BUILD_TYPE STREQUAL "Release" )
		SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -Wfatal-errors -std=c++17 -Wno-format-y2k -fpermissive -m64 -std=gnu++0x -fPIC -fpermissive -Wno-unused-function -O3 -Wall")
		add_definitions(-DLINUX -D_GLIBCXX_USE_CXX11_ABI=1 -D_FILE_OFFSET_BITS=64 -DNDEBUG)
		message("release build")
	else()
		SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -Wfatal-errors -std=c++17 -Wno-format-y2k -fpermissive -m64 -std=gnu++0x -fPIC -fpermissive -Wno-unused-function -O0 -Wall -g -ggdb")
		add_definitions(-DLINUX -D_GLIBCXX_USE_CXX11_ABI=1 -D_FILE_OFFSET_BITS=64 -DDEBUG)
		message("debug build")
	endif()
	
	# TODO: 如有需要，请添加测试并安装目标。
	set(CMAKE_INSTALL_PREFIX /usr/local)
ENDIF()


# 包含子项目。
add_subdirectory ("src")
add_subdirectory ("cpps")
add_subdirectory ("example/demo")
add_subdirectory ("example/test")