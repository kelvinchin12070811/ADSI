cmake_minimum_required(VERSION 3.18)
project(ADSIEncryptorUnitTest)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED 17)

include_directories(../../Encryptor/src)

find_package(Boost COMPONENTS unit_test_framework REQUIRED)
find_package(cryptopp CONFIG REQUIRED)

include_directories(${Boost_INCLUDE_DIR})

set(PROJECT_SOURCE_FILES
    "../../Encryptor/src/codec/AESDecoderCodec.cpp"
    "../../Encryptor/src/codec/AESEncoderCodec.cpp"
    "../../Encryptor/src/codec/SHA3EncoderCodec.cpp"
    "../../Encryptor/src/generator/AESCryptoKeyGenerator.cpp"
    "../../Encryptor/src/utils/DCT.cpp"
)

set(PROJECT_HEADER_FILES
    "../../Encryptor/src/codec/AESDecoderCodec.hpp"
    "../../Encryptor/src/codec/AESEncoderCodec.hpp"
    "../../Encryptor/src/codec/ICodec.hpp"
    "../../Encryptor/src/codec/SHA3EncoderCodec.hpp"
    "../../Encryptor/src/generator/AESCryptoKeyGenerator.hpp"
    "../../Encryptor/src/generator/ICryptoKeyGenerator.hpp"
    "../../Encryptor/src/utils/DCT.hpp"
)

add_library("Lib_${PROJECT_NAME}" ${PROJECT_SOURCE_FILES} ${PROJECT_HEADER_FILES})
add_executable(${PROJECT_NAME} Test.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE
    "Lib_${PROJECT_NAME}"
    cryptopp-static
    Boost::unit_test_framework
)

enable_testing()
add_test(
    NAME encryptor_test
    COMMAND $<TARGET_FILE:${PROJECT_NAME}>
)