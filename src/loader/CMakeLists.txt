message(STATUS " * Building loader")

list(APPEND LIB_DEPENDENCIES 
  utils
  lodepng
  pugixml
  world
  entity
  core
)

message(STATUS "   -> ${LIB_DEPENDENCIES}")

add_library(loader 
  Loader.cpp
)

target_link_libraries(loader PUBLIC ${LIB_DEPENDENCIES})
target_include_directories(loader INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})