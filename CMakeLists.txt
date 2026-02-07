cmake_minimum_required(VERSION 3.10)
project(CFARDetector LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 REQUIRED COMPONENTS Widgets)

add_executable(CFARDetector
    main.cpp
    mainwindow.h
    mainwindow.cpp
    mainwindow.ui
    cfardetector.h
    cfardetector.cpp
    resources.qrc
)

target_link_libraries(CFARDetector PRIVATE Qt5::Widgets)

target_include_directories(CFARDetector PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

set_target_properties(CFARDetector PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
)

# (опционально) прячем консоль на Windows. Убери, если хочешь видеть stdout.
if (WIN32)
    set_target_properties(CFARDetector PROPERTIES WIN32_EXECUTABLE ON)
endif()
