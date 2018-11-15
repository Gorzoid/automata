cmake_minimum_required(VERSION 3.0.0)

add_library(automata automata.cpp)
target_include_directories(automata PUBLIC ../include)

add_executable(automata_cli main.cpp)
target_link_libraries(automata_cli PUBLIC automata)