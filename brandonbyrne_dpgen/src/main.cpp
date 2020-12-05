#include <iostream>
#include "FileReader.h"
#include "FileWriter.h"
#include "IntfParser.h"
#include "Djikstra.h"

int countBits(int n)
{
	int count=0;
	// While loop will run until we get n = 0
	while(n)
	{
		count++;
		// We are shifting n to right by 1
		// place as explained above
		n=n>>1;
	}
	return count;
}

int main(int argc, char * argv[])
{
   if (argc != 4)
   {
      std::cout << "Usage: ./dpgen inputNetlist latency output.v" << std::endl;   
      return -1;
   }
   std::string input(argv[1]);
   std::string output(argv[3]);
   FileReader myReader(input);
   FileWriter myWriter(output);
   stringstream fOut;
   IntfParser myParser; 
   Djikstra myDjik;
   int cycles = atoi(argv[2]);
   
   try 
   {
      while( !myReader.Done() )
      {
         myParser.Convert(myReader.GetLine()); 
      }
      output.pop_back();
      output.pop_back();
      //myWriter.WriteLine(myParser.GenerateOutput(output));
      
      for (int i = 0; i < (int)myParser.inputs.size(); i++)
      {
         Component* component = &myParser.inputs[i];
         myDjik.AddComponent(component);
      } 
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
//      std::cout << "Critical Path : " << myDjik.GreatestLatency() << " ns" << std::endl;
//
   fOut << myParser.GenerateOutput("HLSM");
   fOut <<   myParser.CreateState(countBits(cycles + 2) - 1) ;
   for (int i = 0; i < (int)myParser.wires.size(); i++)
   {
      for (int j = 0; j < (int)myParser.wires[i].wires.size() ; j++)
         fOut << "\t\t\t" << myParser.wires[i].wires[j] << " <= 0;" << std::endl;
   }
   for (int i = 0; i < (int)myParser.outputs.size(); i++)
   {
      for (int j = 0; j < (int)myParser.outputs[i].ports.size() ; j++)
         fOut << "\t\t\t" << myParser.outputs[i].ports[j] << " <= 0;" << std::endl;
   }

   fOut << "\t\tend\n\t\telse begin\n\t\tcase (State)\n" ;
   fOut << "\t\t\t0 : begin\n\t\t\t\tif (Start) State <= 1;\n\t\t\t\telse State <= 0;\n\t\t\tend";




   fOut << endl;
   myDjik.Clean();
   myDjik.ComputeAsapTime(&myDjik.nodes);
   myDjik.ComputeAlapTime(&myDjik.nodes, cycles );
   myDjik.ComputeFrames(&myDjik.nodes);
   myDjik.ComputeDistance(&myDjik.nodes, cycles );

   for (int i = 0; i < (int)myDjik.nodes.size(); i++)
   {
      myDjik.nodes[i].Time = get<0>(myDjik.ComputeForce(&myDjik.nodes, &myDjik.nodes[i], 0, true, true));
      myDjik.nodes[i].asapTime = myDjik.nodes[i].Time;
      myDjik.nodes[i].alapTime = myDjik.nodes[i].Time;
      myDjik.nodes[i].prob = 1.0;
      myDjik.ComputeDistance(&myDjik.nodes, cycles);
   }

   int stateCount = 1;
   for (stateCount = 1; stateCount < cycles+1; stateCount++)
   {
      fOut << "\t\t\t" << stateCount << " : begin\n";
      for (int j = 0; j < (int)myDjik.nodes.size(); j++)
      {
         if (myDjik.nodes[j].Time == stateCount)
	 {
            if (myDjik.nodes[j].ifOp)
	    {
               fOut << "\t\t\t\tif ( " << myDjik.nodes[j].ifCondition << " ) begin\n";
	    }
            fOut << "\t\t\t\t" << myDjik.nodes[j].line << ";" << std::endl;
            if (myDjik.nodes[j].ifOp)
	    {
               fOut << "\t\t\t\tend\n";
	    }
            
	 }
	 else if ( myDjik.nodes[j].Time == -1 )
	 {
            throw string("Could not meet latency constraint\n");
	 }
      }
      fOut << "\t\t\t\t" << "State <= " << stateCount+1 << ";\n\t\t\tend" << std::endl;
   }
   fOut << "\t\t\t" << stateCount  << " : begin\n\t\t\t\tDone <= 1;\n\t\t\t\tState <= 0;\n\t\t\tend" << std::endl;
   fOut << "\t\tendcase\n\t\tend\n\tend\nendmodule" << std::endl;

   for (int i = 0; i < (int)myDjik.nodes.size(); i++)
   {
//      myDjik.PrintNode(myDjik.nodes[i]);
   }

   myWriter.WriteLine(fOut.str());

   }
   catch (std::string exception) 
   {
      std::cout << "Exception caught: " << exception << std::endl;
   }
   catch (...)
   {
      std::cout << "Unhandled exception caught" << std::endl;
   }
   myWriter.CloseWriter();
   exit(0);
   return 0;
}
