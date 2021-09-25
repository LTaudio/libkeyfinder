cmake_minimum_required(VERSION 3.15)
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

    list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})
    list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})

    if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
      message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
      file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/v0.16.1/conan.cmake"
                    "${CMAKE_BINARY_DIR}/conan.cmake"
                    EXPECTED_HASH SHA256=396e16d0f5eabdc6a14afddbcfff62a54a7ee75c6da23f32f7a31bc85db23484
                    TLS_VERIFY ON)
    endif()

    include(${CMAKE_BINARY_DIR}/conan.cmake)
    conan_add_remote(NAME conancenter URL https://center.conan.io)
    conan_cmake_configure(
        REQUIRES
            fftw/3.3.9
        GENERATORS
            cmake_find_package
        OPTIONS
            fftw:precision=double
            fftw:simd=sse
    )
    conan_cmake_autodetect(settings)
    conan_cmake_install(
        PATH_OR_REFERENCE
            .
        BUILD
            outdated
        REMOTE
            conancenter
        SETTINGS
            ${settings}
    )
endif()

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(ltCompilerOptions)
include(ltCompilerWarnings)
include(ltCodeCoverage)

find_package(FFTW3 REQUIRED)

add_library(keyfinder)
add_library(lt::KeyFinder ALIAS keyfinder)
set_target_properties(keyfinder PROPERTIES VERSION ${PROJECT_VERSION})
target_link_libraries(keyfinder PUBLIC FFTW3::fftw3 lt::CompilerWarnings lt::CodeCoverage)
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
