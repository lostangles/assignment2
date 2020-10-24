#include <iostream>
#include <string>
class FileReader
{
   public:
	FileReader(std::string fileName);
	std::string GetLine();
        bool Done();
        std::string ParseForModule();

   private:
	bool done;


};
