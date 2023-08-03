message(STATUS "Building src")

# Copy content of res to build
# file(COPY res DESTINATION ${CMAKE_BINARY_DIR})

# Refresh asset
file(REMOVE_RECURSE ${CMAKE_CURRENT_BINARY_DIR}/res/)
file(COPY ../res DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")


add_subdirectory(utils)
add_subdirectory(component)
add_subdirectory(entity)
add_subdirectory(core)
add_subdirectory(system)
add_subdirectory(game)
add_subdirectory(world)
add_subdirectory(loader)


list(APPEND DEP_LIBS
    lodepng
    pugixml
    utils
    component
    entity
    core
    system
    game
    world
    loader
)







# set(GLAD_GL "${GLFW_SOURCE_DIR}/deps/glad/gl.h"
#             "${GLFW_SOURCE_DIR}/deps/glad/glad.h"
#             "${GLFW_SOURCE_DIR}/deps/glad.c")
# set(GLAD_VULKAN "${GLFW_SOURCE_DIR}/deps/glad/vulkan.h"
#                 "${GLFW_SOURCE_DIR}/deps/glad_vulkan.c")
#https://learnopengl.com/Getting-started/Creating-a-window
#https://glad.dav1d.de/
# include_directories(glfw/deps/glad)
# include_directories(glfw/KHR)
# set(GLAD_GL "${GLFW_SOURCE_DIR}/deps/glad/gl.h"
#             "${GLFW_SOURCE_DIR}/deps/glad/glad.h"
#             "${GLFW_SOURCE_DIR}/deps/glad_gl.c")
# set(GETOPT "${GLFW_SOURCE_DIR}/deps/getopt.h"
#            "${GLFW_SOURCE_DIR}/deps/getopt.c")
# set(TINYCTHREAD "${GLFW_SOURCE_DIR}/deps/tinycthread.h"
#                 "${GLFW_SOURCE_DIR}/deps/tinycthread.c")


# add_executable( GWorld ${GETOPT} ${GLAD_GL} )
add_executable(GWorld
    main.cpp
    # ${GETOPT} 
    # ${GLAD_GL}
    # "${GLFW_SOURCE_DIR}/deps_generated/glad.c"
)

# target_link_libraries(GWorld PUBLIC ${DEP_LIBS})
# target_link_libraries( GWorld ${OPENGL_LIBRARIES} glfw )
target_link_libraries(GWorld PUBLIC ${DEP_LIBS} ${OPENGL_LIBRARIES} glfw)

target_include_directories(GWorld PUBLIC
                           "${PROJECT_BINARY_DIR}"
                        #    "${GLFW_SOURCE_DIR}/deps"
                        #    "${GLFW_SOURCE_DIR}/deps_generated"
                           )
                           