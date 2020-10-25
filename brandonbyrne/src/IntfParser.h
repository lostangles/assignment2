#include <string>
#include <bits/stdc++.h>
enum Component_Type_e
{
   REG,
   ADD,
   SUB,
   MUL,
   COMP,
   MUX2x1,
   SHR,
   SHL,
   DIV,
   MOD,
   INC,
   DEC,
   WIRE,
   INPUT,
   OUTPUT
} ;


class Component;

class IntfParser
{
   public:
	IntfParser();
	Component_Type_e GetType(std::string line);
        std::string CreateModule(std::string moduleName, std::string moduleLines);
        int GetPortSize(std::string line);
        std::vector<std::string> GetPortNames(std::string line);
        std::string Convert(std::string line);
   private:
	bool CheckForError();
        std::vector<Component> components;
};

class Component
{
   public:
	virtual std::string ComponentToLine(std::string componentLine) = 0;
	Component(){};

        Component(int size) { this->size = size; }
        void ParsePorts(std::string line);
        int size;
        std::string output;
        std::string inputA;
        std::string inputB;

};

class ComponentINPUT : public Component
{
   public:
	std::string ComponentToLine(std::string componentLine)
	{
	   return "";
	}

};

class ComponentOUTPUT : public Component
{
   public:
	std::string ComponentToLine(std::string componentLine)
	{
	   return "";
	}

};

class ComponentWIRE : public Component
{
   public:
	std::string ComponentToLine(std::string componentLine)
	{
	   return "";
	}
        ComponentWIRE(int size) { this->size = size; }
        void ParsePorts(std::string line);
        std::vector<std::string> wires;
};

class ComponentREG : public Component
{
   public:
	std::string ComponentToLine(std::string componentLine)
	{
	   return "";
	}

};

class ComponentADD : public Component
{
   public:
	std::string ComponentToLine(std::string componentLine) 
	{

	   return "";
	}

};

class ComponentSUB : public Component
{
   public:
	std::string ComponentToLine(std::string componentLine)
	{

	   return "";
	}

};

class ComponentMUL : public Component
{
   public:
	std::string ComponentToLine(std::string componentLine)
	{

	   return "";
	}

};

class ComponentCOMP : public Component
{
   public:
	std::string ComponentToLine(std::string componentLine)
	{

	   return "";
	}
   void ParsePorts(std::string line);
   std::string equal;
   std::string lessThan;
   std::string greaterThan;

};

class ComponentMUX2x1 : public Component
{
   public:
	std::string ComponentToLine(std::string componentLine)
	{

	   return "";
	}
   void ParsePorts(std::string line);
   std::string switchInput;

};

class ComponentSHR : public Component
{
   public:
	std::string ComponentToLine(std::string componentLine)
	{

	   return "";
	}

};

class ComponentSHL : public Component
{
   public:
	std::string ComponentToLine(std::string componentLine)
	{

	   return "";
	}

};

class ComponentDIV : public Component
{
   public:
	std::string ComponentToLine(std::string componentLine)
	{

	   return "";
	}

};

class ComponentMOD : public Component
{
   public:
	std::string ComponentToLine(std::string componentLine)
	{

	   return "";
	}

};

class ComponentINC : public Component
{
   public:
	std::string ComponentToLine(std::string componentLine) 
	{

	   return "";
	}

};

class ComponentDEC : public Component
{
   public:
	std::string ComponentToLine(std::string componentLine) 
	{

	   return "";
	}

};
