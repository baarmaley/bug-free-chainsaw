cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

project(lib VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)

find_package(Qt5Core REQUIRED)
find_package(Qt5Network REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(Boost)

file(GLOB_RECURSE TARGET_SOURCES
    src/*.cpp)
file(GLOB_RECURSE TARGET_PRIVATE_HEADERS
    src/*.hpp
    src/*.h)

file(GLOB_RECURSE TARGET_PUBLIC_HEADERS
     include/*.hpp
     include/*.h)
     
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES
    ${TARGET_SOURCES}
    ${TARGET_PRIVATE_HEADERS}
    ${TARGET_PUBLIC_HEADERS})
    
add_library(${PROJECT_NAME}
    ${TARGET_SOURCES}
    ${TARGET_PRIVATE_HEADERS}
    ${TARGET_PUBLIC_HEADERS})

add_library(barmaley::lib ALIAS ${PROJECT_NAME})
    
unset(TARGET_SOURCES)
unset(TARGET_PRIVATE_HEADERS)
unset(TARGET_PUBLIC_HEADERS)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        src
    PUBLIC
        include)

target_link_libraries(${PROJECT_NAME} 
    PUBLIC
        Qt5::Core
        Qt5::Network
		Boost::boost
        nlohmann_json::nlohmann_json)

add_subdirectory(tests)
