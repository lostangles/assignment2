#include "gtest/gtest.h"
#include "IntfParser.h"

TEST (IntfParser, GetPortSize)
{
   IntfParser myParser;
   EXPECT_EQ( 31, myParser.GetPortSize("input Int32 a, b, c"));
}

TEST (IntfParser, GetPortNames)
{
   IntfParser myParser;
   std::vector<std::string> names = myParser.GetPortNames("input Int32 a, b, c");
   std::string name = (std::string) names.back(); names.pop_back();
   EXPECT_EQ( "c" , name );
   name = (std::string) names.back(); names.pop_back();
   EXPECT_EQ( "b" , name );
   name = (std::string) names.back(); names.pop_back();
   EXPECT_EQ( "a" , name );
}

TEST (FileReaderTest, ReadTest)
{
   IntfParser myParser;
   EXPECT_EQ( "module circuit1(input [7:0] a, input [7:0] b, input [7:0] c, output [7:0] z, output [15:0] x);", myParser.CreateModule("circuit1","input Int8 a, b, c\n output Int8 z\n output Int16 x\n"));
}
