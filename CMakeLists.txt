cmake_minimum_required(VERSION 3.21)

project(SLD.Core VERSION 0.1.0 LANGUAGES CXX)

option(SLD_CORE_STATIC "Build SLD.Core as a static library instead of a DLL" OFF)

# exceptions are disabled, so drop the /EHsc that cmake adds by default
if(MSVC)
    string(REPLACE "/EHsc" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
endif()

if(SLD_CORE_STATIC)
    add_library(sld_core STATIC)
else()
    add_library(sld_core SHARED)
endif()
add_library(SLD::Core ALIAS sld_core)

# unity build: sld.cpp includes the other source files
target_sources(sld_core PRIVATE
    src/sld.cpp
    src/hash.cpp
    include/sld.hpp
)
set_source_files_properties(src/hash.cpp PROPERTIES HEADER_FILE_ONLY ON)

target_include_directories(sld_core
    PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
            ${CMAKE_CURRENT_SOURCE_DIR}/3rd-party
)

target_compile_features(sld_core PUBLIC cxx_std_17)
set_target_properties(sld_core PROPERTIES
    CXX_EXTENSIONS           OFF
    OUTPUT_NAME              "SLD.Core"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)

if(SLD_CORE_STATIC)
    target_compile_definitions(sld_core PUBLIC SLD_STATIC)
endif()

target_compile_definitions(sld_core PRIVATE _HAS_EXCEPTIONS=0)

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(sld_core PRIVATE /EHs-)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    # meow hash needs aes-ni, the simd helpers in sld.hpp need sse4.1
    target_compile_options(sld_core PUBLIC -msse4.1 PRIVATE -maes)
endif()
