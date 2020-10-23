include_directories(../googletest/googletest/include)

link_directories(../googletest/lib/)
add_executable(UnitTests UnitTests.cpp)
target_link_libraries(UnitTests gtest gtest_main)


