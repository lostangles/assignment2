#include <iostream>
#include "FileReader.h"
#include "FileWriter.h"
#include "IntfParser.h"

int main()
{
   FileReader myReader("test");
   FileWriter myWriter("test");
   IntfParser myParser; 
   
   try 
   {
      while( !myReader.Done() )
      {
         myWriter.WriteLine( myParser.CreateModule( "circuit1" , myReader.ParseForModule() ) );
         myWriter.WriteLine( myParser.Convert( myReader.GetLine() ) );
      }
   }
   catch (std::string exception) 
   {
      std::cout << "Exception caught: " << exception << std::endl;
   }

   std::cout << "Hello world" << std::endl;
   return 0;
}
