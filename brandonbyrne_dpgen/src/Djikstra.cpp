#include "Djikstra.h"

void Djikstra::AddNode(std::string name,  Component_Type_e type, BitSize_e size, std::vector<std::string> neighbors)
{
   Node newNode(name, type, size);
   newNode.distance = std::numeric_limits<float>::infinity();
   for (int i = 0; i < (int)neighbors.size(); i++)
   {
      newNode.AddNeighbor(neighbors[i]);
   }
   nodes.push_back(newNode); 
}

Node * Djikstra::FindNode(std::string name)
{
   Node* node = NULL;
   for (int i = 0; i < (int)nodes.size(); i++)
   {
      if (nodes[i].name.compare(name) == 0)
      {
         node = &nodes[i];
      }
   }
   return node;
}

void Djikstra::VisitNode(std::string name)
{
   Node* nodeId = FindNode(name);

   if (nodeId->neighborNames.size() == 0)
   {
     nodeId->visited = true;
   }   
   //For the current node
   for (int j = 0; j < (int)nodeId->neighborNames.size(); j++)
   {
      Node* neighbor = FindNode(nodeId->neighborNames[j]);
      if (neighbor == NULL) 
      {
         continue;
      }
      if (neighbor->visited == false)
      {
         float currDistance = nodeId->distance + neighbor->weight;
         if (neighbor->distance > currDistance)
         {
            neighbor->distance = currDistance;
         }
      }
   }
   nodeId->visited = true;
  

   Node* nextNode = NULL;
for (int k = 0; k < (int)nodeId->neighborNames.size(); k++)
{   
   for (int i = 0; i < (int)nodeId->neighborNames.size(); i++)
   {
      Node* neighbor = FindNode(nodeId->neighborNames[i]);
      float smallestDistance = std::numeric_limits<float>::infinity();
      if (neighbor == NULL)
      {
         continue;
      }
      if (neighbor->visited == false)
      {           
         if (neighbor->distance < smallestDistance)
         {
            smallestDistance = neighbor->distance;
            nextNode = neighbor;
         }
      }
   }
   if (nextNode != NULL)
   { 
      VisitNode(nextNode->name);    
   }
}
}

float Djikstra::GreatestLatency()
{
   float greatestLatency = std::numeric_limits<float>::infinity();
   for (int j = 0; j < nodes.size(); j++)
   {
      if (nodes[j].type == INPUT)
      {
      VisitNode(nodes[j].name);
      for (int i = 0; i < (int)nodes.size(); i++)
      {
         if ( (nodes[i].distance < greatestLatency) && (nodes[i].distance != std::numeric_limits<float>::infinity()) ) 
         {
            greatestLatency = nodes[i].distance;
         }
      }
      for (int i = 0; i < nodes.size(); i++) nodes[i].Reset();
      }
   }
   return -1.0 * greatestLatency;
}

void Djikstra::AddComponent(Component * component)
{
   switch ( component->type )
   {
   case REG:
   case ADD:
   case SUB:
   case MUL:
   case SHR:
   case SHL:
   case DIV:
   case MOD:
   { 
           std::vector<std::string> neighborsA, neighborsB, neighborsC;
           Node * node = FindNode(component->inputA);
           Node * neighbor = FindNode(component->output);
           if( node == NULL)
           {
              neighborsA.push_back(component->output);
              AddNode(component->inputA, component->type, component->sizeToBitSize(), neighborsA);
           }
           else
           {
              node->neighborNames.push_back(component->output);
           }
           node = FindNode(component->inputB);
           if( node == NULL)
           {
              neighborsB.push_back(component->output);
              AddNode(component->inputB, component->type, component->sizeToBitSize(), neighborsB);
           }
           else
           {
              node->neighborNames.push_back(component->output);
           }
           if (neighbor == NULL)
           {
              AddNode(component->output, component->type, component->sizeToBitSize(), neighborsC);
           }

           break;
   }
   case COMP:
   { 
           std::vector<std::string> neighborsA, neighborsB, neighborsC;
           Node * node = FindNode(component->inputA);
           Node * neighbor = FindNode(component->output);
           ComponentCOMP * compComp = reinterpret_cast<ComponentCOMP*>(component);
           if( node == NULL)
           {
              if (compComp->equal.compare("") != 0)
              neighborsA.push_back(compComp->equal);
              if (compComp->lessThan.compare("") != 0)
              neighborsA.push_back(compComp->lessThan);
              if (compComp->greaterThan.compare("") != 0)
              neighborsA.push_back(compComp->greaterThan);
             
              AddNode(compComp->inputA, compComp->type, compComp->sizeToBitSize(), neighborsA);
           }
           else
           {
              if (compComp->equal.compare("") != 0)
              node->neighborNames.push_back(compComp->equal);
              if (compComp->lessThan.compare("") != 0)
              node->neighborNames.push_back(compComp->lessThan);
              if (compComp->greaterThan.compare("") != 0)
              node->neighborNames.push_back(compComp->greaterThan);
           }
           node = FindNode(component->inputB);
           if( node == NULL)
           {
              if (compComp->equal.compare("") != 0)
              neighborsB.push_back(compComp->equal);
              if (compComp->lessThan.compare("") != 0)
              neighborsB.push_back(compComp->lessThan);
              if (compComp->greaterThan.compare("") != 0)
              neighborsB.push_back(compComp->greaterThan);
             
              AddNode(component->inputB, component->type, component->sizeToBitSize(), neighborsB);
           }
           else
           {
              if (compComp->equal.compare("") != 0)
              node->neighborNames.push_back(compComp->equal);
              if (compComp->lessThan.compare("") != 0)
              node->neighborNames.push_back(compComp->lessThan);
              if (compComp->greaterThan.compare("") != 0)
              node->neighborNames.push_back(compComp->greaterThan);
           }
           if (neighbor == NULL)
           {
              if (compComp->equal.compare("") != 0)
              AddNode(compComp->equal, component->type, component->sizeToBitSize(), neighborsC);
              if (compComp->lessThan.compare("") != 0)
              AddNode(compComp->lessThan, component->type, component->sizeToBitSize(), neighborsC);
              if (compComp->greaterThan.compare("") != 0)
              AddNode(compComp->greaterThan, component->type, component->sizeToBitSize(), neighborsC);
           }
      
           break;
   }
   case MUX2x1:
   {
           std::vector<std::string> neighborsA, neighborsB, neighborsC;
           Node * node = FindNode(component->inputA);
           Node * neighbor = FindNode(component->output);
           ComponentMUX2x1 * compMux = reinterpret_cast<ComponentMUX2x1*>(component);
           if( node == NULL)
           {
              neighborsA.push_back(compMux->output);
              AddNode(component->inputA, component->type, component->sizeToBitSize(), neighborsA);
           }
           else
           {
              node->neighborNames.push_back(component->output);
           }
           node = FindNode(component->inputB);
           if( node == NULL)
           {
              neighborsB.push_back(component->output);
              AddNode(component->inputB, component->type, component->sizeToBitSize(), neighborsB);
           }
           else
           {
              node->neighborNames.push_back(component->output);
           }
           node = FindNode(compMux->switchInput);
           if( node == NULL)
           {
              neighborsC.push_back(component->output);
              AddNode(compMux->switchInput, component->type, component->sizeToBitSize(), neighborsC);
           }
           else
           {
              node->neighborNames.push_back(compMux->output);
           }
           if (neighbor == NULL)
           {
              AddNode(component->output, component->type, component->sizeToBitSize(), neighborsC);
           }

      
      break;
   }
   case INC:
   case DEC:
   case FF:
   {
           std::vector<std::string> neighborsA, neighborsC;
           Node * node = FindNode(component->inputA);
           Node * neighbor = FindNode(component->output);
           if( node == NULL)
           {
              neighborsA.push_back(component->output);
              AddNode(component->inputA, component->type, component->sizeToBitSize(), neighborsA);
           }
           else
           {
              node->neighborNames.push_back(component->output);
           }
           if (neighbor == NULL)
           {
              AddNode(component->output, component->type, component->sizeToBitSize(), neighborsC);
           }

      break;
   }
   case OUTPUT:
   { 
           std::vector<std::string> neighborsA, neighborsB;
           ComponentOUTPUT* compOut = reinterpret_cast<ComponentOUTPUT*>(component);
           for (int i = 0; i < compOut->ports.size(); i++)
           {
              Node * node = FindNode(compOut->ports[i]);
	      if (node == NULL)
              {
	         AddNode(compOut->ports[i], component->type, component->sizeToBitSize(), neighborsA);
              }
           }
      break;
   }
   case INPUT:
   { 
           std::vector<std::string> neighborsA, neighborsB;
           ComponentINPUT* compOut = reinterpret_cast<ComponentINPUT*>(component);
           for (int i = 0; i < compOut->ports.size(); i++)
           {
              AddNode(compOut->ports[i], component->type, component->sizeToBitSize(), neighborsA);
           }
      break;
   }
   
   

   default: break; 
   }

}
