
PROJECT(StatorExampleFibonacci)

CONFIGURE_FILE("config.yaml" "config.yaml" COPYONLY)

SET(SOURCE_FILES
    fibonacci.cpp)

ADD_EXECUTABLE(stator_fibonacci ${SOURCE_FILES})


INCLUDE_DIRECTORIES(SYSTEM ${Stator_INCLUDE_DIRS})

TARGET_LINK_LIBRARIES(stator_fibonacci
                      stator)
