cmake_minimum_required(VERSION 2.8)

add_subdirectory(../lib/googletest/googletest lib/googletest/googletest)
include_directories(${gtest_SOURCE_DIR}/include)

include_directories(${sqlite3_module_plist_SOURCE_DIR})

remove_definitions(
    -fno-exceptions
)

set(
    TEST_FILES
    CellTests.cpp
    PlistTests.cpp
    PlistTableTests.cpp TableTests.cpp)

#foreach (FILE ${TEST_FILES})
#  string(REGEX REPLACE "^(.+)Tests\\.cpp$" "validator-tests-\\1" TEST_NAME ${FILE})
#  add_executable(${TEST_NAME} main.cpp ${FILE})
#  target_link_libraries(${TEST_NAME} gtest_main JsonLint)
#  add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
#endforeach ()

add_executable(module-tests main.cpp ${TEST_FILES})
target_link_libraries(module-tests gtest_main sqlite3_module_plist)
add_test(NAME module-tests COMMAND module-tests)
