#ifndef DJIKSTRA_H
#define DJIKSTRA_H
#include "IntfParser.h"

class Node;

class Djikstra 
{
   public:
	Djikstra(){};
        void AddNode (std::string name, Component_Type_e type, BitSize_e size, std::vector<std::string> neighbors);
        Node* FindNode(std::string name);
        void VisitNode(std::string name);
        float GreatestLatency();
        void AddComponent(Component * component);
        std::vector<Node> nodes;
 
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
	std::vector<std::string> neighborNames;
        bool visited;
        float distance;
        float weight;
        string name;
        Component_Type_e type;
        BitSize_e size;
};

#endif
