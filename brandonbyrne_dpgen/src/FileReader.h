#include <iostream>
#include <string>
#include <fstream>
class FileReader
{
   public:
	FileReader(std::string fileName);
        ~FileReader();
	std::string GetLine();
        bool Done();
        std::string ParseForModule();

   private:
	bool done;
        std::ifstream* myStream;


};
