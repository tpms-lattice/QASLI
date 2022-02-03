cmake_minimum_required(VERSION 3.5)
project(QASLI LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core)
find_package(QT NAMES Qt6 Qt5 COMPONENTS Gui REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Gui)
find_package(QT NAMES Qt6 Qt5 COMPONENTS Xml REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Xml)
find_package(QT NAMES Qt6 Qt5 COMPONENTS 3DCore REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS 3DCore)
find_package(QT NAMES Qt6 Qt5 COMPONENTS 3DRender REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS 3DRender)
find_package(QT NAMES Qt6 Qt5 COMPONENTS 3DInput REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS 3DInput)
find_package(QT NAMES Qt6 Qt5 COMPONENTS 3DExtras REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS 3DExtras)
find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

add_executable(QASLI WIN32 MACOSX_BUNDLE
    src/global.h
    src/about.ui
    src/mainwindow.ui
    src/FileSystemPathUtils.cpp src/FileSystemPathUtils.h
    src/about.cpp src/about.h
    src/main.cpp
    src/mainwindow.cpp src/mainwindow.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/icon.rc
)

target_include_directories(QASLI PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_definitions(QASLI PUBLIC
    APP_VERSION=\\\"0.1\\\"
)

target_link_libraries(QASLI PRIVATE Qt${QT_VERSION_MAJOR}::3DCore)
target_link_libraries(QASLI PRIVATE Qt${QT_VERSION_MAJOR}::3DExtras)
target_link_libraries(QASLI PRIVATE Qt${QT_VERSION_MAJOR}::3DInput)
target_link_libraries(QASLI PRIVATE Qt${QT_VERSION_MAJOR}::3DRender)
target_link_libraries(QASLI PRIVATE Qt${QT_VERSION_MAJOR}::Core)
target_link_libraries(QASLI PRIVATE Qt${QT_VERSION_MAJOR}::Gui)
target_link_libraries(QASLI PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(QASLI PRIVATE Qt${QT_VERSION_MAJOR}::Xml)

install(TARGETS QASLI
    RUNTIME DESTINATION "${CMAKE_INSTALL_PREFIX}")
