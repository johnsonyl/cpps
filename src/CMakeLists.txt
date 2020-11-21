# CMakeList.txt: libcpps 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)

# 将源代码添加到此项目的可执行文件。
AUX_SOURCE_DIRECTORY(${PROJECT_SOURCE_DIR}/src  SRC_LIST)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include .)

IF (CMAKE_SYSTEM_NAME MATCHES "Windows")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/bin)


set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/bin)

ELSE (CMAKE_SYSTEM_NAME MATCHES "Windows")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

ENDIF (CMAKE_SYSTEM_NAME MATCHES "Windows")

ADD_LIBRARY (cpps STATIC ${SRC_LIST})

IF (CMAKE_SYSTEM_NAME MATCHES "Windows")
ELSE (CMAKE_SYSTEM_NAME MATCHES "Windows")

	target_link_libraries(${PROJECT_NAME} -lrt -ldl -lm -lpthread)
		
	set(head_files
	${PROJECT_SOURCE_DIR}/include/cpps.h
	${PROJECT_SOURCE_DIR}/include/coroutine.h
	${PROJECT_SOURCE_DIR}/include/cpps_array.h
	${PROJECT_SOURCE_DIR}/include/cpps_base.h
	${PROJECT_SOURCE_DIR}/include/cpps_buffer.h
	${PROJECT_SOURCE_DIR}/include/cpps_call_function.h
	${PROJECT_SOURCE_DIR}/include/cpps_cast.h
	${PROJECT_SOURCE_DIR}/include/cpps_cat.h
	${PROJECT_SOURCE_DIR}/include/cpps_class.h
	${PROJECT_SOURCE_DIR}/include/cpps_classvar.h
	${PROJECT_SOURCE_DIR}/include/cpps_converter.h
	${PROJECT_SOURCE_DIR}/include/cpps_cppsfunction.h
	${PROJECT_SOURCE_DIR}/include/cpps_cstate.h
	${PROJECT_SOURCE_DIR}/include/cpps_debug.h
	${PROJECT_SOURCE_DIR}/include/cpps_def.h
	${PROJECT_SOURCE_DIR}/include/cpps_dofunction.h
	${PROJECT_SOURCE_DIR}/include/cpps_domain.h
	${PROJECT_SOURCE_DIR}/include/cpps_enum_param.h
	${PROJECT_SOURCE_DIR}/include/cpps_error.h
	${PROJECT_SOURCE_DIR}/include/cpps_function.h
	${PROJECT_SOURCE_DIR}/include/cpps_gc.h
	${PROJECT_SOURCE_DIR}/include/cpps_gcobject.h
	${PROJECT_SOURCE_DIR}/include/cpps_io.h
	${PROJECT_SOURCE_DIR}/include/cpps_is_void.h
	${PROJECT_SOURCE_DIR}/include/cpps_jit_compiler.h
	${PROJECT_SOURCE_DIR}/include/cpps_lock.h
	${PROJECT_SOURCE_DIR}/include/cpps_make_function.h
	${PROJECT_SOURCE_DIR}/include/cpps_make_vector.h
	${PROJECT_SOURCE_DIR}/include/cpps_map.h
	${PROJECT_SOURCE_DIR}/include/cpps_math.h
	${PROJECT_SOURCE_DIR}/include/cpps_module.h
	${PROJECT_SOURCE_DIR}/include/cpps_newptr.h
	${PROJECT_SOURCE_DIR}/include/cpps_node.h
	${PROJECT_SOURCE_DIR}/include/cpps_object.h
	${PROJECT_SOURCE_DIR}/include/cpps_reg.h
	${PROJECT_SOURCE_DIR}/include/cpps_reg_cpps_class.h
	${PROJECT_SOURCE_DIR}/include/cpps_regfunction.h
	${PROJECT_SOURCE_DIR}/include/cpps_regfunctionfmt.h
	${PROJECT_SOURCE_DIR}/include/cpps_regvar.h
	${PROJECT_SOURCE_DIR}/include/cpps_string.h
	${PROJECT_SOURCE_DIR}/include/cpps_symbol.h
	${PROJECT_SOURCE_DIR}/include/cpps_thread.h
	${PROJECT_SOURCE_DIR}/include/cpps_time.h
	${PROJECT_SOURCE_DIR}/include/cpps_trycatch_error.h
	${PROJECT_SOURCE_DIR}/include/cpps_value.h
	${PROJECT_SOURCE_DIR}/include/cpps_vector.h
	${PROJECT_SOURCE_DIR}/include/libcpps.h
	)
	install(FILES ${head_files} DESTINATION include)

	install(TARGETS cpps
	RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib)


ENDIF (CMAKE_SYSTEM_NAME MATCHES "Windows")
