cmake_minimum_required(VERSION 3.6)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(GWorld VERSION 0.0.1)

# Change location of Debug folder
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

# Refresh asset
file(REMOVE_RECURSE ${CMAKE_CURRENT_BINARY_DIR}/res/)
file(COPY res DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")


# Setup OpenGL, GLFW, GLAD
find_package( OpenGL REQUIRED )
include_directories( ${OPENGL_INCLUDE_DIRS} )
if (MATH_LIBRARY)
    link_libraries("${MATH_LIBRARY}")
endif()
add_subdirectory( glfw )
link_libraries(glfw)


if( MSVC )
    SET( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /ENTRY:mainCRTStartup" )
endif()

if (${CMAKE_VERSION} VERSION_EQUAL "3.1.0" OR
    ${CMAKE_VERSION} VERSION_GREATER "3.1.0")
    set(CMAKE_C_STANDARD 99)
else()
    # Remove this fallback when removing support for CMake version less than 3.1
    add_compile_options("$<$<C_COMPILER_ID:AppleClang>:-std=c99>"
                        "$<$<C_COMPILER_ID:Clang>:-std=c99>"
                        "$<$<C_COMPILER_ID:GNU>:-std=c99>")

endif()

# Add libraries
add_subdirectory(libs)

# Add source
add_subdirectory(src)


