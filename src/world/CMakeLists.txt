message(STATUS " * Building world")

list(APPEND LIB_DEPENDENCIES 
  maths
  entity
)

message(STATUS "   -> ${LIB_DEPENDENCIES}")

add_library(world 
  Coordinates.cpp
  Elevation.cpp
  World.cpp
)

target_link_libraries(world PUBLIC ${LIB_DEPENDENCIES})
target_include_directories(world INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})