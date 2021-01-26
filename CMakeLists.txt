cmake_minimum_required(VERSION 3.18)
project("ADSI")

set(CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")

add_subdirectory("Encryptor/src")