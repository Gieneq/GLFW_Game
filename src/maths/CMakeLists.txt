message(STATUS " * Building maths")

list(APPEND LIB_DEPENDENCIES 
  
)

message(STATUS "   -> ${LIB_DEPENDENCIES}")

add_library(maths 
  Maths.cpp
)

target_link_libraries(maths PUBLIC ${LIB_DEPENDENCIES})
target_include_directories(maths INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})