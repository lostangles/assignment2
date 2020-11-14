#include "FileReader.h"

FileReader::FileReader(std::string fileName)
{
   myStream = new std::ifstream(fileName);
   done = false;
}

FileReader::~FileReader()
{
   delete myStream;
}

std::string FileReader::GetLine()
{
   std::string line;
   std::getline(*myStream, line);
   if (myStream->eof() )
   {
      done = true;
   }
   return line;
}

std::string FileReader::ParseForModule()
{

   return "";
}

bool FileReader::Done()
{
   return done;
}
