# Add source files to the library
file(GLOB SOURCES ./*.c)

# Build a static library
add_library(libhttp STATIC ${SOURCES})

# Include the current directory
target_include_directories(libhttp PUBLIC ../include)

# Check for required standard libraries. Terminates the build if not found.
check_include_file("stdlib.h"                               HAVE_STDLIB_H)
check_include_file("stdio.h"                                HAVE_STDIO_H)
check_include_file("stdint.h"                               HAVE_STDINT_H)
check_include_file("stdbool.h"                              HAVE_STDBOOL_H)
check_include_file("string.h"                               HAVE_STRING_H)

if (HAVE_STDLIB_H)
    set(HAVE_STDLIB_H 1)
else()
    message(FATAL_ERROR "stdlib.h is required")
endif()

if (HAVE_STDIO_H)
    set(HAVE_STDIO_H 1)
else()
    message(FATAL_ERROR "stdio.h is required")
endif()

if (HAVE_STDINT_H)
    set(HAVE_STDINT_H 1)
else()
    message(FATAL_ERROR "stdint.h is required")
endif()

if (HAVE_STDBOOL_H)
    set(HAVE_STDBOOL_H 1)
else()
    message(FATAL_ERROR "stdbool.h is required")
endif()

if (HAVE_STRING_H)
    set(HAVE_STRING_H 1)
else()
    message(FATAL_ERROR "string.h is required")
endif()

# Check for required for POSIX and UNIX functions
CHECK_INCLUDE_FILE("unistd.h"                               HAVE_UNISTD_H)

if (HAVE_UNISTD_H)
    set(HAVE_UNISTD_H 1)
endif()
