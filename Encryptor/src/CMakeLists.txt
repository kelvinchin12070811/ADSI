set(PROJECT_NAME "ADISEncryptor")
project(${PROJECT_NAME})

set(CMAKE_AUTOMOC  on)
set(CMAKE_AUTORCC on)
set(CMAKE_AUTOUIC on)
set(CMAKE_INCLUDE_CURRENT_DIR on)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_CRT_SECURE_NO_WARNINGS")
set(LIB_LIBBCRYPT_INC_PATH "" CACHE PATH "Include path of libbcrypt")
set(LIB_LIBBCRYPT_LIB_PATH "" CACHE FILEPATH "Lib file of libbcrypt")

if("${LIB_LIBBCRYPT_INC_PATH}" STREQUAL "" OR
   "${LIB_LIBBCRYPT_LIB_PATH}" STREQUAL "")
    message(FATAL_ERROR "Libraries path does not specified!")
endif()

find_package(Qt5 COMPONENTS
    Core
    Gui
    Widgets
REQUIRED)

find_package(Boost COMPONENTS REQUIRED)
find_package(cryptopp CONFIG REQUIRED)
find_package(yaml-cpp CONFIG REQUIRED)
find_package(unofficial-sqlite3 CONFIG REQUIRED)
find_package(SqliteOrm CONFIG REQUIRED)
find_package(KF5WidgetsAddons CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)

include_directories(${Boost_INCLUDE_DIR})
include_directories(${LIB_LIBBCRYPT_INC_PATH})

get_target_property(QMAKE_BIN Qt5::qmake IMPORTED_LOCATION)
get_filename_component(QMAKE_BIN_DIR "${QMAKE_BIN}" DIRECTORY)
find_program(DEPLOYQT_BIN NAMES windeployqt HINTS "${QMAKE_BIN_DIR}")

set(PROJECT_UI_FILES
    "window/authorinfoeditor/AuthorDetailsEditor.ui"
    "window/authorinfoeditor/AuthorInfoEditor.ui"
    "window/mainwindow/MainWindow.ui"
    "window/setting/Setting.ui"
)

set(PROJECT_SOURCE_FILES
    "codec/AESDecoderCodec.cpp"
    "codec/AESEncoderCodec.cpp"
    "codec/Base64DecoderCodec.cpp"
    "codec/Base64EncoderCodec.cpp"
    "codec/DefaultCodecFactory.cpp"
    "codec/RSASignEncoderCodec.cpp"
    "codec/SHA3EncoderCodec.cpp"
    "components/ImagePreview.cpp"
    "components/Switch.cpp"
    "db/DBManager.cpp"
    "generator/AESCryptoKeyGenerator.cpp"
    "generator/DefaultCryptoKeyGeneratorFactory.cpp"
    "generator/PrivateRSACryptoKeyGenerator.cpp"
    "generator/PublicRSACryptoKeyGenerator.cpp"
    "generator/RSACryptoKeyGeneratorBase.cpp"
    "Main.cpp"
    "utils/ConfigManager.cpp"
    "utils/DCT.cpp"
    "utils/StylesManager.cpp"
    "window/authorinfoeditor/AuthorDetailsEditor.cpp"
    "window/authorinfoeditor/AuthorInfoEditor.cpp"
    "window/mainwindow/MainWindow.cpp"
    "window/setting/Setting.cpp"
)

set(PROJECT_HEADER_FILES
    "codec/AESDecoderCodec.hpp"
    "codec/AESEncoderCodec.hpp"
    "codec/Base64DecoderCodec.hpp"
    "codec/Base64EncoderCodec.hpp"
    "codec/DefaultCodecFactory.hpp"
    "codec/ICodec.hpp"
    "codec/ICodecFactory.hpp"
    "codec/RSASignEncoderCodec.hpp"
    "codec/SHA3EncoderCodec.hpp"
    "components/ImagePreview.hpp"
    "components/Switch.hpp"
    "db/data/Author.hpp"
    "db/data/KeyStore.hpp"
    "db/DBManager.hpp"
    "generator/AESCryptoKeyGenerator.hpp"
    "generator/DefaultCryptoKeyGeneratorFactory.hpp"
    "generator/ICryptoKeyGenerator.hpp"
    "generator/ICryptoKeyGeneratorFactory.hpp"
    "generator/PrivateRSACryptoKeyGenerator.hpp"
    "generator/PublicRSACryptoKeyGenerator.hpp"
    "generator/RSACryptoKeyGeneratorBase.hpp"
    "utils/ConfigManager.hpp"
    "utils/DCT.hpp"
    "utils/StylesManager.hpp"
    "window/authorinfoeditor/AuthorDetailsEditor.hpp"
    "window/authorinfoeditor/AuthorInfoEditor.hpp"
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
     ${LIB_LIBBCRYPT_LIB_PATH}
     cryptopp-static
     fmt::fmt
     KF5::WidgetsAddons
     Qt5::Core
     Qt5::Gui
     Qt5::Widgets
     sqlite_orm::sqlite_orm
     unofficial::sqlite3::sqlite3
     yaml-cpp
)
