CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(Robots CXX)  

# cpp flags
    SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra -Wno-write-strings -g -std=c++0x")
    SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -Wall -Wextra -Wno-write-strings -std=c++0x")

# compile flags, Release by default
IF (NOT CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE Release)
ENDIF()

# include directory
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include)

# library directory
LINK_DIRECTORIES(${PROJECT_SOURCE_DIR}/libs)

# subdirectories
ADD_SUBDIRECTORY(BiRob)
ADD_SUBDIRECTORY(MyDog)
