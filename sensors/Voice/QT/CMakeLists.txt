cmake_minimum_required(VERSION 3.5)

project(Lida_service VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets PrintSupport REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets PrintSupport REQUIRED)
find_library(PIGPIO_LIBRARY NAMES pigpio REQUIRED)

file(GLOB_RECURSE Lidar_SDK_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/sdk/src/*.cpp")

set(PROJECT_SOURCES
    main.cpp
    widget.cpp
    widget.h
    widget.ui
    qcustomplot.cpp
    qcustomplot.h
    CppTimer.h
    sound.cpp
    sound.h
    DFbot_voi.cpp
    DFbot_voi.h
    ${Lidar_SDK_SOURCES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Lida_service
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(Lida_service SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(Lida_service
            ${PROJECT_SOURCES}
        )
    endif()
endif()

include_directories(sdk/include sdk/src)

target_link_libraries(Lida_service PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::PrintSupport ${PIGPIO_LIBRARY} i2c)

target_include_directories(Lida_service PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/sdk/include)
#target_include_directories(Lida_service PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/sdk/src)


set_target_properties(Lida_service PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Lida_service)
endif()
