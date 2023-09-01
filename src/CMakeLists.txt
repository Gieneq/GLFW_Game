message(STATUS "Building src")

add_subdirectory(utils)
add_subdirectory(maths)
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
    maths
    component
    entity
    core
    system
    game
    world
    loader
)

add_executable(GWorld
    main.cpp
)

target_link_libraries(GWorld PUBLIC ${DEP_LIBS} ${OPENGL_LIBRARIES} glfw)

target_include_directories(GWorld PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           )
                           