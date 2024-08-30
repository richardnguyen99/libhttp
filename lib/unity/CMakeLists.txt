# Add the sources files to the Unity library
file(GLOB SOURCES ./*.c)

# Build the Unity library
add_library(unity STATIC ${SOURCES})

# Include the current directory
target_include_directories(unity PUBLIC .)
