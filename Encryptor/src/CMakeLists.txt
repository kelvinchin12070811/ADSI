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

include_directories(${Boost_INCLUDE_DIR})

get_target_property(QMAKE_BIN Qt5::qmake IMPORTED_LOCATION)
get_filename_component(QMAKE_BIN_DIR "${QMAKE_BIN}" DIRECTORY)
find_program(DEPLOYQT_BIN NAMES windeployqt HINTS "${QMAKE_BIN_DIR}")

set(PROJECT_UI_FILES
    "window/mainwindow/MainWindow.ui"
)

set(PROJECT_SOURCE_FILES
    "codec/AESDecoderCodec.cpp"
    "codec/AESEncoderCodec.cpp"
    "codec/SHA3EncoderCodec.cpp"
    "generator/AESCryptoKeyGenerator.cpp"
    "Main.cpp"
    "TestSubject.cpp"
    "utils/DCT.cpp"
    "window/mainwindow/MainWindow.cpp"
)

set(PROJECT_HEADER_FILES
    "codec/AESDecoderCodec.hpp"
    "codec/AESEncoderCodec.hpp"
    "codec/ICodec.hpp"
    "codec/SHA3EncoderCodec.hpp"
    "generator/AESCryptoKeyGenerator.hpp"
    "generator/ICryptoKeyGenerator.hpp"
    "TestSubject.hpp"
    "utils/DCT.hpp"
    "window/mainwindow/MainWindow.hpp"
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

target_link_libraries(
     ${PROJECT_NAME}
     cryptopp-static
     Qt5::Core
     Qt5::Gui
     Qt5::Widgets
)
