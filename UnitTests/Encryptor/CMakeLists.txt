cmake_minimum_required(VERSION 3.18)
project(ADSIEncryptorUnitTest)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED 17)

include_directories(../../Encryptor/src)

find_package(Boost COMPONENTS unit_test_framework REQUIRED)
find_package(cryptopp CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(Qt5 COMPONENTS
    Core
    Gui
    Widgets
REQUIRED)
find_package(ZLIB REQUIRED)

include_directories(${Boost_INCLUDE_DIR})

set(PROJECT_SOURCE_FILES
    "../../Encryptor/src/codec/AESDecoderCodec.cpp"
    "../../Encryptor/src/codec/AESEncoderCodec.cpp"
    "../../Encryptor/src/codec/Base64DecoderCodec.cpp"
    "../../Encryptor/src/codec/Base64EncoderCodec.cpp"
    "../../Encryptor/src/codec/DefaultCodecFactory.cpp"
    "../../Encryptor/src/codec/DeflateCodec.cpp"
    "../../Encryptor/src/codec/ImageSignCodec.cpp"
    "../../Encryptor/src/codec/InflateCodec.cpp"
    "../../Encryptor/src/codec/RSASignEncoderCodec.cpp"
    "../../Encryptor/src/codec/SHA3EncoderCodec.cpp"
    "../../Encryptor/src/generator/AESCryptoKeyGenerator.cpp"
    "../../Encryptor/src/generator/DefaultCryptoKeyGeneratorFactory.cpp"
    "../../Encryptor/src/generator/PrivateRSACryptoKeyGenerator.cpp"
    "../../Encryptor/src/generator/PublicRSACryptoKeyGenerator.cpp"
    "../../Encryptor/src/generator/RSACryptoKeyGeneratorBase.cpp"
    "../../Encryptor/src/utils/DCT.cpp"
)

set(PROJECT_HEADER_FILES
    "../../Encryptor/src/codec/AESDecoderCodec.hpp"
    "../../Encryptor/src/codec/AESEncoderCodec.hpp"
    "../../Encryptor/src/codec/Base64DecoderCodec.hpp"
    "../../Encryptor/src/codec/Base64EncoderCodec.hpp"
    "../../Encryptor/src/codec/DefaultCodecFactory.hpp"
    "../../Encryptor/src/codec/DeflateCodec.hpp"
    "../../Encryptor/src/codec/ICodec.hpp"
    "../../Encryptor/src/codec/ICodec.hpp"
    "../../Encryptor/src/codec/ICodecFactory.hpp"
    "../../Encryptor/src/codec/ImageSignCodec.hpp"
    "../../Encryptor/src/codec/InflateCodec.hpp"
    "../../Encryptor/src/codec/RSASignEncoderCodec.hpp"
    "../../Encryptor/src/codec/SHA3EncoderCodec.hpp"
    "../../Encryptor/src/generator/AESCryptoKeyGenerator.hpp"
    "../../Encryptor/src/generator/DefaultCryptoKeyGeneratorFactory.hpp"
    "../../Encryptor/src/generator/ICryptoKeyGenerator.hpp"
    "../../Encryptor/src/generator/ICryptoKeyGeneratorFactory.hpp"
    "../../Encryptor/src/generator/PrivateRSACryptoKeyGenerator.hpp"
    "../../Encryptor/src/generator/PublicRSACryptoKeyGenerator.hpp"
    "../../Encryptor/src/generator/RSACryptoKeyGeneratorBase.hpp"
    "../../Encryptor/src/utils/DCT.hpp"
)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCE_FILES} ${PROJECT_HEADER_FILES} Test.cpp)
target_link_libraries(${PROJECT_NAME}
    Boost::unit_test_framework
    cryptopp-static
    fmt::fmt
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
    ZLIB::ZLIB
)

enable_testing()
add_test(
    NAME encryptor_test
    COMMAND $<TARGET_FILE:${PROJECT_NAME}>
)