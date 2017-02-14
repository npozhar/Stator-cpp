
PROJECT(StatorExampleFibonacci)

CONFIGURE_FILE("config.yaml" "config.yaml" COPYONLY)

SET(SOURCE_FILES
    fibonacci.cpp)

ADD_EXECUTABLE(stator_fibonacci ${SOURCE_FILES})

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


INCLUDE_DIRECTORIES(SYSTEM ${Stator_INCLUDE_DIRS})

TARGET_LINK_LIBRARIES(stator_fibonacci
                      ${USED_LIBS}
                      stator)

