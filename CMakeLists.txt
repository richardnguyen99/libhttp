# Set the minimum version of CMake that can be used to build the project
cmake_minimum_required(VERSION 3.9...3.12)

# LibHTTP is C-only prject.
project(libhttp VERSION 0.1.0 LANGUAGES C)

# Set default build type to Debug
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

# Set C flags for optimization and warnings
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O2 -Wall -Wextra")


# Set some packaging variables.
set(CPACK_PROJECT_NAME              ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION_MAJOR     ${PROJECT_VERSION_MAJOR})
set(CPACK_PROJECT_VERSION_MINOR     ${PROJECT_VERSION_MINOR})
set(CPACK_PROJECT_VERSION_PATCH     ${PROJECT_VERSION_PATCH})
set(CPACK_SOURCE_IGNORE_FILES 
        ${PROJECT_SOURCE_DIR}/build
        ${PROJECT_SOURCE_DIR}/cmake-build-debug
        ${PROJECT_SOURCE_DIR}/pack
        ${PROJECT_SOURCE_DIR}/.idea
        ${PROJECT_SOURCE_DIR}/.DS_Store
        ${PROJECT_SOURCE_DIR}/.git
        ${PROJECT_SOURCE_DIR}/.vscode)

# Install some libraries for building tools.
include(CheckSymbolExists)
include(CheckIncludeFile)
include(CheckIncludeFiles)
include(CheckCSourceCompiles)
include(CheckTypeSize)
include(CPack)
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# Enable testing
include(CTest)

# Add debug flags on Debug build
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g")

# Add debug definitions on Debug build
if (CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions(-DDEBUG)
endif()


# Add subdirectories for testing framework
add_subdirectory(lib)

# Add subdirectories for the LibHTTP library
add_subdirectory(src)

# Add subdirectories for the tests
enable_testing()
add_subdirectory(tests)


