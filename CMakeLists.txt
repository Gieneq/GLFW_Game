cmake_minimum_required( VERSION 3.1 )

project( GLFW-CMake-starter )

find_package( OpenGL REQUIRED )

include_directories( ${OPENGL_INCLUDE_DIRS} )

if (MATH_LIBRARY)
    link_libraries("${MATH_LIBRARY}")
endif()

# set( GLFW_BUILD_DOCS OFF CACHE BOOL  "GLFW lib only" )
# set( GLFW_INSTALL OFF CACHE BOOL  "GLFW lib only" )

add_subdirectory( glfw )
link_libraries(glfw)
include_directories("${GLFW_SOURCE_DIR}/deps")

set(GLAD_GL "${GLFW_SOURCE_DIR}/deps/glad/gl.h"
            "${GLFW_SOURCE_DIR}/deps/glad_gl.c")
set(GLAD_VULKAN "${GLFW_SOURCE_DIR}/deps/glad/vulkan.h"
                "${GLFW_SOURCE_DIR}/deps/glad_vulkan.c")
set(GETOPT "${GLFW_SOURCE_DIR}/deps/getopt.h"
           "${GLFW_SOURCE_DIR}/deps/getopt.c")
set(TINYCTHREAD "${GLFW_SOURCE_DIR}/deps/tinycthread.h"
                "${GLFW_SOURCE_DIR}/deps/tinycthread.c")

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

set( GLFW-CMAKE-STARTER-SRC
    src/core/Core.cpp
    src/core/Settings.cpp
    src/core/Errors.cpp
    src/core/Window.cpp

    src/game/Game.cpp
    src/game/Camera.cpp

    src/utils/Maths.cpp

    src/entity/Entity.cpp

    src/component/Component.cpp
    src/component/TransformComponent.cpp
    src/component/FoodComponent.cpp
    src/component/SpriteComponent.cpp
    src/component/ControllableComponent.cpp

    src/system/SystemBase.cpp
    src/system/RenderSystem.cpp
    src/system/UserInputSystem.cpp
    src/system/TransformSystem.cpp

    src/main.cpp 
    )
     
add_executable( GLFW-CMake-starter ${GLFW-CMAKE-STARTER-SRC} ${GETOPT} ${GLAD_GL} )
# add_executable( GLFW-CMake-starter WIN32 ${GLFW-CMAKE-STARTER-SRC} )
target_link_libraries( GLFW-CMake-starter ${OPENGL_LIBRARIES} glfw )

# set(CONSOLE_BINARIES GLFW-CMake-starter)

# set_target_properties(${GUI_ONLY_BINARIES} ${CONSOLE_BINARIES} PROPERTIES
#                       FOLDER "GLFW3/Tests")

if( MSVC )
    if(${CMAKE_VERSION} VERSION_LESS "3.6.0") 
        message( "\n\t[ WARNING ]\n\n\tCMake version lower than 3.6.\n\n\t - Please update CMake and rerun; OR\n\t - Manually set 'GLFW-CMake-starter' as StartUp Project in Visual Studio.\n" )
    else()
    
        set_property( DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT GLFW-CMake-starter )
    endif()
endif()
