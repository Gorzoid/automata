cmake_minimum_required(VERSION 3.9.0)

add_executable(automata_tests test.cpp)
target_link_libraries(automata_tests PUBLIC automata)

hunter_add_package(GTest)
find_package(GTest CONFIG REQUIRED)
target_link_libraries(automata_tests PUBLIC GTest::main)

find_package(GMock CONFIG REQUIRED)
target_link_libraries(automata_tests PUBLIC GMock::gmock)

include(GoogleTest)
gtest_discover_tests(automata_tests
    TEST_LIST test_targets)