set(PROJECT_NAME "ADISEncryptor")
project(${PROJECT_NAME})

set(CMAKE_AUTOMOC  on)
set(CMAKE_AUTORCC on)
set(CMAKE_AUTOUIC on)
set(CMAKE_INCLUDE_CURRENT_DIR on)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_CRT_SECURE_NO_WARNINGS")

find_package(Qt5 COMPONENTS
    Core
    Gui
    Widgets
REQUIRED)

find_package(Boost COMPONENTS REQUIRED)
find_package(cryptopp CONFIG REQUIRED)
find_package(yaml-cpp CONFIG REQUIRED)

include_directories(${Boost_INCLUDE_DIR})

get_target_property(QMAKE_BIN Qt5::qmake IMPORTED_LOCATION)
get_filename_component(QMAKE_BIN_DIR "${QMAKE_BIN}" DIRECTORY)
find_program(DEPLOYQT_BIN NAMES windeployqt HINTS "${QMAKE_BIN_DIR}")

set(PROJECT_UI_FILES
    "window/mainwindow/MainWindow.ui"
    "window/setting/Setting.ui"
)

set(PROJECT_SOURCE_FILES
    "codec/AESDecoderCodec.cpp"
    "codec/AESEncoderCodec.cpp"
    "codec/RSASignEncoderCodec.cpp"
    "codec/SHA3EncoderCodec.cpp"
    "components/ImagePreview.cpp"
    "components/Switch.cpp"
    "generator/AESCryptoKeyGenerator.cpp"
    "generator/PrivateRSACryptoKeyGenerator.cpp"
    "generator/PublicRSACryptoKeyGenerator.cpp"
    "generator/RSACryptoKeyGeneratorBase.cpp"
    "Main.cpp"
    "utils/ConfigManager.cpp"
    "utils/DCT.cpp"
    "window/mainwindow/MainWindow.cpp"
    "window/setting/Setting.cpp"
)

set(PROJECT_HEADER_FILES
    "codec/AESDecoderCodec.hpp"
    "codec/AESEncoderCodec.hpp"
    "codec/ICodec.hpp"
    "codec/RSASignEncoderCodec.hpp"
    "codec/SHA3EncoderCodec.hpp"
    "components/ImagePreview.hpp"
    "components/Switch.hpp"
    "generator/AESCryptoKeyGenerator.hpp"
    "generator/ICryptoKeyGenerator.hpp"
    "generator/PrivateRSACryptoKeyGenerator.hpp"
    "generator/PublicRSACryptoKeyGenerator.hpp"
    "generator/RSACryptoKeyGeneratorBase.hpp"
    "utils/ConfigManager.hpp"
    "utils/DCT.hpp"
    "window/mainwindow/MainWindow.hpp"
    "window/setting/Setting.hpp"
)

if (WIN32)
    add_executable(${PROJECT_NAME} WIN32
         ${PROJECT_SOURCE_FILES}
         ${PROJECT_HEADER_FILES}
         ${PROJECT_UI_FILES}
         "../Resource.qrc"
     )

    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND
        ${DEPLOYQT_BIN} "$<TARGET_FILE:${PROJECT_NAME}>" --qmldir "${CMAKE_SOURCE_DIR}" --no-translations
    )
endif ()

if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
  target_compile_options(${PROJECT_NAME} PRIVATE "/ZI")
  target_link_options(${PROJECT_NAME} PRIVATE "/SAFESEH:NO")
endif()

target_link_libraries(
     ${PROJECT_NAME}
     cryptopp-static
     yaml-cpp
     Qt5::Core
     Qt5::Gui
     Qt5::Widgets
)
