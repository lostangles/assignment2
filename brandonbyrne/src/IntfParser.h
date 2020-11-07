#ifndef INTFPARSER_H
#define INTFPARSER_H

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
   NUM_COMPONENTS,
   NO_TYPE_FOUND
} ;

enum BitSize_e
{
   _1BIT,
   _2BIT,
   _8BIT,
   _16BIT,
   _32BIT,
   _64BIT,
   NUM_BYTES
} ;

const float LATENCY[NUM_COMPONENTS][NUM_BYTES] = {
//1BIT  2BIT   8BIT   16BIT  32BIT  64BIT
{2.616, 2.644, 2.879, 3.061, 3.602, 3.966}, //REG
{2.704, 3.713, 4.924, 5.638, 7.270, 9.566}, //ADD
{3.024, 3.412, 4.890, 5.569, 7.253, 9.556}, //SUB
{2.438, 3.651, 7.453, 7.811, 12.395, 15.354}, //MUL
{3.031, 3.934, 5.949, 6.256, 7.264, 8.416}, //COMP
{4.083, 4.115, 4.815, 5.623, 8.079, 8.766}, //MUX2x1
{3.644, 4.007, 5.178, 6.460, 8.819, 11.095}, //SHR
{3.614, 3.980, 5.152, 6.549, 8.565, 11.220}, //SHL
{0.619, 2.144, 15.439, 33.093, 86.312, 243.233}, //DIV
{0.758, 2.149, 16.078, 35.563, 88.142, 250.583}, //MOD
{1.792, 2.218, 3.111, 3.471, 4.347, 6.200}, //INC
{1.792, 2.218, 3.108, 3.701, 4.685, 6.503}, //DEC
{0, 0, 0, 0, 0}, //WIRE
{0, 0, 0, 0, 0}, //INPUT
{0, 0, 0, 0, 0}, //OUTPUT
{2.616, 2.644, 2.879, 3.061, 3.602, 3.966} //FF
};



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
        int GetSigned(std::string line);
	bool isComponentSigned(std::string portName);

        std::vector<ComponentWIRE> wires;
        std::vector<ComponentINPUT> inputs;
        std::vector<ComponentOUTPUT> outputs;
        std::vector<ComponentREG> registers;
        std::vector<Component*> components;
   private:
	bool CheckForError();
        stringstream stringInputs;
        stringstream stringOutputs;
        bool emptyFile;


};

class Component
{
   public:
	virtual std::string ComponentToLine() = 0;
	Component(){ size = 0; inputSizeA = 0; inputSizeB = 0; isSigned = 0;};

        Component(int size) { this->size = size; }
        void ParsePorts(std::string line);
        std::string pad_zeros(std::string port);
        BitSize_e sizeToBitSize();
        int size;
        int isSigned;
        int inputSizeA;
        int inputSizeB;
        std::string output;
        std::string inputA;
        std::string inputB;
        Component_Type_e type;
        

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

#endif
