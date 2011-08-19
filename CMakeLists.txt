project(repo-converter)

cmake_minimum_required(VERSION 2.8)

add_library(libPocoFoundation STATIC IMPORTED)

set_property(TARGET libPocoFoundation PROPERTY IMPORTED_LOCATION /usr/lib64/libPocoFoundation.so)

add_executable(repo-converter anyoption.cpp utils.cpp repository.cpp main.cpp)

target_link_libraries(repo-converter libPocoFoundation)
