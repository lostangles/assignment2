#include "IntfParser.h"

using namespace std;

IntfParser::IntfParser()
{
   emptyFile = true;
}

std::string IntfParser::Convert(std::string line)
{
   stringstream myStream(line);
   string firstToken = "";
   getline(myStream, firstToken, ' ');
   if (
        (firstToken.compare("input") == 0) ||
        (firstToken.compare("output") == 0) ||
        (firstToken.compare("register") == 0) ||
        (firstToken.compare("wire") == 0) 
      )
   {
      AddPorts(line);
      emptyFile = false;
   }
   else
   {
      AddComponent(line);
   }
   if (emptyFile)
   {
      throw string("No ports/components found in input netlist");
   }
   stringInputs << line;
   return "";
}

string IntfParser::GenerateOutput(string moduleName)
{
   if (emptyFile)
   {
      throw string("No ports/components found in input netlist");
   }
   string line = "";
   line += "module ";
   line += moduleName;
   line += "(\n";
   
   line += "\tinput clk,\n\tinput rst,\n";
 
   for (int i = 0; i < (int)inputs.size(); i++)
   {
      line += inputs.at(i).ComponentToLine();
   }
   line += "\n";
   for (int i = 0; i < (int)outputs.size(); i++)
   {
      line += outputs.at(i).ComponentToLine();
   }
   line.pop_back();
   line.pop_back();
   line += ");\n\n";

   for (int i = 0; i < (int)wires.size(); i++)
   {
      line += wires.at(i).ComponentToLine();
      line += "\n";
   }

   line += "\n";

   for (int i = 0; i < (int)registers.size(); i++)
   {
      line += registers.at(i).ComponentToLine();
      line += "\n";
   }

   

   for (int i = 0; i < (int)components.size(); i++)
   {
      line += components.at(i)->ComponentToLine();
      line += "\n";
   }
   
   
   line += "\nendmodule";

   cout << line << endl;
   return line;
}

int IntfParser::GetPortSize(std::string line)
{
   const char * tokenArr = line.c_str();
   string size = "0";
   bool firstNumberFound = false;
   for (uint32_t i = 0; i < line.size(); i++)
   {
      if ( ((tokenArr[i] - '0') >= 0) && ((tokenArr[i] - '0') <= 9) )
      {
         size += tokenArr[i];
         firstNumberFound = true;
      }
      else if (firstNumberFound)
      {
         //Not a number, done
         break;
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

   for(uint32_t i = 0; i < outputVector.size(); i++)
   {
      outputStr += outputVector[i];     
   }
  
   outputStr += ");"; 
     
   return outputStr;
}

Component_Type_e IntfParser::GetType(std::string line)
{
   Component_Type_e type = NO_TYPE_FOUND;
  
   if (line.find("wire") != std::string::npos)
   {
      if (line.find("Int") != std::string::npos)
      {
         type = Component_Type_e::WIRE;  
      }
      //Handle 1 bit wires
      stringstream myStream(line);
      string temp;
      getline(myStream, temp, ' ');
      if (temp.compare("wire") == 0)
      {
         type = Component_Type_e::WIRE;
      }
   }

   if (line.find("=") != std::string::npos)
   {
      type = Component_Type_e::FF;
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

   if (line.find("$") != std::string::npos)
   {
      type = Component_Type_e::NO_TYPE_FOUND;  
   }



   return type;
}

int IntfParser::GetSigned(string line)
{
   int isSigned = 0;
   if (line.find("UInt") != std::string::npos)
   {
      isSigned = 0;      
   }
   else
   {
      isSigned = 1;
   }

   return isSigned;
}

bool IntfParser::AddPorts(string line)
{
   bool componentAdded = false;
   Component_Type_e type = GetType(line);
   if (type == Component_Type_e::NO_TYPE_FOUND)
   {
      throw "Invalid type/operator found";
   }
   int size;

   switch(type)
   {
   case REG:
   {
      size = GetPortSize(line);
      ComponentREG newREG(size);
      newREG.ParsePorts(line);
      newREG.isSigned = GetSigned(line);
      newREG.type = REG;
      registers.push_back(newREG);   
      break;
   }
   case WIRE:
   {
      size = GetPortSize(line);
      if (size == -1)
      {
         size = 1;
      }
      ComponentWIRE newWire(size);
      newWire.ParsePorts(line);
      newWire.isSigned = GetSigned(line);
      newWire.type = WIRE;
      wires.push_back(newWire);
      break;
   }
   case INPUT:
   {
      size = GetPortSize(line);
      ComponentINPUT newInput(size);
      newInput.ParsePorts(line);
      newInput.isSigned = GetSigned(line);
      newInput.type = INPUT;
      inputs.push_back(newInput);
      break;
   }
   case OUTPUT:
   {
      size = GetPortSize(line);
      ComponentOUTPUT newOutput(size);
      newOutput.ParsePorts(line);
      newOutput.isSigned = GetSigned(line);
      newOutput.type = OUTPUT;
      outputs.push_back(newOutput); 
      break;
   }
   default: break;
   }   

   return componentAdded;
}

int IntfParser::FindPortSize(string port)
{
   int portSize = 1;
   bool portFound = false;
   //Search through all of our different sized outputs
   for (uint32_t i = 0; i < outputs.size(); i++)
   {
      //If we have an input
      if (outputs.size() != 0)
      {
         for (uint32_t j = 0; j < outputs.at(i).ports.size(); j++)
         {
            if ( outputs.at(i).ports.at(j).compare(port) == 0)
            {
               //Port name found
               portSize = outputs.at(i).size;
               portFound = true;
            }
         }
      }
   }
   //Search through all of our different sized inputs
   for (uint32_t i = 0; i < inputs.size(); i++)
   {
      //If we have an input
      if (inputs.size() != 0)
      {
         for (uint32_t j = 0; j < inputs.at(i).ports.size(); j++)
         {
            if ( inputs.at(i).ports.at(j).compare(port) == 0)
            {
               //Port name found
               portSize = inputs.at(i).size;
               portFound = true;
            }
         }
      }
   }
   //Search through all of our different sized wires
   for (uint32_t i = 0; i < wires.size(); i++)
   {
      //If we have an input
      if (wires.size() != 0)
      {
         for (uint32_t j = 0; j < wires.at(i).wires.size(); j++)
         {
            if ( wires.at(i).wires.at(j).compare(port) == 0)
            {
               //Port name found
               portSize = wires.at(i).size;
               portFound = true;
            }
         }
      }
   }
   //Search through all of our different sized registers
   for (uint32_t i = 0; i < registers.size(); i++)
   {
      //If we have an input
      if (registers.size() != 0)
      {
         for (uint32_t j = 0; j < registers.at(i).registers.size(); j++)
         {
            if ( registers.at(i).registers.at(j).compare(port) == 0)
            {
               //Port name found
               portSize = registers.at(i).size;
               portFound = true;
            }
         }
      }
   }
   if ( (!portFound) && (port.compare("1") != 0))
   {
      string error = "";
      error += "Port ";
      error += port;
      error += " not found.";
      throw string(error);
   }
   return portSize;
}

bool IntfParser::AddComponent(string line)
{
   bool componentAdded = false;
   Component_Type_e type = GetType(line);
   int size;
   if (line.compare("") == 0)
   {
      return false;
   }
   else if (type == Component_Type_e::NO_TYPE_FOUND)
   {
      throw string("Invalid type/operator found");
   }

   switch(type)
   {
   case ADD:
   {
      ComponentADD * myComponent = new ComponentADD();
      myComponent->ParsePorts(line);
      size = FindPortSize(myComponent->output);
      myComponent->size = size;
      size = FindPortSize(myComponent->inputA);
      myComponent->inputSizeA = size;
      size = FindPortSize(myComponent->inputB);
      myComponent->inputSizeB = size;
      myComponent->type = ADD;
      components.push_back(myComponent);
      break;
   }
   case SUB:
   {
      ComponentSUB * myComponent = new ComponentSUB;
      myComponent->ParsePorts(line);
      size = FindPortSize(myComponent->output);
      myComponent->size = size;
      size = FindPortSize(myComponent->inputA);
      myComponent->inputSizeA = size;
      size = FindPortSize(myComponent->inputB);
      myComponent->inputSizeB = size;
      myComponent->type = SUB;
      components.push_back(myComponent);
      break;
   }
   case MUL:
   {
      ComponentMUL * myComponent = new ComponentMUL;
      myComponent->ParsePorts(line);
      size = FindPortSize(myComponent->output);
      myComponent->size = size;
      size = FindPortSize(myComponent->inputA);
      myComponent->inputSizeA = size;
      size = FindPortSize(myComponent->inputB);
      myComponent->inputSizeB = size;
      myComponent->type = MUL;
      components.push_back(myComponent);
      break;
   }
   case COMP:
   {
      ComponentCOMP * myComponent = new ComponentCOMP;
      myComponent->ParsePorts(line);
      size = FindPortSize(myComponent->output);
      myComponent->size = size;
      size = FindPortSize(myComponent->inputA);
      myComponent->inputSizeA = size;
      size = FindPortSize(myComponent->inputB);
      myComponent->inputSizeB = size;
      myComponent->type = COMP;
      components.push_back(myComponent);
      break;
   }
   case MUX2x1:
   {
      ComponentMUX2x1 * myComponent = new ComponentMUX2x1;
      myComponent->ParsePorts(line);
      size = FindPortSize(myComponent->output);
      myComponent->size = size;
      size = FindPortSize(myComponent->inputA);
      myComponent->inputSizeA = size;
      size = FindPortSize(myComponent->inputB);
      myComponent->inputSizeB = size;
      myComponent->type = MUX2x1;
      components.push_back(myComponent);
      break;
   }
   case SHR:
   {
      ComponentSHR * myComponent = new ComponentSHR;
      myComponent->ParsePorts(line);
      size = FindPortSize(myComponent->output);
      myComponent->size = size;
      size = FindPortSize(myComponent->inputA);
      myComponent->inputSizeA = size;
      size = FindPortSize(myComponent->inputB);
      myComponent->inputSizeB = size;
      myComponent->type = SHR;
      components.push_back(myComponent);
      break;
   }
   case SHL:
   {
      ComponentSHL * myComponent = new ComponentSHL;
      myComponent->ParsePorts(line);
      size = FindPortSize(myComponent->output);
      myComponent->size = size;
      size = FindPortSize(myComponent->inputA);
      myComponent->inputSizeA = size;
      size = FindPortSize(myComponent->inputB);
      myComponent->inputSizeB = size;
      myComponent->type = SHL;
      components.push_back(myComponent);
      break;
   }
   case DIV:
   {
      ComponentDIV * myComponent = new ComponentDIV;
      myComponent->ParsePorts(line);
      size = FindPortSize(myComponent->output);
      myComponent->size = size;
      size = FindPortSize(myComponent->inputA);
      myComponent->inputSizeA = size;
      size = FindPortSize(myComponent->inputB);
      myComponent->inputSizeB = size;
      myComponent->type = DIV;
      components.push_back(myComponent);
      break;
   }
   case MOD:
   {
      ComponentMOD * myComponent = new ComponentMOD;
      myComponent->ParsePorts(line);
      size = FindPortSize(myComponent->output);
      myComponent->size = size;
      size = FindPortSize(myComponent->inputA);
      myComponent->inputSizeA = size;
      size = FindPortSize(myComponent->inputB);
      myComponent->inputSizeB = size;
      myComponent->type = MOD;
      components.push_back(myComponent);
      break;
   }
   case INC:
   {
      ComponentINC * myComponent = new ComponentINC;
      myComponent->ParsePorts(line);
      size = FindPortSize(myComponent->output);
      myComponent->size = size;
      size = FindPortSize(myComponent->inputA);
      myComponent->inputSizeA = size;
      size = FindPortSize(myComponent->inputB);
      myComponent->inputSizeB = size;
      myComponent->type = INC;
      components.push_back(myComponent);
      break;
   }
   case DEC:
   {
      ComponentDEC * myComponent = new ComponentDEC;
      myComponent->ParsePorts(line);
      size = FindPortSize(myComponent->output);
      myComponent->size = size;
      size = FindPortSize(myComponent->inputA);
      myComponent->inputSizeA = size;
      size = FindPortSize(myComponent->inputB);
      myComponent->inputSizeB = size;
      myComponent->type = DEC;
      components.push_back(myComponent);
      break;
   }
   case FF:
   {
      ComponentFF * myComponent = new ComponentFF;
      myComponent->ParsePorts(line);
      size = FindPortSize(myComponent->output);
      myComponent->size = size;
      size = FindPortSize(myComponent->inputA);
      myComponent->inputSizeA = size;
      size = FindPortSize(myComponent->inputB);
      myComponent->inputSizeB = size;
      myComponent->type = FF;
      components.push_back(myComponent);
      break;
   }
   default: break;
   }   

   return componentAdded;
}

string IntfParser::WriteReg(int number)
{
   if (number > (int)registers.size() )
   {
      //Throw exception
   }
   return registers.at(number).ComponentToLine();
}

string IntfParser::WriteWire(int number)
{
   if (number > (int)wires.size() )
   {
      //Throw exception
   }
   return wires.at(number).ComponentToLine();
}

string IntfParser::WriteComponent(int number)
{
   if (number > (int)components.size() )
   {
      //Throw exception
   }
   return components.at(number)->ComponentToLine();
}

void Component::ParsePorts(string line)
{

   string port;
   bool assignmentFound = false;
   char delim = ' ';
   bool oneShot = false;
   string lastToken;
   stringstream myStream(line);
   while(getline(myStream, port, delim ))
   {
      //Start collecting port names
      if ( port.find("//") != std::string::npos)
      {
         break;
      }
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
      if ( port.compare("") != 0)
      {
         lastToken = port;
      }
   }

   inputB = lastToken;


}

string Component::pad_zeros(std::string port)
{
   string zeros = "";
   if (port.compare(inputA) == 0)
   {
      if (inputSizeA < size)
      {
         zeros += to_string(size - inputSizeA);
         if (isSigned)
         {
            zeros += "'b1,";
         }
         else
         {
            zeros += "'b0,";
         }
      }
   }
   if (port.compare(inputB) == 0)
   {
      if (inputSizeB < size)
      {
         zeros += to_string(size - inputSizeB);
         if (isSigned)
         {
            zeros += "'b1,";
         }
         else
         {
            zeros += "'b0,";
         }
      }
   }
   return zeros;
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

   string port;
   char delim = ' ';
   bool oneShot = false;
   stringstream myStream(line);
   while(getline(myStream, port, delim ))
   {
      //Start collecting port names
      if (oneShot == false)
      {
         oneShot = true;
         continue;
      }
      if ( port.find("Int") != std::string::npos ) 
      {
         continue;
      }/*
      else if (!portFound)
      {
         continue;
      }*/
       
      if ( port.find(",") != std::string::npos )
      {
         wires.push_back(port.substr(0, port.size() - 1) );
      }
      else
      {
         wires.push_back(port);
      }
   }

}

void ComponentREG::ParsePorts(string line)
{

   string port;
   bool portFound = false;
   char delim = ' ';
   stringstream myStream(line);
   while(getline(myStream, port, delim ))
   {
      //Start collecting port names
      if ( port.find("Int") != std::string::npos ) 
      {
         portFound = true;
         continue;
      }
      else if (!portFound)
      {
         continue;
      } 
      if ( port.find(",") != std::string::npos )
      {
         registers.push_back(port.substr(0, port.size() - 1) );
      }
      else
      {
         registers.push_back(port);
      }
   }

}

void ComponentINPUT::ParsePorts(string line)
{

   string token;
   string port;
   bool portFound = false;
   char delim = ' ';
   stringstream myStream(line);
   
   while(getline(myStream, token, delim ))
   {
      stringstream noTabs(token);
      while(getline(noTabs, port, '\t'))
      {
         //Start collecting port names
         if ( port.find("//") != std::string::npos)
         {
            break;
         }
         if ( port.find("Int") != std::string::npos ) 
         {
            portFound = true;
            continue;
         }
         else if (!portFound)
         {
            continue;
         } 
         if ( port.find(",") != std::string::npos )
         {
            ports.push_back(port.substr(0, port.size() - 1) );
         }
         else
         {
            ports.push_back(port);
         }
      }
   }
}

void ComponentOUTPUT::ParsePorts(string line)
{
   string token;
   string port;
   bool portFound = false;
   char delim = ' ';
   stringstream myStream(line);
   
   while(getline(myStream, token, delim ))
   {
      stringstream noTabs(token);
      while(getline(noTabs, port, '\t'))
      {
         //Start collecting port names
         if ( port.find("//") != std::string::npos)
         {
            break;
         }
         if ( port.find("Int") != std::string::npos ) 
         {
            portFound = true;
            continue;
         }
         else if (!portFound)
         {
            continue;
         } 
         if ( port.find(",") != std::string::npos )
         {
            ports.push_back(port.substr(0, port.size() - 1) );
         }
         else
         {
            ports.push_back(port);
         }
      }
   }
/* 
   string port;
   bool portFound = false;
   char delim = ' ';
   stringstream myStream(line);
   while(getline(myStream, port, delim ))
   {
      //Start collecting port names
      if ( port.find("Int") != std::string::npos ) 
      {
         portFound = true;
         continue;
      }
      else if (!portFound)
      {
         continue;
      } 
      if ( port.find(",") != std::string::npos )
      {
         ports.push_back(port.substr(0, port.size() - 1) );
      }
      else
      {
         ports.push_back(port);
      }
   }*/
}

BitSize_e Component::sizeToBitSize()
{
   int inputSize = inputSizeA > inputSizeB ? inputSizeA : inputSizeB;
//   inputSize = (inputSize > size) ? inputSize : size;
   if (inputSize < size)
   {
      inputSize = size;
   }
   switch (inputSize)
   {
   case 1: return _1BIT; break;
   case 2: return _2BIT; break;
   case 7: return _8BIT; break;
   case 15: return _16BIT; break;
   case 31: return _32BIT; break;
   case 63: return _64BIT; break;
   default: break;
   }
return _1BIT;
}
