# CMake Version
cmake_minimum_required (VERSION 3.15)

# Enable ASM
enable_language (ASM_MASM)

# Project Name
project (libwthread VERSION 1.0.0)

# MSVC Only
if (NOT MSVC)
	message (FATAL_ERROR "Current project working on MSVC compiler only!")
endif (NOT MSVC)

# Default Build With Release
if (NOT DEFINED CMAKE_BUILD_TYPE)
    set (CMAKE_BUILD_TYPE "Release"  CACHE STRING "Choose the type of build." FORCE)
endif (NOT DEFINED CMAKE_BUILD_TYPE)

# Set Language Version
set (CMAKE_CXX_STANDARD 20)# Using C++20
set (CMAKE_CXX_EXTENSIONS OFF)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_C_STANDARD 17)# Using C17
set (CMAKE_C_EXTENSIONS OFF)
set (CMAKE_C_STANDARD_REQUIRED ON)

# Get Source Files
file (GLOB_RECURSE LIBWTHREAD_SRC CONFIGURE_DEPENDS "src/*.cpp" "src/*.c" "src/*.asm")
file (GLOB_RECURSE LIBWTHREAD_INC CONFIGURE_DEPENDS "include/*.hpp" "include/*.h")
foreach (ITEM ${LIBWTHREAD_INC})
	set_source_files_properties ("${ITEM}" PROPERTIES HEADER_FILE_ONLY TRUE)
endforeach (ITEM ${LIBWTHREAD_INC})

# Main Target
add_library (libwthread SHARED ${LIBWTHREAD_INC} ${LIBWTHREAD_SRC})
add_library (libwthread-static STATIC ${LIBWTHREAD_INC} ${LIBWTHREAD_SRC})

# Include Dir
target_include_directories (libwthread PUBLIC "include")
target_include_directories (libwthread-static PUBLIC "include")

# Symbol Export
target_compile_definitions (libwthread PUBLIC "WTHREAD_BUILD_DLL=1")
target_compile_definitions (libwthread PRIVATE "WTHREAD_DLL=1")

# Using MD For VCRT
set_target_properties (libwthread libwthread-static
	PROPERTIES MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")

# Test Context
add_executable (test-context "test/TestContext.c")
target_link_libraries (test-context PRIVATE libwthread-static)
target_compile_options (test-context PRIVATE /Od)
set_target_properties (test-context
	PROPERTIES MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")

# Test Thread
add_executable (test-thread "test/TestThread.cpp")
target_link_libraries (test-thread PRIVATE libwthread)
set_target_properties (test-thread
	PROPERTIES MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")

# Test Exit
add_executable (test-exit "test/TestExit.cpp")
target_link_libraries (test-exit PRIVATE libwthread-static)
set_target_properties (test-exit
	PROPERTIES MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")

# Output Path
set_target_properties (libwthread libwthread-static test-context test-thread test-exit
	PROPERTIES
	ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
	LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# All Tests
enable_testing ()
add_test (NAME "Test-Context" COMMAND $<TARGET_FILE:test-context>)
add_test (NAME "Test-Thread" COMMAND $<TARGET_FILE:test-thread>)
add_test (NAME "Test-Exit" COMMAND $<TARGET_FILE:test-exit>)