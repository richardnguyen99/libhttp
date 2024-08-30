# add files to sources
file(GLOB SOURCES *.c)

# Every test file in the tests directory is a separate standalone executable
# This loop will create an executable for each source file so that CTest/CMake
# can run the tests with a single command.
foreach(SOURCE ${SOURCES})
    message(STATUS "Adding testing source file: ${SOURCE}")

    # get the file name
    get_filename_component(FILE_NAME ${SOURCE} NAME)

    # get the file name without the extension
    string(REGEX REPLACE "\\.[^.]*$" "" FILE_NAME ${FILE_NAME})

    # set the include directories relative to the root of the project
    include_directories(
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/lib
    )

    # add the source file to the test executable
    add_executable(${FILE_NAME} ${SOURCE})

    # add directories that contain the link libraries
    target_link_directories(${FILE_NAME} PUBLIC
        ${CMAKE_BINARY_DIR}/lib
        ${CMAKE_BINARY_DIR}/src
    )

    # add link to the unity library in lib/unity build directory
    target_link_libraries(${FILE_NAME} 
        unity
        libhttp
    )

    # Tell CMake and CTest to run the tests
    add_test(NAME ${FILE_NAME} COMMAND ${FILE_NAME})

    
endforeach(SOURCE ${SOURCES})
