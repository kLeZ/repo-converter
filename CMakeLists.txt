project(repo-converter)

cmake_minimum_required(VERSION 2.8)

add_library(libPocoFoundation SHARED IMPORTED)
add_library(libcurl SHARED IMPORTED)

set_property(TARGET libPocoFoundation PROPERTY IMPORTED_LOCATION /usr/lib64/libPocoFoundation.so)
set_property(TARGET libcurl PROPERTY IMPORTED_LOCATION /usr/lib64/libcurl.so)

add_executable(repo-converter anyoption.cpp utils.cpp repository.cpp main.cpp)

target_link_libraries(repo-converter libPocoFoundation)
target_link_libraries(repo-converter libcurl)