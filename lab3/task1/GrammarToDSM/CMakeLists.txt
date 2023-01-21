cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

project(GrammarToDSM)

set(CMAKE_CXX_STANDARD 20)

add_executable(GrammarToDSM main.cpp src/ReadWriteTable/ReadWriteTable.h src/ReadWriteTable/ReadWriteTable.cpp)