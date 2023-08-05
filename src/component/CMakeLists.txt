message(STATUS " * Building component")

list(APPEND LIB_DEPENDENCIES 
  utils
  loader
)

message(STATUS "   -> ${LIB_DEPENDENCIES}")

add_library(component 
  GraphicsComponent.cpp
  MovementComponent.cpp
  ControllableComponent.cpp
)

target_link_libraries(component PUBLIC ${LIB_DEPENDENCIES})
target_include_directories(component INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})