cmake_minimum_required(VERSION 3.12)

# Project configuration
project(xensiv_bgt60trxx
    VERSION 1.1.0
    DESCRIPTION "XENSIV BGT60TRxx 60 GHz FMCW Radar Sensor Library"
    HOMEPAGE_URL "https://github.com/DynamicDevices/sensor-xensiv-bgt60trxx"
    LANGUAGES C
)

# Set C standard
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Build options
option(BUILD_SHARED_LIBS "Build shared libraries" ON)
option(BUILD_EXAMPLES "Build example applications" ON)
option(BUILD_TESTS "Build test applications" OFF)
option(ENABLE_LINUX_SUPPORT "Enable Linux platform support" ON)
option(ENABLE_MTB_SUPPORT "Enable ModusToolbox platform support" OFF)

# Platform detection
if(UNIX AND NOT APPLE)
    set(LINUX TRUE)
endif()

# Compiler flags
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -Wpedantic)
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_compile_options(-g -O0 -DDEBUG)
    else()
        add_compile_options(-O2 -DNDEBUG)
    endif()
endif()

# Core library sources
set(CORE_SOURCES
    xensiv_bgt60trxx.c
)

set(CORE_HEADERS
    xensiv_bgt60trxx.h
    xensiv_bgt60trxx_regs.h
    xensiv_bgt60trxx_platform.h
)

# Platform-specific sources
set(PLATFORM_SOURCES "")
set(PLATFORM_HEADERS "")
set(PLATFORM_LIBS "")

# Linux platform support
if(ENABLE_LINUX_SUPPORT AND LINUX)
    list(APPEND PLATFORM_SOURCES xensiv_bgt60trxx_linux.c)
    list(APPEND PLATFORM_HEADERS xensiv_bgt60trxx_linux.h)
    add_compile_definitions(ENABLE_LINUX_SUPPORT=1)
    message(STATUS "Linux platform support enabled")
endif()

# ModusToolbox platform support
if(ENABLE_MTB_SUPPORT)
    list(APPEND PLATFORM_SOURCES xensiv_bgt60trxx_mtb.c)
    list(APPEND PLATFORM_HEADERS xensiv_bgt60trxx_mtb.h)
    add_compile_definitions(ENABLE_MTB_SUPPORT=1)
    message(STATUS "ModusToolbox platform support enabled")
endif()

# Create the main library
add_library(xensiv_bgt60trxx
    ${CORE_SOURCES}
    ${PLATFORM_SOURCES}
)

# Set library properties
set_target_properties(xensiv_bgt60trxx PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    PUBLIC_HEADER "${CORE_HEADERS};${PLATFORM_HEADERS}"
)

# Include directories
target_include_directories(xensiv_bgt60trxx
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>
)

# Link libraries
target_link_libraries(xensiv_bgt60trxx ${PLATFORM_LIBS})

# Create alias for consistent naming
add_library(xensiv_bgt60trxx::xensiv_bgt60trxx ALIAS xensiv_bgt60trxx)

# Examples
if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

# Tests
if(BUILD_TESTS)
    enable_testing()
    # Add test subdirectory when available
    # add_subdirectory(tests)
endif()

# Installation
include(GNUInstallDirs)

# Install library
install(TARGETS xensiv_bgt60trxx
    EXPORT xensiv_bgt60trxx-targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Install CMake config files
install(EXPORT xensiv_bgt60trxx-targets
    FILE xensiv_bgt60trxx-targets.cmake
    NAMESPACE xensiv_bgt60trxx::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/xensiv_bgt60trxx
)

# Create and install config file
include(CMakePackageConfigHelpers)

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/xensiv_bgt60trxx-config.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/xensiv_bgt60trxx-config.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/xensiv_bgt60trxx
)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/xensiv_bgt60trxx-config-version.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/xensiv_bgt60trxx-config.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/xensiv_bgt60trxx-config-version.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/xensiv_bgt60trxx
)

# pkg-config file
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/xensiv_bgt60trxx.pc.in"
    "${CMAKE_CURRENT_BINARY_DIR}/xensiv_bgt60trxx.pc"
    @ONLY
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/xensiv_bgt60trxx.pc"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig
)

# Configuration summary
message(STATUS "")
message(STATUS "XENSIV BGT60TRxx Configuration Summary:")
message(STATUS "  Version: ${PROJECT_VERSION}")
message(STATUS "  Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  Shared libraries: ${BUILD_SHARED_LIBS}")
message(STATUS "  Linux support: ${ENABLE_LINUX_SUPPORT}")
message(STATUS "  MTB support: ${ENABLE_MTB_SUPPORT}")
message(STATUS "  Build examples: ${BUILD_EXAMPLES}")
message(STATUS "  Build tests: ${BUILD_TESTS}")
message(STATUS "")