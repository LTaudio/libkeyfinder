cmake_minimum_required(VERSION 3.4)
project(KeyFinder VERSION 2.2.5)

# Only do these if this is the main project,
# and not if it is included through add_subdirectory.
if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    # Caches build artifacts for faster builds
    find_program(CCACHE ccache)
    if (CCACHE)
        set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE})
    endif ()

    # Set C11 with no extensions enabled. Fail if the
    # standard is not supported.
    set(CMAKE_C_STANDARD 11)
    set(CMAKE_C_STANDARD_REQUIRED ON)
    set(CMAKE_C_EXTENSIONS OFF)

    # Set C++17 with no extensions enabled. Fail if the
    # standard is not supported.
    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_CXX_EXTENSIONS OFF)

    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
find_package(FFTW3 REQUIRED)

add_library(keyfinder)
add_library(lt::KeyFinder ALIAS keyfinder)
set_target_properties(keyfinder PROPERTIES VERSION ${PROJECT_VERSION})
target_link_libraries(keyfinder PUBLIC FFTW3::fftw3)
target_include_directories(keyfinder PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_sources(keyfinder
  PRIVATE
    src/audiodata.cpp
    src/chromagram.cpp
    src/chromatransform.cpp
    src/chromatransformfactory.cpp
    src/fftadapter.cpp
    src/keyclassifier.cpp
    src/keyfinder.cpp
    src/lowpassfilter.cpp
    src/lowpassfilterfactory.cpp
    src/spectrumanalyser.cpp
    src/temporalwindowfactory.cpp
    src/toneprofiles.cpp
    src/windowfunctions.cpp
    src/workspace.cpp
    src/constants.cpp
)

include(CTest)
if(BUILD_TESTING)
  add_subdirectory(tests)
endif()
