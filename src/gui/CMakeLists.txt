cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

project(gui VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
    
file(GLOB_RECURSE TARGET_SOURCES 
    src/*.cpp)
    
file(GLOB_RECURSE TARGET_HEADERS
    src/*.hpp
    src/*.h)
    
file(GLOB_RECURSE TARGET_QRC_FILES
    resources/*.qrc)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${TARGET_UI_FILES})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES
    ${TARGET_SOURCES}
    ${TARGET_HEADERS}
    ${TARGET_QRC_FILES}
    )
    
add_executable(${PROJECT_NAME} WIN32
    ${TARGET_SOURCES}
    ${TARGET_HEADERS}
    ${TARGET_QRC_FILES})

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        Qt5::Core
        Qt5::Widgets
        barmaley::lib)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        src)

add_windeploy_to_target(${PROJECT_NAME})

