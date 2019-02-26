
cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

project(esp_watcher VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include("utils.cmake")

add_subdirectory(gui)
add_subdirectory(lib)


