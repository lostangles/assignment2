#include "gtest/gtest.h"
#include "IntfParser.h"
#include "FileReader.h"
#include "Djikstra.h"

TEST (IntfParser, GetPortSize)
{
   IntfParser myParser;
   EXPECT_EQ( 31, myParser.GetPortSize("input Int32 a, b, c"));
   EXPECT_EQ( 63, myParser.GetPortSize("wire Int64 e, f, g, zwire"));
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
   ComponentREG myREG(63);
   ComponentINPUT myINPUT(63);
   ComponentOUTPUT myOUTPUT(63);
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

   myINPUT.ParsePorts("input Int64 a, b, c");
   EXPECT_EQ(myINPUT.ports.at(0), "a");
   EXPECT_EQ(myINPUT.ports.at(1), "b");
   EXPECT_EQ(myINPUT.ports.at(2), "c");

   myOUTPUT.ParsePorts("output Int32 z, x");
   EXPECT_EQ(myOUTPUT.ports.at(0), "z");
   EXPECT_EQ(myOUTPUT.ports.at(1), "x");

   myWIRE.ParsePorts("wire Int64 e, f, g, zwire");
   EXPECT_EQ(myWIRE.wires.at(0), "e");
   EXPECT_EQ(myWIRE.wires.at(1), "f");
   EXPECT_EQ(myWIRE.wires.at(2), "g");
   EXPECT_EQ(myWIRE.wires.at(3), "zwire");
   
   myREG.ParsePorts("register Int64 greg, hreg");
   EXPECT_EQ(myREG.registers.at(0), "greg");
   EXPECT_EQ(myREG.registers.at(1), "hreg");

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

TEST (IntfParser, AddComponent)
{
   IntfParser myParser;
   myParser.AddPorts("wire Int64 e, f, g, zwire");
   EXPECT_EQ(myParser.wires.at(0).size, 63);
   EXPECT_EQ(myParser.wires.at(0).wires.at(0), "e");
   EXPECT_EQ(myParser.wires.at(0).wires.at(1), "f");
   EXPECT_EQ(myParser.wires.at(0).wires.at(2), "g");
   EXPECT_EQ(myParser.wires.at(0).wires.at(3), "zwire");
   myParser.AddPorts("input Int64 a, b, c");
   EXPECT_EQ(myParser.inputs.at(0).size, 63);
   EXPECT_EQ(myParser.inputs.at(0).ports.at(0), "a");
   EXPECT_EQ(myParser.inputs.at(0).ports.at(1), "b");
   EXPECT_EQ(myParser.inputs.at(0).ports.at(2), "c");
   myParser.AddPorts("output Int32 z, x");
   EXPECT_EQ(myParser.outputs.at(0).size, 31);
   EXPECT_EQ(myParser.outputs.at(0).ports.at(0), "z");
   EXPECT_EQ(myParser.outputs.at(0).ports.at(1), "x");
   myParser.AddPorts("register Int64 greg, hreg");
   EXPECT_EQ(myParser.registers.at(0).size, 63);
   EXPECT_EQ(myParser.registers.at(0).registers.at(0), "greg");
   EXPECT_EQ(myParser.registers.at(0).registers.at(1), "hreg");
 
   //Testing finding output port size
   myParser.FindPortSize("x");  
   EXPECT_EQ(31, myParser.FindPortSize("x"));
   EXPECT_EQ(63, myParser.FindPortSize("greg"));
   EXPECT_EQ(63, myParser.FindPortSize("zwire"));
   EXPECT_EQ(63, myParser.FindPortSize("a"));

  //Testing adding adder
  myParser.AddComponent("z = greg + zwire");
  EXPECT_EQ(myParser.components.at(0)->output, "z");
  EXPECT_EQ(myParser.components.at(0)->inputA, "greg");
  EXPECT_EQ(myParser.components.at(0)->inputB, "zwire");
  EXPECT_EQ(myParser.components.at(0)->size, 31);
  EXPECT_EQ(myParser.components.at(0)->inputSizeA, 63);
  EXPECT_EQ(myParser.components.at(0)->inputSizeB, 63);
}

TEST (Component, ConvertToLine)
{
   IntfParser myParser;

   myParser.AddPorts("wire Int64 e, f, g, zwire, xwire");
   myParser.AddPorts("wire dEQe, dLTe");
   myParser.AddPorts("input Int64 a, b, c");
   myParser.AddPorts("output Int32 z, x, xrin, d");
   myParser.AddPorts("output Int64 d, x, zrin");
   myParser.AddPorts("register Int64 greg, hreg");
   myParser.AddComponent("d = a + b");
   myParser.AddComponent("xwire = f - d");
   myParser.AddComponent("f = a * c");
   myParser.AddComponent("g = d > e");
   myParser.AddComponent("z = g ? d : e");
   myParser.AddComponent("zrin = greg >> dEQe");
   myParser.AddComponent("xrin = hreg << dLTe");
   myParser.AddComponent("e = a / b");
   myParser.AddComponent("g = a % b");
   myParser.AddComponent("f = c + 1");
   myParser.AddComponent("e = a - 1");
   EXPECT_EQ(myParser.WriteComponent(0), "ADD#(.DATAWIDTH(64)) ADDER_0($signed({a}),$signed({b}),{d});");
   EXPECT_EQ(myParser.WriteComponent(1), "SUB#(.DATAWIDTH(64)) SUB_0($signed({f}),$signed({d}),{xwire});");
   EXPECT_EQ(myParser.WriteComponent(2), "MUL#(.DATAWIDTH(64)) MUL_0($signed({a}),$signed({c}),{f});");
   EXPECT_EQ(myParser.WriteComponent(3), "COMP#(.DATAWIDTH(64)) COMP_0(.a($signed({d})),.b($signed({e})),.gt(g),.lt(),.eq());");
   EXPECT_EQ(myParser.WriteComponent(4), "MUX2x1#(.DATAWIDTH(32)) MUX_0(.a({d}),.b({e}),.sel(g),.d({z}));");
   EXPECT_EQ(myParser.WriteComponent(5), "SHR#(.DATAWIDTH(64)) SHR_0({greg},{{63{dEQe[0]}},dEQe},{zrin});");
   EXPECT_EQ(myParser.WriteComponent(6), "SHL#(.DATAWIDTH(32)) SHL_0({hreg},{{31{dLTe[0]}},dLTe},{xrin});");
   EXPECT_EQ(myParser.WriteComponent(7), "DIV#(.DATAWIDTH(64)) DIV_0($signed({a}),$signed({b}),{e});");
   EXPECT_EQ(myParser.WriteComponent(8), "MOD#(.DATAWIDTH(64)) MOD_0($signed({a}),$signed({b}),{g});");
   EXPECT_EQ(myParser.WriteComponent(9), "INC#(.DATAWIDTH(64)) INC_0({c},{f});");
   EXPECT_EQ(myParser.WriteComponent(10), "DEC#(.DATAWIDTH(64)) DEC_0({a},{e});");
   EXPECT_EQ(myParser.WriteWire(0), "wire [63:0] e,f,g,zwire,xwire;");
   EXPECT_THROW(myParser.AddComponent("d = bob + cat"), string);

}

TEST(FileReader, GetLine)
{
   FileReader myReader("474a_circuit1.txt");
   EXPECT_EQ(myReader.GetLine(), "input Int8 a, b, c");
   EXPECT_EQ(myReader.GetLine(), "");
   EXPECT_EQ(myReader.GetLine(), "output Int8 z");
   EXPECT_EQ(myReader.GetLine(), "output Int16 x");
   EXPECT_EQ(myReader.GetLine(), "");
   EXPECT_EQ(myReader.GetLine(), "wire Int8 d, e");
   EXPECT_EQ(myReader.GetLine(), "wire Int16 f, g");
   EXPECT_EQ(myReader.GetLine(), "wire Int16 xwire");
   EXPECT_EQ(myReader.GetLine(), "");
   EXPECT_EQ(myReader.GetLine(), "d = a + b");
   EXPECT_EQ(myReader.GetLine(), "e = a + c");
   EXPECT_EQ(myReader.GetLine(), "g = d > e");
   EXPECT_EQ(myReader.GetLine(), "z = g ? d : e");
   EXPECT_EQ(myReader.GetLine(), "f = a * c");
   EXPECT_EQ(myReader.GetLine(), "xwire = f - d  ");
   EXPECT_EQ(myReader.GetLine(), "x = xwire");
}

TEST(IntfParser_FileReader, GetLineConvertToLine)
{
   FileReader myReader("474a_circuit1.txt");
   IntfParser myParser;
   while (myReader.Done() != true)
   {
      myParser.Convert(myReader.GetLine());   
   }
   EXPECT_EQ(myReader.Done(), true); 
   myParser.GenerateOutput("circuit1");   

}

TEST(Djikstra, Init)
{
   Djikstra myDjik;
   EXPECT_EQ(LATENCY[MUL][_64BIT], (float)15.354);
   FileReader myReader("474a_circuit1.txt");
   IntfParser myParser;
   while (myReader.Done() != true)
   {
      myParser.Convert(myReader.GetLine());
   }
   for (int i = 0; i < (int)myParser.inputs.size(); i++)
   {
      Component* component = &myParser.inputs[i];
      myDjik.AddComponent(component);
   }
   
   /*for (int i = 0; i < myParser.outputs.size(); i++)
   {
      Component* component = &myParser.outputs[i];
      myDjik.AddComponent(component);
   }*/
   for (int i = 0; i < (int)myParser.components.size(); i++)
   {
      Component* component = myParser.components[i];
      
      myDjik.AddComponent(component); 
   }
   for (int i = 0; i < (int)myParser.outputs.size(); i++)
   {
      Component* component = &myParser.outputs[i];
      myDjik.AddComponent(component);
   }
   Node * node = myDjik.FindNode("a");
   node->distance = 0;
   myDjik.VisitNode("a");
   std::cout <<   myDjik.GreatestLatency() << std::endl;

/*
   std::vector<Node> tempNodes = myDjik.nodes;

   for (int i = 0; i < (int)tempNodes.size(); i++)
   {
      std::cout << "Node name: " << tempNodes[i].name << " Node type: " << tempNodes[i].type << std::endl;
      if (tempNodes[i].type == INPUT)
	      myDjik.PurgeNode(&myDjik.nodes, tempNodes[i].name);
   }
 */
   myDjik.Clean();
//   myDjik.ResetVisited(&myDjik.nodes);
   myDjik.ComputeAsapTime(&myDjik.nodes);
   myDjik.ComputeAlapTime(&myDjik.nodes, 20);
   myDjik.ComputeFrames(&myDjik.nodes);
   myDjik.ComputeDistance(&myDjik.nodes, 20);

   
   for (int i = 0; i < (int)myDjik.nodes.size(); i++)
   {
      myDjik.nodes[i].Time = get<0>(myDjik.ComputeForce(&myDjik.nodes, &myDjik.nodes[i], 0, true, true));
      myDjik.nodes[i].asapTime = myDjik.nodes[i].Time;
      myDjik.nodes[i].alapTime = myDjik.nodes[i].Time;
      myDjik.nodes[i].prob = 1.0;
      myDjik.ComputeDistance(&myDjik.nodes, 20);
   }

   for (int i = 0; i < (int)myDjik.nodes.size(); i++)
   {
      myDjik.PrintNode(myDjik.nodes[i]);
   }
}
