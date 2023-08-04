message(STATUS " * Building game")

list(APPEND LIB_DEPENDENCIES 
  utils
  core
  system
)

message(STATUS "   -> ${LIB_DEPENDENCIES}")

add_library(game 
  GameBase.cpp
  Camera.cpp
  Game.cpp
)

target_link_libraries(game PUBLIC ${LIB_DEPENDENCIES})
target_include_directories(game INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})