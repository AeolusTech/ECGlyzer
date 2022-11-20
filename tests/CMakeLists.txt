project(unittests LANGUAGES CXX)

find_package(Qt6 REQUIRED COMPONENTS Test)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)

enable_testing(true)

add_executable(unittests ArcParseUT.cpp)
add_test(NAME unittests COMMAND unittests)

target_link_libraries(unittests PRIVATE Qt::Test)
