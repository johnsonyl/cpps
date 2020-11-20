# CMakeList.txt: 顶层 CMake 项目文件，在此处执行全局配置
# 并包含子项目。
#
cmake_minimum_required (VERSION 3.8)

project ("cpps")


IF (CMAKE_SYSTEM_NAME MATCHES "Windows")
ELSE (CMAKE_SYSTEM_NAME MATCHES "Windows")
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wfatal-errors -std=c++17 -Wno-format-y2k -fpermissive -std=gnu++0x -fPIC -fpermissive" )
	add_definitions(-DLINUX -D_FILE_OFFSET_BITS=64)
ENDIF(CMAKE_SYSTEM_NAME MATCHES "Windows")

# TODO: 如有需要，请添加测试并安装目标。
set(CMAKE_INSTALL_PREFIX /usr)

# 包含子项目。
add_subdirectory ("src")
add_subdirectory ("example/demo")
add_subdirectory ("example/cppsc")
add_subdirectory ("lib/curl")
add_subdirectory ("lib/json")
add_subdirectory ("lib/socket")
