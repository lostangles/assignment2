#include "IntfParser.h"

using namespace std;

IntfParser::IntfParser()
{

}

std::string IntfParser::Convert(std::string line)
{

   return "";
}

int IntfParser::GetPortSize(std::string line)
{
   const char * tokenArr = line.c_str();
   string size;
   for (int i = 0; i < line.size(); i++)
   {
      if ( ((tokenArr[i] - '0') >= 0) && ((tokenArr[i] - '0') <= 9) )
      {
         size += tokenArr[i];
      }
   }
   return stoi(size) - 1;
}

std::vector<string> IntfParser::GetPortNames(std::string line)
{
   vector<string> names;
   string port;
   bool portFound = false;
   char delim = ' ';
   int skipTwo = 0;
   stringstream myStream(line);
   while(getline(myStream, port, delim ))
   {
      //Start collecting port names
      if ( (port.find("input") != std::string::npos) || (port.find("output") != std::string::npos) )
      {
         portFound = true;
         continue;
      }
      else if (!portFound)
      {
         continue;
      }
      if ( (skipTwo++ < 1) && portFound )
      {
         continue;
      }
      if ( port.find(",") != std::string::npos )
      {
         names.push_back(port.substr(0, port.size() - 1) );
      }
      else
      {
         names.push_back(port);
      }
   }

   
   return names;
}

std::string IntfParser::CreateModule(std::string moduleName, std::string file)
{
   
   vector <string> outputVector;
   stringstream fileStream(file);
   string token;
   string outputStr;
   
   while(getline(fileStream, token))
   {
      //Find all inputs
      if (token.find("input") != std::string::npos)
      {
         string inputStr;
         string inputPorts;
         //Extract the size
         int size = GetPortSize(token);
         
         //Extract the port names
         vector<string> portNames = GetPortNames(token);
         
         //Format the string
         int numPorts = portNames.size();
         for (int i = 0; i < numPorts; i++)
         {
            inputStr = "input ["; 
            inputStr += to_string(size);
            inputStr += ":0] ";
            inputStr += portNames.at(i);
            if (i != (numPorts - 1) )
            {
               inputStr += ", ";
            }
            inputPorts += inputStr;
         }
         
         outputVector.push_back(inputPorts);
      }

      //Find all outputs
      if (token.find("output") != std::string::npos)
      {
         string outputStr2;
         string outputPorts;
         outputPorts = ", ";
         //Extract the size
         int size = GetPortSize(token);
         
         //Extract the port names
         vector<string> portNames = GetPortNames(token);
         
         //Format the string
         int numPorts = portNames.size();
         for (int i = 0; i < numPorts; i++)
         {
            outputStr2 = "output ["; 
            outputStr2 += to_string(size);
            outputStr2 += ":0] ";
            outputStr2 += portNames.at(i);
            if (i != (numPorts - 1) )
            {
               outputStr2 += ", ";
            }
            outputPorts += outputStr2;
         }
         
         outputVector.push_back(outputPorts);

      }
   }
   
   outputStr += "module ";   
   outputStr += moduleName;
   outputStr += "(";

   for(int i = 0; i < outputVector.size(); i++)
   {
      outputStr += outputVector[i];     
   }
  
   outputStr += ");"; 
     
   return outputStr;
}

Component_Type_e IntfParser::GetType(std::string line)
{
   Component_Type_e type;
  
   if (line.find("wire") != std::string::npos)
   {
      if (line.find("Int") != std::string::npos)
      {

      }
      else
      {
         type = Component_Type_e::WIRE;  
      }
   }
 
   if (line.find("register") != std::string::npos)
   {
      type = Component_Type_e::REG;  
   }
   if (line.find("+") != std::string::npos)
   {
      type = Component_Type_e::ADD;  
   }

   if (line.find("-") != std::string::npos)
   {
      type = Component_Type_e::SUB;  
   }

   if (line.find("*") != std::string::npos)
   {
      type = Component_Type_e::MUL;  
   }

   if (line.find(">") != std::string::npos)
   {
      type = Component_Type_e::COMP;  
   }
   if (line.find("==") != std::string::npos)
   {
      type = Component_Type_e::COMP;  
   }
   if (line.find("<") != std::string::npos)
   {
      type = Component_Type_e::COMP;  
   }

   if (line.find("?") != std::string::npos)
   {
      type = Component_Type_e::MUX2x1;  
   }

   if (line.find(">>") != std::string::npos)
   {
      type = Component_Type_e::SHR;  
   }

   if (line.find("<<") != std::string::npos)
   {
      type = Component_Type_e::SHL;  
   }

   if (line.find("%") != std::string::npos)
   {
      type = Component_Type_e::MOD;  
   }

   if (line.find("/") != std::string::npos)
   {
      type = Component_Type_e::DIV;  
   }

   if (line.find("+ 1") != std::string::npos)
   {
      type = Component_Type_e::INC;  
   }

   if (line.find("- 1") != std::string::npos)
   {
      type = Component_Type_e::DEC;  
   }
   
   if (line.find("output") != std::string::npos)
   {
      type = Component_Type_e::OUTPUT;  
   }

   if (line.find("input") != std::string::npos)
   {
      type = Component_Type_e::INPUT;  
   }



   return type;
}

void Component::ParsePorts(string line)
{

   string port;
   bool assignmentFound = false;
   int firstPortFound = 0;
   char delim = ' ';
   bool oneShot = false;
   string lastToken;
   stringstream myStream(line);
   while(getline(myStream, port, delim ))
   {
      //Start collecting port names
      if ( port.find("=") != std::string::npos ) 
      {
         output = lastToken;
         assignmentFound = true;

         continue;
      }
      if (assignmentFound && !oneShot)
      {
         inputA = port;
         oneShot = true;
      }

      lastToken = port;
   }

   inputB = lastToken;


}

void ComponentCOMP::ParsePorts(string line)
{
   equal = "";
   greaterThan = "";
   lessThan = "";
   reinterpret_cast<Component*>(this)->ParsePorts(line);   
   
   if ( line.find("==") != std::string::npos ) 
   {
      equal = output;
   }

   if ( line.find(">") != std::string::npos ) 
   {
      greaterThan = output;
   }

   
   if ( line.find("<") != std::string::npos ) 
   {
      lessThan = output;
   }
}

void ComponentMUX2x1::ParsePorts(string line)
{

   string port;
   bool assignmentFound = false;
   int firstPortFound = 0;
   char delim = ' ';
   int twoShot = 0;
   string lastToken;
   stringstream myStream(line);
   while(getline(myStream, port, delim ))
   {
      //Start collecting port names
      if ( port.find("=") != std::string::npos ) 
      {
         output = lastToken;
         assignmentFound = true;

         continue;
      }
      if (twoShot == 0)
      {
         switchInput = port;
      }
      if (assignmentFound && (twoShot++ < 3) )
      {
         inputA = port;
      }

      lastToken = port;
   }

   inputB = lastToken;

}

void ComponentWIRE::ParsePorts(string line)
{

}
