message(STATUS " * Building system")

list(APPEND LIB_DEPENDENCIES 
  utils
  core
  game
  component
)

message(STATUS "   -> ${LIB_DEPENDENCIES}")

add_library(system 
  SystemBase.cpp
  RenderSystem.cpp
  MovementSystem.cpp
  UserInputSystem.cpp
)

target_link_libraries(system PUBLIC ${LIB_DEPENDENCIES})
target_include_directories(system INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})