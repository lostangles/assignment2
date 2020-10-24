#include <string>
#include <bits/stdc++.h>
enum Component_Type_e
{
   REG,
   ADD,
   SUB,
   MUL,
   COMP,
   MUT2x1,
   SHR,
   SHL,
   DIV,
   MOD,
   INC,
   DEC
} ;

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
};

class Component
{
   public:
	virtual std::string ComponentToLine(std::string componentLine) = 0;
	Component(){};

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

};

class ComponentMUX2x1 : public Component
{
   public:
	std::string ComponentToLine(std::string componentLine)
	{

	   return "";
	}

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
