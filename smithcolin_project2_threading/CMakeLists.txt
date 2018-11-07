cmake_minimum_required(VERSION 3.8)
project(project1_threading)

set(CMAKE_CXX_STANDARD 11)
include_directories(. $ENV{SIMULATOR_INC_LIB_DIR}/includes)
LINK_DIRECTORIES($ENV{SIMULATOR_INC_LIB_DIR}/lib)
set(SOURCE_FILES main.cpp thread.cpp lock.cpp testHelper.cpp testHelper.h goodStuff.h)

add_executable(project1_threading ${SOURCE_FILES})

TARGET_LINK_LIBRARIES(project1_threading libgtest.a libos_simulator.a pthread)