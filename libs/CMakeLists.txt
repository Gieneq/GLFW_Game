message(STATUS "Building libs from: " ${CMAKE_CURRENT_SOURCE_DIR})

message(STATUS " * Building lodepng")
add_library(lodepng
    lodepng/lodepng.cpp
)
target_include_directories(lodepng INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/lodepng)


message(STATUS " * Building pugixml")
add_library(pugixml
    pugixml/src/pugixml.cpp
)
target_include_directories(pugixml INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/pugixml/src)


message(STATUS " * Building cpppath")
add_library(cpppath
    INTERFACE
)
target_include_directories(cpppath INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/cpppath/include)

message(STATUS " * Building cJSON")
add_library(cJSON
    cJSON/cJSON.c
    cJSON/cJSON_Utils.c
)
target_include_directories(cJSON INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/cJSON)
