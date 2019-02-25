cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(broadcast_receiver VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Network REQUIRED)

set(TARGET_NAME "broadcast_receiver")
set(TARGET_NAME_LIB "${TARGET_NAME}_lib")
    
file(GLOB_RECURSE TARGET_SOURCES 
	*.cpp)
    
file(GLOB_RECURSE TARGET_HEADERS
    *.hpp
    *.h)


source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${TARGET_UI_FILES})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES
    ${TARGET_SOURCES}
    ${TARGET_HEADERS}
    )

add_executable(${TARGET_NAME} WIN32
    ${TARGET_SOURCES}
    ${TARGET_HEADERS})

    
unset(TARGET_SOURCES)
unset(TARGET_HEADERS)

target_link_libraries(${TARGET_NAME}
    PUBLIC
		Qt5::Core
		Qt5::Widgets
		Qt5::Network)

target_include_directories(${TARGET_NAME}
    PUBLIC
        .)


if(WIN32)
	# Run winddeployqt if it can be found
    find_program(WINDEPLOYQT_EXECUTABLE NAMES windeployqt HINTS ${QTDIR} ENV QTDIR PATH_SUFFIXES bin)
    add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
    COMMAND ${WINDEPLOYQT_EXECUTABLE} $<TARGET_FILE:${TARGET_NAME}>)
endif()

unset(${TARGET_NAME})
unset(${TARGET_NAME_LIB})
