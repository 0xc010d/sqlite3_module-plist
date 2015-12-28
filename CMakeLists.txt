cmake_minimum_required(VERSION 3.3)

project(sqlite3_module_plist)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")

add_definitions(
    -std=c++11
    -fno-rtti
    -fno-exceptions
    -Wall
    -Wextra
    -fobjc-arc
#    -Werror
)

set(CMAKE_BUILD_TYPE Debug)

set(SOURCE_FILES
    Module.cpp
    Cell.cpp
    Plist.mm
    PlistTable.cpp
    PlistCursor.cpp
    )

set(SHARED_LIBRARY_NAME Sqlite3ModulePlist)
message(STATUS "Shared library: ${SHARED_LIBRARY_NAME}")

string(REGEX REPLACE "([a-z0-9])([A-Z])" "\\1_\\2" STATIC_LIBRARY_NAME ${SHARED_LIBRARY_NAME})
string(TOLOWER ${STATIC_LIBRARY_NAME} STATIC_LIBRARY_NAME)
message(STATUS "Static library: ${STATIC_LIBRARY_NAME}")

string(TOLOWER ${SHARED_LIBRARY_NAME} SQLITE3_EXTENSION_NAME)
string(REGEX REPLACE "[0-9]" "" SQLITE3_EXTENSION_NAME ${SQLITE3_EXTENSION_NAME})
message(STATUS "Extension name: ${SQLITE3_EXTENSION_NAME}")

set(SQLITE3_MODULE_NAME PLIST)

configure_file(config.h.in config.h)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_library(${STATIC_LIBRARY_NAME} ${SOURCE_FILES})
target_link_libraries(${STATIC_LIBRARY_NAME} sqlite3 "-framework Foundation")
include_directories(${PROJECT_SOURCE_DIR})

add_library(${SHARED_LIBRARY_NAME} SHARED main.cpp)
target_link_libraries(${SHARED_LIBRARY_NAME} ${STATIC_LIBRARY_NAME})

#add_custom_command(
#    TARGET ${SHARED_LIBRARY_NAME}
#    POST_BUILD COMMAND dsymutil ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/lib${SHARED_LIBRARY_NAME}.dylib
#)

enable_testing()
add_subdirectory(tests)
