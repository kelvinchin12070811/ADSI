cmake_minimum_required(VERSION 3.18)
set(PROJECT_NAME "ADISEncryptor")
project(${PROJECT_NAME})

set(CMAKE_AUTOMOC  on)
set(CMAKE_AUTORCC on)
set(CMAKE_AUTOUIC on)
set(CXX_STANDARD 17)

find_package(Qt5 COMPONENTS
    Widgets
REQUIRED)

set(PROJECT_UI_FILES
    "src/window/mainwindow/MainWindow.ui"
)

set(PROJECT_SOURCE_FILES
    "src/Main.cpp"
    "src/window/mainwindow/MainWindow.cpp"
)

set(PROJECT_HEADER_FILES
    "src/window/mainwindow/MainWindow.hpp"
)

add_executable(${PROJECT_NAME} WIN32
     ${PROJECT_SOURCE_FILES}
     ${PROJECT_HEADER_FILES}
     ${PROJECT_UI_FILES}
     "Resource.qrc"
)

target_link_libraries(
     ${PROJECT_NAME}
     Qt5::Widgets
)