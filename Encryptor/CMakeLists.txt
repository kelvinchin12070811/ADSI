cmake_minimum_required(VERSION 3.18)
set(PROJECT_NAME "ADISEncryptor")
project(${PROJECT_NAME} VERSION 1.0)

set(CXX_STANDARD 17)
set(AUTOMOC on)
set(AUTOUIC on)
set(AUTORCC on)

find_package(Qt5 COMPONENTS
 Widgets
REQUIRED)

set(PROJECT_SOURCE_FILES
 "src/Main.cpp"
)

set(PROJECT_HEADER_FILES
)

set(PROJECT_UI_FILES
)

add_executable(${PROJECT_NAME} WIN32
 ${PROJECT_HEADER_FILES}
 ${PROJECT_SOURCE_FILES}
 ${PROJECT_UI_FILES}
)

target_link_libraries(
 ${PROJECT_NAME}
 Qt5::Widgets
)