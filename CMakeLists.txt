cmake_minimum_required(VERSION 3.14)

project(TheGameQt LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
# If Qt6 is not found, try Qt5
if(NOT Qt6_FOUND)
    find_package(Qt5 REQUIRED COMPONENTS Widgets)
endif()

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

file(GLOB_RECURSE SOURCES 
    "Refrence/*.cpp"
    "Games/*.cpp"
    "Neural_Network/Source/*.cpp"
    "GUI/*.cpp"
    "GUI/*.h"
)

# Exclude TheGame.cpp from the GUI build to avoid multiple main functions
list(REMOVE_ITEM SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/TheGame.cpp") 

# Add the main GUI entry point
# We will create GUI/main.cpp later

add_executable(TheGameQt 
    ${SOURCES}
    "GUI/main.cpp"
)

target_include_directories(TheGameQt PRIVATE 
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/header"
)

if(Qt6_FOUND)
    target_link_libraries(TheGameQt PRIVATE Qt6::Widgets)
else()
    target_link_libraries(TheGameQt PRIVATE Qt5::Widgets)
endif()
