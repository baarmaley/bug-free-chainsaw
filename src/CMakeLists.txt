cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

project(esp_watcher VERSION 0.1 LANGUAGES CXX)

option(ENABLE_CONAN "use conan" ON)

if(${ENABLE_CONAN})
    set(CMAKE_SKIP_RPATH 1)
    include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    conan_basic_setup(KEEP_RPATHS)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include("utils.cmake")

add_subdirectory(gui)
add_subdirectory(lib)
add_subdirectory(qml_gui)



