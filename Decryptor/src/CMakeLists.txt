project("ADSIDecryptor")
set(CMAKE_AUTOMOC  on)
set(CMAKE_AUTORCC on)
set(CMAKE_AUTOUIC on)
set(CMAKE_INCLUDE_CURRENT_DIR on)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")
set(CMAKE_CXX_FLAGS "${CMAEK_CXX_FLAGS} -DADSI_DECRYPTOR")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_CRT_SECURE_NO_WARNINGS")

if(WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXXFLAGS} -DWIN32")
endif()

find_package(Qt5 COMPONENTS
    Core
    Gui
    Widgets
REQUIRED)

find_package(Boost COMPONENTS filesystem REQUIRED)
find_package(cryptopp CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(KF5WidgetsAddons CONFIG REQUIRED)
find_package(SqliteOrm CONFIG REQUIRED)
find_package(unofficial-sqlite3 CONFIG REQUIRED)
find_package(yaml-cpp CONFIG REQUIRED)
find_package(ZLIB REQUIRED)

include_directories(${Boost_INCLUDE_DIR})
include_directories("../../Encryptor/src")

get_target_property(QMAKE_BIN Qt5::qmake IMPORTED_LOCATION)
get_filename_component(QMAKE_BIN_DIR "${QMAKE_BIN}" DIRECTORY)
find_program(DEPLOYQT_BIN NAMES windeployqt HINTS "${QMAKE_BIN_DIR}")

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/../../PerceptualHashModule/perceptual_hash.py
    ${CMAKE_CURRENT_BINARY_DIR}/.scripts/perceptual_hash.py
)

set(PROJECT_UI_FILES
    "window/mainwindow/MainWindow.ui"
    "../../Encryptor/src/window/setting/Setting.ui"
)

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
    "../../Encryptor/src/components/ImagePreview.cpp"
    "../../Encryptor/src/components/Switch.cpp"
    "../../Encryptor/src/generator/AESCryptoKeyGenerator.cpp"
    "../../Encryptor/src/generator/DefaultCryptoKeyGeneratorFactory.cpp"
    "../../Encryptor/src/generator/PrivateRSACryptoKeyGenerator.cpp"
    "../../Encryptor/src/generator/PublicRSACryptoKeyGenerator.cpp"
    "../../Encryptor/src/generator/RSACryptoKeyGeneratorBase.cpp"
    "../../Encryptor/src/utils/ConfigManager.cpp"
    "../../Encryptor/src/utils/DCT.cpp"
    "../../Encryptor/src/utils/StylesManager.cpp"
    "../../Encryptor/src/window/setting/Setting.cpp"
    "Main.cpp"
    "window/mainwindow/MainWindow.cpp"
)

set(PROJECT_HEADER_FILES
    "window/mainwindow/MainWindow.hpp"
)

if (WIN32)
    add_executable(${PROJECT_NAME} WIN32
         ${PROJECT_SOURCE_FILES}
         ${PROJECT_HEADER_FILES}
         ${PROJECT_UI_FILES}
         "../../Encryptor/Resource.qrc"
     )

    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND
        ${DEPLOYQT_BIN} "$<TARGET_FILE:${PROJECT_NAME}>" --qmldir "${CMAKE_SOURCE_DIR}" --no-translations
    )

    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND
        pyinstaller --onefile --distpath "${CMAKE_CURRENT_BINARY_DIR}" "${CMAKE_CURRENT_BINARY_DIR}/.scripts/perceptual_hash.py"
    )
endif ()

if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
  target_compile_options(${PROJECT_NAME} PRIVATE "/ZI")
  target_link_options(${PROJECT_NAME} PRIVATE "/SAFESEH:NO")
endif()

target_link_libraries(
     ${PROJECT_NAME}
     ${LIB_LIBBCRYPT_LIB_PATH}
     Boost::boost
     Boost::filesystem
     cryptopp-static
     fmt::fmt
     KF5::WidgetsAddons
     Qt5::Core
     Qt5::Gui
     Qt5::Widgets
     yaml-cpp
     ZLIB::ZLIB
)