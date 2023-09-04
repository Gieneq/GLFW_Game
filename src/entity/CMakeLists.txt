message(STATUS " * Building entity")

list(APPEND LIB_DEPENDENCIES 
  utils
  maths
  component
  world
)

message(STATUS "   -> ${LIB_DEPENDENCIES}")

add_library(entity 
  Entity.cpp
  EntityContainers.cpp
  Player.cpp
)

target_link_libraries(entity PUBLIC ${LIB_DEPENDENCIES})
target_include_directories(entity INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})