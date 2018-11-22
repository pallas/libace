cmake_minimum_required(VERSION 3.1)

set(default_build_type "Release")
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
	set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE
		STRING "Choose the type of build." FORCE)
	set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
		"Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

project(libace)
add_library(libace INTERFACE)

file(GLOB header_files "*.h")
install(FILES ${header_files}
	DESTINATION "include/lace"
	)

enable_testing()
add_executable(test_divide test_divide.cc)
add_test(NAME test_divide COMMAND $<TARGET_FILE:test_divide>)
