project(repo-converter)

cmake_minimum_required(VERSION 2.8)

add_library(libPocoFoundation SHARED IMPORTED)
add_library(libcurl SHARED IMPORTED)
add_library(libboost_program_options SHARED IMPORTED)

set_property(TARGET libPocoFoundation PROPERTY IMPORTED_LOCATION /usr/lib64/libPocoFoundation.so)
set_property(TARGET libcurl PROPERTY IMPORTED_LOCATION /usr/lib64/libcurl.so)
set_property(TARGET libboost_program_options PROPERTY IMPORTED_LOCATION /usr/lib64/libboost_program_options.so.1.42.0)

add_executable(repo-converter utils.cpp repository.cpp main.cpp)

target_link_libraries(repo-converter libPocoFoundation)
target_link_libraries(repo-converter libcurl)
target_link_libraries(repo-converter libboost_program_options)
