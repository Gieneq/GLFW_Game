message(STATUS "Building test")

message(STATUS " -> ${LIB_DEPENDENCIES}")

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(GWorld_test
MathsTest.cpp
)

target_link_libraries(
    GWorld_test
    GTest::gtest_main
    maths
)

include(GoogleTest)
gtest_discover_tests(GWorld_test)