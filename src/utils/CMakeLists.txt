message(STATUS " * Building utils")

list(APPEND LIB_DEPENDENCIES 
  
)

message(STATUS "   -> ${LIB_DEPENDENCIES}")

add_library(utils 
  IOPath.cpp
)

target_link_libraries(utils PUBLIC ${LIB_DEPENDENCIES})
target_include_directories(utils INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})