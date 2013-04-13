cmake_minimum_required(VERSION 2.8)
PROJECT(mnb_libsvm)

SET(Boost_FIND_REQUIRED TRUE) 
SET(Boost_USE_MULTITHREAD ON)
FIND_PACKAGE(Boost 1.48 COMPONENTS system program_options)
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS}) 

INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})

SET(CMAKE_CXX_FLAGS "-std=c++0x")

ADD_EXECUTABLE(mnb_libsvm main.cpp data.cpp)
TARGET_LINK_LIBRARIES(mnb_libsvm ${Boost_LIBRARIES})

