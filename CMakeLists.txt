cmake_minimum_required(VERSION 3.10)
project(CFARDetector LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Чтобы uic находил mainwindow.ui (он в корне, а mainwindow.cpp в src/)
set(CMAKE_AUTOUIC_SEARCH_PATHS
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

find_package(Qt5 REQUIRED COMPONENTS Widgets)

add_executable(CFARDetector
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/src/mainwindow.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/mainwindow.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/src/cfardetector.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/cfardetector.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/mainwindow.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/resources.qrc
)

target_link_libraries(CFARDetector PRIVATE Qt5::Widgets)

target_include_directories(CFARDetector PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Удобно для запуска из IDE: рабочая папка = корень проекта (видно Tests/)
set_target_properties(CFARDetector PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
)

# (опционально) прячем консоль на Windows. Убери, если хочешь видеть stdout.
if (WIN32)
    set_target_properties(CFARDetector PROPERTIES WIN32_EXECUTABLE ON)
endif()
