set(PROJECT_NAME "ADISEncryptor")
project(${PROJECT_NAME})

set(CMAKE_AUTOMOC  on)
set(CMAKE_AUTORCC on)
set(CMAKE_AUTOUIC on)
set(CMAKE_INCLUDE_CURRENT_DIR on)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")

set(CRYPTOPP_INCLUDE_DIR "" CACHE PATH "Crypto++ include directory")
set(CRYPTOPP_LIB_DIR "" CACHE FILEPATH "Crypto++ library file")

find_package(Qt5 COMPONENTS
    Gui
    Qml
    Quick
REQUIRED)
find_package(Boost 1.70.0)
get_target_property(QMAKE_BIN Qt5::qmake IMPORTED_LOCATION)
get_filename_component(QMAKE_BIN_DIR "${QMAKE_BIN}" DIRECTORY)
find_program(DEPLOYQT_BIN NAMES windeployqt HINTS "${QMAKE_BIN_DIR}")

include_directories(${Boost_INCLUDE_DIR})
include_directories(${CRYPTOPP_INCLUDE_DIR})

set(PROJECT_UI_FILES
)

set(PROJECT_SOURCE_FILES
    "generator/AESCryptoKeyGenerator.cpp"
    "Main.cpp"
    "TestSubject.cpp"
    "utils/DCT.cpp"
)

set(PROJECT_HEADER_FILES
    "codec/ICodec.hpp"
    "generator/AESCryptoKeyGenerator.hpp"
    "generator/ICryptoKeyGenerator.hpp"
    "TestSubject.hpp"
    "utils/DCT.hpp"
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
     ${CRYPTOPP_LIB_DIR}
     Qt5::Gui
     Qt5::Qml
     Qt5::Quick
)