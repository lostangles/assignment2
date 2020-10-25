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

TEST (IntfParser, GetType)
{
   IntfParser myParser;
   EXPECT_EQ(Component_Type_e::REG, myParser.GetType("register Int64 greg, hreg"));
   EXPECT_EQ(Component_Type_e::ADD, myParser.GetType("d = a + b"));
   EXPECT_EQ(Component_Type_e::SUB, myParser.GetType("xwire = f - d"));
   EXPECT_EQ(Component_Type_e::MUL, myParser.GetType("f = a * c"));
   EXPECT_EQ(Component_Type_e::COMP, myParser.GetType("g = d > e"));
   EXPECT_EQ(Component_Type_e::MUX2x1, myParser.GetType("z = g ? d : e"));
   EXPECT_EQ(Component_Type_e::SHR, myParser.GetType("zrin = greg >> dEQe"));
   EXPECT_EQ(Component_Type_e::SHL, myParser.GetType("xrin = hreg << dLTe"));
   EXPECT_EQ(Component_Type_e::DIV, myParser.GetType("e = a / b"));
   EXPECT_EQ(Component_Type_e::MOD, myParser.GetType("g = a % b"));
   EXPECT_EQ(Component_Type_e::INC, myParser.GetType("f = c + 1"));
   EXPECT_EQ(Component_Type_e::DEC, myParser.GetType("e = a - 1"));
}

TEST (Component, ParsePorts)
{
   ComponentWIRE myWIRE(63);
   ComponentADD myADD;
   ComponentSUB mySUB;
   ComponentMUL myMUL;
   ComponentCOMP myCOMP;
   ComponentMUX2x1 myMUX2x1;
   ComponentSHR mySHR;
   ComponentSHL mySHL;
   ComponentDIV myDIV;
   ComponentMOD myMOD;
   ComponentINC myINC;
   ComponentDEC myDEC;

   myWIRE.ParsePorts("wire Int64 e, f, g, zwire");

   myADD.ParsePorts("d = a + b");
   EXPECT_EQ(myADD.output, "d");
   EXPECT_EQ(myADD.inputA, "a");
   EXPECT_EQ(myADD.inputB, "b");

   mySUB.ParsePorts("xwire = f - d");
   EXPECT_EQ(mySUB.output, "xwire");
   EXPECT_EQ(mySUB.inputA, "f");
   EXPECT_EQ(mySUB.inputB, "d");

   myMUL.ParsePorts("f = a * c");
   EXPECT_EQ(myMUL.output, "f");
   EXPECT_EQ(myMUL.inputA, "a");
   EXPECT_EQ(myMUL.inputB, "c");

   myCOMP.ParsePorts("g = d > e");
   EXPECT_EQ(myCOMP.output, "g");
   EXPECT_EQ(myCOMP.inputA, "d");
   EXPECT_EQ(myCOMP.inputB, "e");
   EXPECT_EQ(myCOMP.greaterThan, "g");

   myMUX2x1.ParsePorts("z = g ? d : e");
   EXPECT_EQ(myMUX2x1.output, "z");
   EXPECT_EQ(myMUX2x1.inputA, "d");
   EXPECT_EQ(myMUX2x1.inputB, "e");
   EXPECT_EQ(myMUX2x1.switchInput, "g");

   mySHR.ParsePorts("zrin = greg >> dEQe");
   EXPECT_EQ(mySHR.output, "zrin");
   EXPECT_EQ(mySHR.inputA, "greg");
   EXPECT_EQ(mySHR.inputB, "dEQe");

   mySHL.ParsePorts("xrin = hreg << dLTe");
   EXPECT_EQ(mySHL.output, "xrin");
   EXPECT_EQ(mySHL.inputA, "hreg");
   EXPECT_EQ(mySHL.inputB, "dLTe");

   myDIV.ParsePorts("e = a / b");
   EXPECT_EQ(myDIV.output, "e");
   EXPECT_EQ(myDIV.inputA, "a");
   EXPECT_EQ(myDIV.inputB, "b");

   myMOD.ParsePorts("g = a % b");
   EXPECT_EQ(myMOD.output, "g");
   EXPECT_EQ(myMOD.inputA, "a");
   EXPECT_EQ(myMOD.inputB, "b");

   myINC.ParsePorts("f = c + 1");
   EXPECT_EQ(myINC.output, "f");
   EXPECT_EQ(myINC.inputA, "c");
   EXPECT_EQ(myINC.inputB, "1");

   myDEC.ParsePorts("e = a - 1");
   EXPECT_EQ(myDEC.output, "e");
   EXPECT_EQ(myDEC.inputA, "a");
   EXPECT_EQ(myDEC.inputB, "1");

}
