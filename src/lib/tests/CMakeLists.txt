cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

project(lib_tests VERSION 0.1 LANGUAGES CXX)

find_package(Catch2 CONFIG REQUIRED)

file(GLOB_RECURSE TARGET_SOURCES
    src/*.cpp)
file(GLOB_RECURSE TARGET_HEADERS
    src/*.hpp
    src/*.h)
    
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES
    ${TARGET_SOURCES}
    ${TARGET_HEADERS})
    
add_executable(${PROJECT_NAME}
    ${TARGET_SOURCES}
    ${TARGET_HEADERS})

unset(TARGET_SOURCES)
unset(TARGET_HEADERS)

target_include_directories(${PROJECT_NAME}
    PRIVATE src)
    
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        barmaley::lib
        Catch2::Catch2)