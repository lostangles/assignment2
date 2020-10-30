#include <iostream>
#include <string>
#include <fstream>
class FileWriter
{
   public:
	FileWriter(std::string fileName);
        ~FileWriter();
	bool WriteLine(std:: string);
   private:
        std::ofstream* myStream;


};
