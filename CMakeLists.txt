cmake_minimum_required(VERSION 3.15)
project(Aincrad VERSION 2.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Project options
option(AINCRAD_BUILD_TESTS "Build test suites" ON)
option(AINCRAD_BUILD_EXAMPLES "Build example projects" ON)
option(AINCRAD_ENABLE_VR "Enable VR support" OFF)
option(AINCRAD_ENABLE_VISION_PRO "Enable Apple Vision Pro support" OFF)
option(AINCRAD_ENABLE_STEAM "Enable Steam integration" OFF)
option(AINCRAD_BUILD_TOOLS "Build development tools" ON)
option(AINCRAD_ENABLE_PROFILING "Enable performance profiling" OFF)
option(AINCRAD_BUILD_SAO_FRAMEWORK "Build SAO Framework" ON)

# SAO Framework specific options
option(SAO_BUILD_TESTS "Build SAO Framework test suites" ON)
option(SAO_BUILD_EXAMPLES "Build SAO Framework examples" ON)
option(SAO_ENABLE_VR "Enable SAO Framework VR support" OFF)
option(SAO_ENABLE_VISION_PRO "Enable SAO Framework Vision Pro support" OFF)
option(SAO_ENABLE_STEAM "Enable SAO Framework Steam integration" OFF)
option(SAO_BUILD_TOOLS "Build SAO Framework development tools" ON)
option(SAO_ENABLE_PROFILING "Enable SAO Framework performance profiling" OFF)

# Platform-specific settings
if(WIN32)
    set(PLATFORM_WINDOWS ON)
    add_definitions(-DWIN32_LEAN_AND_MEAN -D_CRT_SECURE_NO_WARNINGS)
elseif(APPLE)
    set(PLATFORM_MACOS ON)
    if(SAO_ENABLE_VISION_PRO)
        find_package(VisionOS REQUIRED)
        add_definitions(-DSAO_VISION_PRO_ENABLED)
    endif()
elseif(UNIX)
    set(PLATFORM_LINUX ON)
endif()

# Find required packages
find_package(Threads REQUIRED)

# VR SDKs
if(SAO_ENABLE_VR)
    if(WIN32)
        # Oculus SDK
        if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/external/oculus-sdk")
            add_subdirectory(external/oculus-sdk)
            add_definitions(-DSAO_OCULUS_ENABLED)
        endif()
        
        # OpenXR
        find_package(OpenXR QUIET)
        if(OpenXR_FOUND)
            add_definitions(-DSAO_OPENXR_ENABLED)
        endif()
    endif()
    
    # SteamVR
    if(SAO_ENABLE_STEAM)
        find_package(SteamVR QUIET)
        if(SteamVR_FOUND)
            add_definitions(-DSAO_STEAMVR_ENABLED)
        endif()
    endif()
endif()

# Testing framework
if(AINCRAD_BUILD_TESTS OR SAO_BUILD_TESTS)
    enable_testing()
    find_package(GTest QUIET)
    if(NOT GTest_FOUND)
        include(FetchContent)
        FetchContent_Declare(
            googletest
            GIT_REPOSITORY https://github.com/google/googletest.git
            GIT_TAG release-1.12.1
        )
        FetchContent_MakeAvailable(googletest)
    endif()
endif()

# Profiling
if(AINCRAD_ENABLE_PROFILING OR SAO_ENABLE_PROFILING)
    find_package(gperftools QUIET)
    if(gperftools_FOUND)
        add_definitions(-DSAO_PROFILING_ENABLED)
    endif()
endif()

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Source files
file(GLOB_RECURSE AINCRAD_SOURCES 
    "src/*.cpp"
    "src/*.c"
)

file(GLOB_RECURSE AINCRAD_HEADERS 
    "src/*.h"
    "src/*.hpp"
    "include/*.h"
    "include/*.hpp"
)

# SAO Framework sources
if(AINCRAD_BUILD_SAO_FRAMEWORK)
    file(GLOB_RECURSE SAO_SOURCES 
        "src/SAO/*.cpp"
    )
    
    file(GLOB_RECURSE SAO_HEADERS 
        "src/SAO/*.h"
        "src/SAO/*.hpp"
    )
    
    list(APPEND AINCRAD_SOURCES ${SAO_SOURCES})
    list(APPEND AINCRAD_HEADERS ${SAO_HEADERS})
endif()

# Create main library
add_library(Aincrad ${AINCRAD_SOURCES} ${AINCRAD_HEADERS})

# Set library properties
set_target_properties(Aincrad PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    PUBLIC_HEADER "${AINCRAD_HEADERS}"
)

# Link libraries
target_link_libraries(Aincrad
    Threads::Threads
)

# Platform-specific linking
if(PLATFORM_WINDOWS)
    target_link_libraries(Aincrad ws2_32 winmm)
elseif(PLATFORM_MACOS)
    target_link_libraries(Aincrad "-framework Cocoa" "-framework IOKit" "-framework CoreVideo")
    if(SAO_ENABLE_VISION_PRO)
        target_link_libraries(Aincrad "-framework RealityKit" "-framework ARKit")
    endif()
elseif(PLATFORM_LINUX)
    target_link_libraries(Aincrad X11 Xrandr Xi)
endif()

# VR libraries
if(SAO_ENABLE_VR)
    if(OpenXR_FOUND)
        target_link_libraries(Aincrad ${OpenXR_LIBRARIES})
    endif()
    if(SteamVR_FOUND)
        target_link_libraries(Aincrad ${SteamVR_LIBRARIES})
    endif()
endif()

# Profiling
if(AINCRAD_ENABLE_PROFILING OR SAO_ENABLE_PROFILING)
    if(gperftools_FOUND)
        target_link_libraries(Aincrad ${gperftools_LIBRARIES})
    endif()
endif()

# Compiler-specific flags
if(MSVC)
    target_compile_options(Aincrad PRIVATE /W4 /utf-8)
else()
    target_compile_options(Aincrad PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Create main executable
add_executable(AincradApp 
    src/main.cpp
)

target_link_libraries(AincradApp Aincrad)

# SAO Framework executable
if(AINCRAD_BUILD_SAO_FRAMEWORK)
    add_executable(SAOFramework 
        src/SAO/main.cpp
    )
    
    target_link_libraries(SAOFramework Aincrad)
    
    # Set properties for SAO Framework
    set_target_properties(SAOFramework PROPERTIES
        OUTPUT_NAME "sao-framework"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    )
endif()

# Tests
if(AINCRAD_BUILD_TESTS)
    add_subdirectory(tests)
endif()

if(SAO_BUILD_TESTS)
    add_subdirectory(src/SAO/tests)
endif()

# Examples
if(AINCRAD_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if(SAO_BUILD_EXAMPLES)
    add_subdirectory(src/SAO/examples)
endif()

# Tools
if(AINCRAD_BUILD_TOOLS)
    add_subdirectory(tools)
endif()

if(SAO_BUILD_TOOLS)
    add_subdirectory(src/SAO/tools)
endif()

# Installation
install(TARGETS Aincrad
    EXPORT AincradTargets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    PUBLIC_HEADER DESTINATION include
)

if(AINCRAD_BUILD_SAO_FRAMEWORK)
    install(TARGETS SAOFramework
        RUNTIME DESTINATION bin
    )
endif()

# Install headers
install(DIRECTORY src/
    DESTINATION include
    FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp"
)

# Export targets
install(EXPORT AincradTargets
    FILE AincradTargets.cmake
    NAMESPACE Aincrad::
    DESTINATION lib/cmake/Aincrad
)

# Config file
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    AincradConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/aincradConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/AincradConfig.cmake
    INSTALL_DESTINATION lib/cmake/Aincrad
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/AincradConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/AincradConfigVersion.cmake
    DESTINATION lib/cmake/Aincrad
)

# Documentation
find_package(Doxygen QUIET)
if(Doxygen_FOUND)
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
    
    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
    
    add_custom_target(docs ALL
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM
    )
endif()

# Package configuration
set(CPACK_PACKAGE_NAME "Aincrad")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Sword Art Online Framework and Game Engine")
set(CPACK_PACKAGE_VENDOR "Aincrad Development Team")
set(CPACK_PACKAGE_CONTACT "contact@aincrad.dev")

if(WIN32)
    set(CPACK_GENERATOR "NSIS;ZIP")
    set(CPACK_NSIS_DISPLAY_NAME "Aincrad SAO Framework")
    set(CPACK_NSIS_PACKAGE_NAME "Aincrad")
elseif(APPLE)
    set(CPACK_GENERATOR "DragNDrop;TGZ")
    set(CPACK_DMG_VOLUME_NAME "Aincrad")
else()
    set(CPACK_GENERATOR "DEB;RPM;TGZ")
endif()

include(CPack)

# Print configuration summary
message(STATUS "")
message(STATUS "=== Aincrad Configuration Summary ===")
message(STATUS "Version: ${PROJECT_VERSION}")
message(STATUS "Platform: ${CMAKE_SYSTEM_NAME}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "")
message(STATUS "Build Options:")
message(STATUS "  Tests: ${AINCRAD_BUILD_TESTS}")
message(STATUS "  Examples: ${AINCRAD_BUILD_EXAMPLES}")
message(STATUS "  VR Support: ${AINCRAD_ENABLE_VR}")
message(STATUS "  Vision Pro: ${AINCRAD_ENABLE_VISION_PRO}")
message(STATUS "  Steam Integration: ${AINCRAD_ENABLE_STEAM}")
message(STATUS "  Tools: ${AINCRAD_BUILD_TOOLS}")
message(STATUS "  Profiling: ${AINCRAD_ENABLE_PROFILING}")
message(STATUS "  SAO Framework: ${AINCRAD_BUILD_SAO_FRAMEWORK}")
message(STATUS "")
message(STATUS "SAO Framework Options:")
message(STATUS "  Tests: ${SAO_BUILD_TESTS}")
message(STATUS "  Examples: ${SAO_BUILD_EXAMPLES}")
message(STATUS "  VR Support: ${SAO_ENABLE_VR}")
message(STATUS "  Vision Pro: ${SAO_ENABLE_VISION_PRO}")
message(STATUS "  Steam Integration: ${SAO_ENABLE_STEAM}")
message(STATUS "  Tools: ${SAO_BUILD_TOOLS}")
message(STATUS "  Profiling: ${SAO_ENABLE_PROFILING}")
message(STATUS "")
message(STATUS "Installation:")
message(STATUS "  Prefix: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "  Binaries: ${CMAKE_INSTALL_PREFIX}/bin")
message(STATUS "  Libraries: ${CMAKE_INSTALL_PREFIX}/lib")
message(STATUS "  Headers: ${CMAKE_INSTALL_PREFIX}/include")
message(STATUS "=====================================")
message(STATUS "") 