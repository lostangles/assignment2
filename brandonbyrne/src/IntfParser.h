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
   OUTPUT,
   FF,
   NO_TYPE_FOUND
} ;

using namespace std;

static int numAdd = 0;
static int numSub = 0;
static int numMul = 0;
static int numComp = 0;
static int numMux = 0;
static int numShr = 0;
static int numShl = 0;
static int numDiv = 0;
static int numMod = 0;
static int numInc = 0;
static int numDec = 0;
static int numReg = 0;

class Component;
class ComponentREG;
class ComponentWIRE;
class ComponentINPUT;
class ComponentOUTPUT;

class IntfParser
{
   public:
	IntfParser();
	Component_Type_e GetType(std::string line);
        std::string CreateModule(std::string moduleName, std::string moduleLines);
        int GetPortSize(std::string line);
        std::vector<std::string> GetPortNames(std::string line);
        std::string Convert(std::string line);
        bool AddComponent(std::string line);
        bool AddPorts(std::string line);
        int FindPortSize(std::string port);
        std::string WriteComponent(int number);
        std::string WriteWire(int number);
        std::string WriteReg(int number); 
        string GenerateOutput(std::string moduleName);

        std::vector<ComponentWIRE> wires;
        std::vector<ComponentINPUT> inputs;
        std::vector<ComponentOUTPUT> outputs;
        std::vector<ComponentREG> registers;
        std::vector<Component*> components;
   private:
	bool CheckForError();
        stringstream stringInputs;
        stringstream stringOutputs;


};

class Component
{
   public:
	virtual std::string ComponentToLine() = 0;
	Component(){};

        Component(int size) { this->size = size; }
        void ParsePorts(std::string line);
        std::string pad_zeros(std::string port);
        int size;
        int inputSizeA;
        int inputSizeB;
        std::string output;
        std::string inputA;
        std::string inputB;
        

};

class ComponentINPUT : public Component
{
   public:
	std::string ComponentToLine()
	{
           string outputLine;
           for (int i = 0; i < (int)ports.size(); i++)
           {
              outputLine += "\tinput [";
              outputLine += to_string(size);
              outputLine += ":0] ";
              outputLine += ports.at(i);
              outputLine += ",\n";
           }
	   return outputLine;
	}
	ComponentINPUT(){};
        ComponentINPUT(int size) { this->size = size; }
        void ParsePorts(std::string line);
        std::vector<std::string> ports;
};

class ComponentOUTPUT : public Component
{
   public:
	std::string ComponentToLine()
	{
           string outputLine;
           for (int i = 0; i < (int)ports.size(); i++)
           {
              outputLine += "\toutput [";
              outputLine += to_string(size);
              outputLine += ":0] ";
              outputLine += ports.at(i);
              outputLine += ",\n";
           }
	   return outputLine;
	}
        ComponentOUTPUT(int size) { this->size = size; }
	ComponentOUTPUT(){};
        void ParsePorts(std::string line);
        std::vector<std::string> ports;
};

class ComponentWIRE : public Component
{
   public:
	std::string ComponentToLine()
	{
           string outputLine;
           outputLine += "wire [";
           outputLine += to_string(size);
           outputLine += ":0] ";
           for (int i = 0; i < (int)wires.size(); i++)
           {
              outputLine += wires.at(i);
              if ( i != ((int)wires.size() - 1))
              {
                 outputLine += ",";
              }
           }
           outputLine += ";";
	   return outputLine;
	}
	ComponentWIRE(){};
        ComponentWIRE(int size) { this->size = size; }
        void ParsePorts(std::string line);
        std::vector<std::string> wires;
};

class ComponentREG : public Component
{
   public:
	std::string ComponentToLine()
	{
	   return "";
	}
	ComponentREG(){};
        ComponentREG(int size) { this->size = size; }
        void ParsePorts(std::string line);
        std::vector<std::string> registers;

};

class ComponentFF : public Component
{
   public:
	std::string ComponentToLine()
	{
           string outputLine;
           outputLine = "REG#(.DATAWIDTH(";
           outputLine += to_string(1 + size);
           outputLine += ")) REG_";
           outputLine += to_string(numReg++);
           outputLine += "(.d({";
           outputLine += pad_zeros(inputA);
           outputLine += inputA;
           outputLine += "}),.clk(clk),.rst(rst),.q({";
           outputLine += pad_zeros(output);
           outputLine += output;
           outputLine += "}));";

	   return outputLine;
	}
	ComponentFF(){};
        ComponentFF(int size) { this->size = size; }

};

class ComponentADD : public Component
{
   public:
	std::string ComponentToLine()
	{
           string outputLine;
           outputLine = "ADD#(.DATAWIDTH(";
           outputLine += to_string(1 + size);
           outputLine += ")) ADDER_";
           outputLine += to_string(numAdd++);
           outputLine += "({";
           outputLine += pad_zeros(inputA);
           outputLine += inputA;
           outputLine += "},{";
           outputLine += pad_zeros(inputB);
           outputLine += inputB;
           outputLine += "},{";
           outputLine += output;
           outputLine += "});";

	   return outputLine;
	}
	ComponentADD(){};
        ComponentADD(int size) { this->size = size; }

};

class ComponentSUB : public Component
{
   public:
	std::string ComponentToLine()
	{
           string outputLine;
           outputLine = "SUB#(.DATAWIDTH(";
           outputLine += to_string(1 + size);
           outputLine += ")) SUB_";
           outputLine += to_string(numSub++);
           outputLine += "({";
           outputLine += pad_zeros(inputA);
           outputLine += inputA;
           outputLine += "},{";
           outputLine += pad_zeros(inputB);
           outputLine += inputB;
           outputLine += "},{";
           outputLine += output;
           outputLine += "});";

	   return outputLine;
	}
	ComponentSUB(){};
        ComponentSUB(int size) { this->size = size; }

};

class ComponentMUL : public Component
{
   public:
	std::string ComponentToLine()
	{
           string outputLine;
           outputLine = "MUL#(.DATAWIDTH(";
           outputLine += to_string(1 + size);
           outputLine += ")) MUL_";
           outputLine += to_string(numMul++);
           outputLine += "({";
           outputLine += pad_zeros(inputA);
           outputLine += inputA;
           outputLine += "},{";
           outputLine += pad_zeros(inputB);
           outputLine += inputB;
           outputLine += "},{";
           outputLine += output;
           outputLine += "});";

	   return outputLine;
	}
	ComponentMUL(){};
        ComponentMUL(int size) { this->size = size; }

};

class ComponentCOMP : public Component
{
   public:
	std::string ComponentToLine()
	{
           string outputLine;
           outputLine = "COMP#(.DATAWIDTH(";
           outputLine += to_string(1 + size);
           outputLine += ")) COMP_";
           outputLine += to_string(numComp++);
           outputLine += "(.a({";
           outputLine += pad_zeros(inputA);
           outputLine += inputA;
           outputLine += "}),.b({";
           outputLine += pad_zeros(inputB);
           outputLine += inputB;
           outputLine += "}),.gt(";
           outputLine += greaterThan;
           outputLine += "),";
           outputLine += ".lt(";
           outputLine += lessThan;
           outputLine += "),.eq(";
           outputLine += equal;
           outputLine += "));";

	   return outputLine;
	}
	ComponentCOMP(){ equal = ""; lessThan = ""; greaterThan = ""; }
        ComponentCOMP(int size) { this->size = size; }
        void ParsePorts(std::string line);
        std::string equal;
        std::string lessThan;
        std::string greaterThan;

};

class ComponentMUX2x1 : public Component
{
   public:
	std::string ComponentToLine()
	{
           string outputLine;
           outputLine = "MUX2x1#(.DATAWIDTH(";
           outputLine += to_string(1 + size);
           outputLine += ")) MUX_";
           outputLine += to_string(numMux++);
           outputLine += "(.a({";
           outputLine += pad_zeros(inputA);
           outputLine += inputA;
           outputLine += "}),.b({";
           outputLine += pad_zeros(inputB);
           outputLine += inputB;
           outputLine += "}),.sel(";
           outputLine += switchInput;
           outputLine += "),.d({";
           outputLine += output;
           outputLine += "}));";

	   return outputLine;
	}
	ComponentMUX2x1(){};
        ComponentMUX2x1(int size) { this->size = size; }
        void ParsePorts(std::string line);
        std::string switchInput;

};

class ComponentSHR : public Component
{
   public:
	std::string ComponentToLine()
	{
           string outputLine;
           outputLine = "SHR#(.DATAWIDTH(";
           outputLine += to_string(1 + size);
           outputLine += ")) SHR_";
           outputLine += to_string(numShr++);
           outputLine += "({";
           outputLine += pad_zeros(inputA);
           outputLine += inputA;
           outputLine += "},{";
           outputLine += pad_zeros(inputB);
           outputLine += inputB;
           outputLine += "},{";
           outputLine += output;
           outputLine += "});";

	   return outputLine;
	}
	ComponentSHR(){};
        ComponentSHR(int size) { this->size = size; }

};

class ComponentSHL : public Component
{
   public:
	std::string ComponentToLine()
	{
           string outputLine;
           outputLine = "SHL#(.DATAWIDTH(";
           outputLine += to_string(1 + size);
           outputLine += ")) SHL_";
           outputLine += to_string(numShl++);
           outputLine += "({";
           outputLine += pad_zeros(inputA);
           outputLine += inputA;
           outputLine += "},{";
           outputLine += pad_zeros(inputB);
           outputLine += inputB;
           outputLine += "},{";
           outputLine += output;
           outputLine += "});";

	   return outputLine;
	}
	ComponentSHL(){};
        ComponentSHL(int size) { this->size = size; }

};

class ComponentDIV : public Component
{
   public:
	std::string ComponentToLine()
	{
           string outputLine;
           outputLine = "DIV#(.DATAWIDTH(";
           outputLine += to_string(1 + size);
           outputLine += ")) DIV_";
           outputLine += to_string(numDiv++);
           outputLine += "({";
           outputLine += pad_zeros(inputA);
           outputLine += inputA;
           outputLine += "},{";
           outputLine += pad_zeros(inputB);
           outputLine += inputB;
           outputLine += "},{";
           outputLine += output;
           outputLine += "});";

	   return outputLine;
	}
	ComponentDIV(){};
        ComponentDIV(int size) { this->size = size; }

};

class ComponentMOD : public Component
{
   public:
	std::string ComponentToLine()
	{
           string outputLine;
           outputLine = "MOD#(.DATAWIDTH(";
           outputLine += to_string(1 + size);
           outputLine += ")) MOD_";
           outputLine += to_string(numMod++);
           outputLine += "({";
           outputLine += pad_zeros(inputA);
           outputLine += inputA;
           outputLine += "},{";
           outputLine += pad_zeros(inputB);
           outputLine += inputB;
           outputLine += "},{";
           outputLine += output;
           outputLine += "});";

	   return outputLine;
	}
	ComponentMOD(){};
        ComponentMOD(int size) { this->size = size; }

};

class ComponentINC : public Component
{
   public:
	std::string ComponentToLine()
	{
           string outputLine;
           outputLine = "INC#(.DATAWIDTH(";
           outputLine += to_string(1 + size);
           outputLine += ")) INC_";
           outputLine += to_string(numInc++);
           outputLine += "({";
           outputLine += pad_zeros(inputA);
           outputLine += inputA;
           outputLine += "},{";
           outputLine += output;
           outputLine += "});";
	   return outputLine;
	}
	ComponentINC(){};
        ComponentINC(int size) { this->size = size; }

};

class ComponentDEC : public Component
{
   public:
	std::string ComponentToLine()
	{
           string outputLine;
           outputLine = "DEC#(.DATAWIDTH(";
           outputLine += to_string(1 + size);
           outputLine += ")) DEC_";
           outputLine += to_string(numDec++);
           outputLine += "({";
           outputLine += pad_zeros(inputA);
           outputLine += inputA;
           outputLine += "},{";
           outputLine += output;
           outputLine += "});";
	   return outputLine;
	}
	ComponentDEC(){};
        ComponentDEC(int size) { this->size = size; }

};
