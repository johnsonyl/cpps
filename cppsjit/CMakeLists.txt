# CMakeList.txt: libcpps 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.5)

project(cppsjit)

# 将源代码添加到此项目的可执行文件。
message(${PROJECT_SOURCE_DIR})

if(WINDOWS)
	INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/../include ${PROJECT_SOURCE_DIR}/../deps/deps/include)
	LINK_DIRECTORIES(${PROJECT_SOURCE_DIR}/../lib ${PROJECT_SOURCE_DIR}/../deps/deps/lib)
ELSEIF(MACOS)
	INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/../include /usr/include /usr/local/include /usr/local/opt/llvm/include)
	LINK_DIRECTORIES(${PROJECT_SOURCE_DIR}/../lib  /lib /usr/lib /usr/local/lib /usr/lib64 )
elseif(LINUX)
	INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/../include /usr/local/include)
	LINK_DIRECTORIES(${PROJECT_SOURCE_DIR}/../lib /lib /usr/lib)
endif()

LINK_DIRECTORIES(${PROJECT_SOURCE_DIR}/../bin)

AUX_SOURCE_DIRECTORY(${PROJECT_SOURCE_DIR}  SRC_LIST)

file(GLOB_RECURSE CURRENT_HEADERS  *.h)
source_group("Include" FILES ${CURRENT_HEADERS}) 


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



ADD_LIBRARY (${PROJECT_NAME} SHARED ${SRC_LIST} ${CURRENT_HEADERS})

IF(WINDOWS)

add_definitions(-D_CRT_SECURE_NO_DEPRECATE)
add_definitions(-D_CRT_SECURE_NO_WARNINGS)
add_definitions(-D_CRT_NONSTDC_NO_DEPRECATE)
add_definitions(-D_CRT_NONSTDC_NO_WARNINGS)
add_definitions(-D_SCL_SECURE_NO_DEPRECATE)
add_definitions(-D_SCL_SECURE_NO_WARNINGS)
add_definitions(-DUNICODE)
add_definitions(-D_UNICODE)
add_definitions(-D__STDC_CONSTANT_MACROS)
add_definitions(-D__STDC_FORMAT_MACROS)
add_definitions(-D__STDC_LIMIT_MACROS)
add_definitions(-DNOMINMAX)




target_link_libraries(cppsjit libcpps
LLVMWindowsManifest.lib
LLVMXRay.lib
LLVMLibDriver.lib
LLVMDlltoolDriver.lib
LLVMCoverage.lib
LLVMLineEditor.lib
LLVMX86TargetMCA.lib
LLVMX86Disassembler.lib
LLVMX86AsmParser.lib
LLVMX86CodeGen.lib
LLVMX86Desc.lib
LLVMX86Info.lib
LLVMOrcJIT.lib
LLVMWindowsDriver.lib
LLVMMCJIT.lib
LLVMJITLink.lib
LLVMInterpreter.lib
LLVMExecutionEngine.lib
LLVMRuntimeDyld.lib
LLVMOrcTargetProcess.lib
LLVMOrcShared.lib
LLVMDWP.lib
LLVMDebugInfoLogicalView.lib
LLVMDebugInfoGSYM.lib
LLVMOption.lib
LLVMObjectYAML.lib
LLVMObjCopy.lib
LLVMMCA.lib
LLVMMCDisassembler.lib
LLVMLTO.lib
LLVMPasses.lib
LLVMCFGuard.lib
LLVMCoroutines.lib
LLVMipo.lib
LLVMVectorize.lib
LLVMLinker.lib
LLVMInstrumentation.lib
LLVMFrontendOpenMP.lib
LLVMFrontendOpenACC.lib
LLVMFrontendHLSL.lib
LLVMExtensions.lib
LLVMDWARFLinkerParallel.lib
LLVMDWARFLinker.lib
LLVMGlobalISel.lib
LLVMMIRParser.lib
LLVMAsmPrinter.lib
LLVMSelectionDAG.lib
LLVMCodeGen.lib
LLVMTarget.lib
LLVMObjCARCOpts.lib
LLVMCodeGenTypes.lib
LLVMIRPrinter.lib
LLVMInterfaceStub.lib
LLVMFileCheck.lib
LLVMFuzzMutate.lib
LLVMScalarOpts.lib
LLVMInstCombine.lib
LLVMAggressiveInstCombine.lib
LLVMTransformUtils.lib
LLVMBitWriter.lib
LLVMAnalysis.lib
LLVMProfileData.lib
LLVMSymbolize.lib
LLVMDebugInfoBTF.lib
LLVMDebugInfoPDB.lib
LLVMDebugInfoMSF.lib
LLVMDebugInfoDWARF.lib
LLVMObject.lib
LLVMTextAPI.lib
LLVMMCParser.lib
LLVMIRReader.lib
LLVMAsmParser.lib
LLVMMC.lib
LLVMDebugInfoCodeView.lib
LLVMBitReader.lib
LLVMFuzzerCLI.lib
LLVMCore.lib
LLVMRemarks.lib
LLVMBitstreamReader.lib
LLVMBinaryFormat.lib
LLVMTargetParser.lib
LLVMTableGen.lib
LLVMSupport.lib
LLVMDemangle.lib)
ELSE()
target_link_libraries(cppsjit libcpps)
ENDIF()

