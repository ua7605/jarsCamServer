cmake_minimum_required(VERSION 3.10)
project(dust_java_core)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(WITH_DUST_STATIC ON)
find_package(dust 2 CONFIG REQUIRED)

find_package(JNI REQUIRED)

message(STATUS "DUST_INCLUDE_DIR = ${DUST_INCLUDE_DIR}")
message(STATUS "DUST_LIBRARY = ${DUST_LIBRARY}")
message(STATUS "JNI_INCLUDE_DIR = ${JNI_INCLUDE_DIRS}")
message(STATUS "JNI_LIBRARY = ${JNI_LIBRARIES}")

add_library(_core SHARED
    src/main/cpp/common.h src/main/cpp/common.cpp
    src/main/cpp/Core_Native.h src/main/cpp/Core_Native.cpp
    src/main/cpp/Version_Native.h src/main/cpp/Version_Native.cpp)

set_target_properties(_core
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

target_include_directories(_core PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        ${DUST_INCLUDE_DIR}
        ${JNI_INCLUDE_DIRS}
        PRIVATE pydust
        )

target_link_libraries(_core
        ${DUST_LIBRARY}
        ${JNI_LIBRARIES})