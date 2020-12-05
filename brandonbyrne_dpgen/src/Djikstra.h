#ifndef DJIKSTRA_H
#define DJIKSTRA_H
#include <tuple>
#include "IntfParser.h"

class Node;

enum Resource_E
{
   MUL_R,
   ADD_R,
   DIV_R,
   ALU_R,
   NUM_RESOURCES
};

class Djikstra 
{
   public:
	Djikstra(){};
	~Djikstra() {};
        void AddNode (std::string name, Component_Type_e type, BitSize_e size, std::vector<std::string> neighbors, std::string line, bool ifOp, std::string ifConditions, std::string realname, int ifCount);
        void AddNode (std::string name, Component_Type_e type, BitSize_e size, std::vector<std::string> neighbors, std::vector<std::string> parents, std::string line, bool ifOp, std::string ifConditions, std::string realname, int ifCount);
        Node* FindNode(std::string name);
        void VisitNode(std::string name);
        float GreatestLatency();
        void AddComponent(Component * component);
        std::vector<Node> nodes;
	void PrintNode(Node node);
	void ComputeAsapTime(std::vector<Node>* nodes);
	void ComputeAlapTime(std::vector<Node>* nodes, int latency);
	void ComputeFrames(std::vector<Node>* nodes);
	void ComputeDistance(std::vector<Node>* nodes, int latency);
	tuple<int, double> ComputeForce(std::vector<Node>* nodes, Node * node, int time, bool first, bool successor);
	vector<vector<double>> prob;
	void ResetVisited(std::vector<Node>*nodes);
	void PurgeNode(std::vector<Node>*nodes, std::string name);
        void Clean(); 
};

class Node
{
   public:
        Node(std::string name, Component_Type_e type, BitSize_e size) 
        { 
           this->name = name; 
           this->type = type; 
           this->size = size;
           this->weight = (float) -1 *  LATENCY[type][size]; 
           Reset(); 
        }
        void Reset() { distance = 0; visited = false; }
        void AddNeighbor(std::string neighbor) { neighborNames.push_back(neighbor); }
	void AddParent(std::string parent) { parentNames.push_back(parent); }
	std::vector<std::string> neighborNames;
	std::vector<std::string> parentNames;
        bool visited;
        float distance;
        float weight;
        string name;
        Component_Type_e type;
        BitSize_e size;
	int Time = 0;
	int asapTime = -1;
	int alapTime = -1;
	double prob;
	int width;
	std::string line;
	bool ifOp = false;
	std::string ifCondition = "";
	std::string netName = "";
	int ifCount = 0;
};

#endif
