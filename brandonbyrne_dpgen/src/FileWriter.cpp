#include "FileWriter.h"

FileWriter::FileWriter(std::string fileName)
{
   myStream = new std::ofstream;
   myStream->open(fileName);
}

void FileWriter::CloseWriter()
{
   closed = true;
   myStream->close();
   delete myStream;
}

FileWriter::~FileWriter()
{ 
   if (!closed)
   {
      myStream->close();
      delete myStream;
   }
}

bool FileWriter::WriteLine(std::string line)
{
   *myStream << line; 
   return true;
}
