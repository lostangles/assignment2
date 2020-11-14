#include <iostream>
#include "FileReader.h"
#include "FileWriter.h"
#include "IntfParser.h"
#include "Djikstra.h"

int main(int argc, char * argv[])
{
   if (argc != 3)
   {
      std::cout << "Usage: ./dpgen inputNetlist output.v" << std::endl;   
      return -1;
   }
   std::string input(argv[1]);
   std::string output(argv[2]);
   FileReader myReader(input);
   FileWriter myWriter(output);
   IntfParser myParser; 
   Djikstra myDjik;
   
   try 
   {
      while( !myReader.Done() )
      {
         myParser.Convert(myReader.GetLine()); 
      }
      output.pop_back();
      output.pop_back();
      myWriter.WriteLine(myParser.GenerateOutput(output));
      
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
      std::cout << "Critical Path : " << myDjik.GreatestLatency() << std::endl;

   }
   catch (std::string exception) 
   {
      std::cout << "Exception caught: " << exception << std::endl;
   }
   catch (...)
   {
      std::cout << "Unhandled exception caught" << std::endl;
   }

   return 0;
}
