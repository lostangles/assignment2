#include "FileWriter.h"

FileWriter::FileWriter(std::string fileName)
{
   myStream = new std::ofstream;
   myStream->open(fileName);
}

FileWriter::~FileWriter()
{
   myStream->close();
   delete myStream;
}

bool FileWriter::WriteLine(std::string line)
{
   *myStream << line; 
   return true;
}
