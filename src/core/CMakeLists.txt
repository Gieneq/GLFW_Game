message(STATUS " * Building core")

list(APPEND LIB_DEPENDENCIES 
maths
  loader
)

message(STATUS "   -> ${LIB_DEPENDENCIES}")

add_library(core 
  Timers.cpp
  Core.cpp
  Errors.cpp
  Settings.cpp
  Window.cpp
)

target_link_libraries(core PUBLIC ${LIB_DEPENDENCIES})
target_include_directories(core INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})