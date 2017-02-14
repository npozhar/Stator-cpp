CMAKE_MINIMUM_REQUIRED(VERSION 2.8.9)

PROJECT(Stator)

if ("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
    message(SEND_ERROR "In-source builds are not allowed.")
endif ()

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fno-strict-overflow -Wall -Wextra -pedantic -Wshadow -Wformat=2 -Wuninitialized -Wfloat-equal -Wundef -Wpointer-arith -Wcast-align -Wwrite-strings -Wcast-qual -Wswitch-default -Wswitch-enum -Wconversion -Wunreachable-code -Werror-implicit-function-declaration")

INCLUDE_DIRECTORIES(include/stator)

SET(HEADER_FILES
    include/stator/config.hpp
    include/stator/serializer.hpp
    include/stator/reporter.hpp
    include/stator/engine.hpp)

SET(SOURCE_FILES
    src/config.cpp
    src/serializer.cpp
    src/reporter.cpp
    src/engine.cpp)

ADD_LIBRARY(stator ${SOURCE_FILES} ${HEADER_FILES})

# Boost #

FIND_PACKAGE(Boost REQUIRED COMPONENTS system thread regex program_options)
IF(Boost_FOUND)
    INCLUDE_DIRECTORIES(SYSTEM ${Boost_INCLUDE_DIRS})
    LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
ELSE()
    MESSAGE(WARNING Boost not found)
ENDIF()

SET(USED_LIBS ${USED_LIBS} ${Boost_LIBRARIES})

# yaml-cpp #

FIND_PATH(YAMLCPP_INCLUDE_DIR
          NAMES yaml-cpp/yaml.h
          HINTS ${YAMLCPP_ROOT}
          PATH_SUFFIXES include)

FIND_LIBRARY(YAMLCPP_LIBRARY
             yaml-cpp
             HINTS ${YAMLCPP_ROOT}
             PATH_SUFFIXES lib)

INCLUDE_DIRECTORIES(SYSTEM ${YAMLCPP_INCLUDE_DIR})
IF(YAMLCPP_LIBRARY)
    SET(USED_LIBS ${USED_LIBS} ${YAMLCPP_LIBRARY})
ENDIF()


TARGET_LINK_LIBRARIES(stator
                      ${USED_LIBS})


# Expose includes to other subprojects through cache variable.
SET(${PROJECT_NAME}_INCLUDE_DIRS
    ${PROJECT_SOURCE_DIR}/include
    CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)


# Add examples
ADD_SUBDIRECTORY(example)

