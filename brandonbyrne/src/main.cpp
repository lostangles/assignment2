#include <iostream>
#include "FileReader.h"
#include "FileWriter.h"
#include "IntfParser.h"

int main(int argc, char * argv[])
{
   if (argc != 3)
   {
   
   }
   std::string input(argv[1]);
   std::string output(argv[2]);
   FileReader myReader(input);
   FileWriter myWriter(output);
   IntfParser myParser; 
   
   try 
   {
      while( !myReader.Done() )
      {
         myParser.Convert(myReader.GetLine()); 
      }
      output.pop_back();
      output.pop_back();
      myWriter.WriteLine(myParser.GenerateOutput(output));
   }
   catch (std::string exception) 
   {
      std::cout << "Exception caught: " << exception << std::endl;
   }

   std::cout << "Hello world" << std::endl;
   return 0;
}
