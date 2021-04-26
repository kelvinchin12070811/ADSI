cmake_minimum_required(VERSION 3.18)
project("ADSI")

if(CMAKE_TOOLCHAIN_FILE)
    include(${CMAKE_TOOLCHAIN_FILE})
endif(CMAKE_TOOLCHAIN_FILE)

add_subdirectory("Encryptor/src")
add_subdirectory("Decryptor/src")
add_subdirectory("UnitTests/Encryptor")