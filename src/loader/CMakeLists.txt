message(STATUS " * Building loader")

list(APPEND LIB_DEPENDENCIES 
  maths
  utils
  lodepng
  pugixml
  world
  entity
  core
)

message(STATUS "   -> ${LIB_DEPENDENCIES}")

add_library(loader 
  LoaderDatatypes.cpp
  LoaderBase.cpp
  Loader.cpp
)

target_link_libraries(loader PUBLIC ${LIB_DEPENDENCIES})
target_include_directories(loader INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})