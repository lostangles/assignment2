include_directories(../googletest/googletest/include)
include_directories(../src/)
link_directories(../googletest/lib/)
add_executable(UnitTests UnitTests.cpp ../src/IntfParser.cpp ../src/FileReader.cpp ../src/Djikstra.cpp)
target_link_libraries(UnitTests gtest gtest_main)


