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

set(${PROJECT_NAME}_EXECUTABLES
	deal
	)

set(${PROJECT_NAME}_HEADERS
	compare
	container_of
	divider
	do_not_copy
	hash
	haystack
	named_error
	objectstack
	pick
	random
	ropebuf
	scoped
	singleton
	try
	welford
	)

set(${PROJECT_NAME}_TESTS
	test_comparable
	test_divide
	test_hash
	test_haystack
	test_objectstack
	test_random
	test_ropebuf
	)

foreach(executable ${${PROJECT_NAME}_EXECUTABLES})
	add_executable(${executable} ${executable}.cc)
	install(TARGETS ${executable} RUNTIME DESTINATION bin)
endforeach(executable)

foreach(header ${${PROJECT_NAME}_HEADERS})
	install(FILES ${header}.h DESTINATION "include/lace")
endforeach(header)

enable_testing()
add_custom_target(all_tests)
foreach(test ${${PROJECT_NAME}_TESTS})
	add_executable(${test} EXCLUDE_FROM_ALL ${test}.cc)
	add_test(NAME ${test} COMMAND $<TARGET_FILE:${test}>)
	add_dependencies(all_tests ${test})
endforeach(test)
build_command(CTEST_CUSTOM_PRE_TEST TARGET all_tests)
string(CONFIGURE \"@CTEST_CUSTOM_PRE_TEST@\" CTEST_CUSTOM_PRE_TEST_QUOTED ESCAPE_QUOTES)
file(WRITE "${CMAKE_BINARY_DIR}/CTestCustom.cmake" "set(CTEST_CUSTOM_PRE_TEST ${CTEST_CUSTOM_PRE_TEST_QUOTED})" "\n")
