#include <iostream>
#include <string>
class FileReader
{
   public:
	FileReader(std::string fileName);
	std::string GetLine();
        bool Done();

   private:
	bool done;


};
