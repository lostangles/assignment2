#include "Djikstra.h"

void Djikstra::PrintNode(Node node)
{
   std::cout << "Name: " << node.name << std::endl
	   << "Type: " << node.type << std::endl
	   << "Size: " << node.size << std::endl
	   << "Neighbor names: " ;
   for (int i = 0; i < (int)node.neighborNames.size(); i++)
   {
      std::cout << node.neighborNames[i] << " ";
   }
   std::cout << endl;
   std::cout << "Parent names: ";

   for (int i = 0; i < (int)node.parentNames.size(); i++)
   {
      std::cout << node.parentNames[i] << " ";
   }
   std::cout << endl;
   std::cout << "Node width: " << node.width << std::endl;
   std::cout << "Node prob: " << node.prob << std::endl;
   std::cout << "Scheduled time: " << node.Time << std::endl;
   std::cout << "ASAP time: " << node.asapTime << std::endl;
   std::cout << "ALAP time: " << node.alapTime << std::endl << std::endl;
}

void Djikstra::PurgeNode(std::vector<Node>* nodes, std::string name)
{
   std::cout << "Purge Node name: " << name << std::endl;	
   for (int i = 0; i < (int) (*nodes).size(); i++)
   {
      Node * curNode = &(*nodes)[i];
      for (int j = 0; j < (int)curNode->neighborNames.size(); j++)
      {
         if (curNode->neighborNames[j].compare(name) == 0)
		 curNode->neighborNames.erase(curNode->neighborNames.begin() + j);
      }
      for (int j = 0; j < (int)curNode->parentNames.size(); j++)
      {
         if (curNode->parentNames[j].compare(name) == 0)
		 curNode->parentNames.erase(curNode->parentNames.begin() + j);
      }
   }
   for (int i = 0; i < (int) (*nodes).size(); i++)
   {
      if ( (*nodes)[i].name.compare(name) == 0)
	      (*nodes).erase( (*nodes).begin() + i);
   }
}

void Djikstra::AddNode(std::string name,  Component_Type_e type, BitSize_e size, std::vector<std::string> neighbors, std::vector<std::string> parents)
{
   Node newNode(name, type, size);
   newNode.distance = std::numeric_limits<float>::infinity();
   for (int i = 0; i < (int)neighbors.size(); i++)
   {
      newNode.AddNeighbor(neighbors[i]);
   }
   for (int i = 0; i < (int)parents.size(); i++)
   {
      newNode.AddParent(parents[i]);
   }
   nodes.push_back(newNode); 
}

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
   for (int j = 0; j < (int)nodes.size(); j++)
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
      for (int i = 0; i < (int)nodes.size(); i++) nodes[i].Reset();
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
           std::vector<std::string> neighborsA, neighborsB, neighborsC, parents;
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
	      parents.push_back(component->inputA);
	      parents.push_back(component->inputB);
              AddNode(component->output, component->type, component->sizeToBitSize(), neighborsC, parents);
           }

           break;
   }
   case COMP:
   { 
           std::vector<std::string> neighborsA, neighborsB, neighborsC, parents;
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
	      parents.push_back(compComp->inputA);
	      parents.push_back(compComp->inputB);
              if (compComp->equal.compare("") != 0)
              AddNode(compComp->equal, component->type, component->sizeToBitSize(), neighborsC, parents);
              if (compComp->lessThan.compare("") != 0)
              AddNode(compComp->lessThan, component->type, component->sizeToBitSize(), neighborsC, parents);
              if (compComp->greaterThan.compare("") != 0)
              AddNode(compComp->greaterThan, component->type, component->sizeToBitSize(), neighborsC, parents);
           }
      
           break;
   }
   case MUX2x1:
   {
           std::vector<std::string> neighborsA, neighborsB, neighborsC, parents;
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
	      parents.push_back(component->inputA);
	      parents.push_back(component->inputB);
	      parents.push_back(compMux->switchInput);
              AddNode(component->output, component->type, component->sizeToBitSize(), neighborsC, parents);
           }

      
      break;
   }
   case INC:
   case DEC:
   case FF:
   {
           std::vector<std::string> neighborsA, neighborsC, parents;
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
	      parents.push_back(component->inputA);
	      parents.push_back(component->inputB);
              AddNode(component->output, component->type, component->sizeToBitSize(), neighborsC, parents);
           }

      break;
   }
   case OUTPUT:
   { 
           std::vector<std::string> neighborsA, neighborsB;
           ComponentOUTPUT* compOut = reinterpret_cast<ComponentOUTPUT*>(component);
           for (int i = 0; i < (int)compOut->ports.size(); i++)
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
           for (int i = 0; i < (int)compOut->ports.size(); i++)
           {
              AddNode(compOut->ports[i], component->type, component->sizeToBitSize(), neighborsA);
           }
      break;
   }
   
   

   default: break; 
   }

}

void Djikstra::ComputeAsapTime(std::vector<Node>* nodes)
{
   bool done = false;
   int maxTime,tempTime;
   while (!done)
   {
      done = true;

      for (int i = 0; i < (int)nodes->size(); i++)
      {
         maxTime = 0;
         tempTime = 0;
	 
         Node * curNode = FindNode((*nodes)[i].name);
	 //Nodes with no parents are inputs, not counting as cycles, they get 0 asap time
	 if (curNode->parentNames.size() == 0)
	 {
            curNode->asapTime = 1;
	 }
	 else //has a parent, calculate asap time as parent + cyles for that operation
	 {
            


	    for (int j = 0; j < (int)curNode->parentNames.size(); j++) //go through each operations parents
	    {
               Node * parNode = FindNode(curNode->parentNames[j]);
	       if (parNode->type == MUL)
	       {
                  tempTime = parNode->asapTime + 1;
	       }
	       else if ( (parNode->type == DIV) || (parNode->type == MOD) )
	       {
                  tempTime = parNode->asapTime + 2;
	       }
	       else
	       {
                  tempTime = parNode->asapTime;
	       }
	       if (tempTime > maxTime)
	       {
                  maxTime = tempTime;
	       }
	    }
	    curNode->asapTime = maxTime + 1;
	 }

      }
   }
}

void Djikstra::ComputeAlapTime(std::vector<Node>* nodes, int latency)
{
   bool done = false;
   int maxTime,tempTime;
   while (!done)
   {
      done = true;

      for (int i = 0; i < (int)nodes->size(); i++)
      {
         maxTime = 10000;
         tempTime = 0;
	 
         Node * curNode = FindNode((*nodes)[i].name);
	 if (curNode->alapTime == -1)
	 {
            done = false;
	 }
	 //Nodes with no children are the last operations, their alap time is latency minus whatever their time to complete is
	 if (curNode->neighborNames.size() == 0)
	 {
            if (curNode->type == MUL)
	    {
               curNode->alapTime = latency - 2;
	    }
	    else if ( (curNode->type == DIV) || (curNode->type == MOD) )
	    {
               curNode->alapTime = latency - 3;
	    }
	    else if ( (curNode->type == OUTPUT) )
	    {
               curNode->alapTime = latency;
	    }
	    else
	    {
               curNode->alapTime = latency;
	    }

	 }
	 else //has neighbors, calculate alap time as neighbor - cyles for that operation
	 {
            


	    for (int j = 0; j < (int)curNode->neighborNames.size(); j++) //go through each operations parents
	    {
               Node * neighborNode = FindNode(curNode->neighborNames[j]);
	       if (neighborNode->alapTime == -1 ) continue;
	       if (curNode->type == MUL)
	       {
                  tempTime = neighborNode->alapTime - 1;
	       }
	       else if ( (curNode->type == DIV) || (curNode->type == MOD) )
	       {
                  tempTime = neighborNode->alapTime - 2;
	       }
	       else
	       {
                  tempTime = neighborNode->alapTime;
	       }
	       if (tempTime < maxTime)
	       {
                  maxTime = tempTime;
	       }
	    }
	    if (maxTime != 10000)
	    {
	       curNode->alapTime = maxTime - 1;
	    }
	 }

      }
   }
}

void Djikstra::ComputeFrames(std::vector<Node> * nodes)
{
   for (int i = 0; i < (int) (*nodes).size(); i++)
   {
      Node * node = &(*nodes)[i];
      node->width = (node->alapTime - node->asapTime) + 1;
      node->prob = 1.0 / node->width;
   }
}

void Djikstra::ComputeDistance(std::vector<Node>* nodes, int latency)
{
   //Init prob vector
   prob.resize(NUM_RESOURCES);
   for (int i = 0; i < NUM_RESOURCES; i++)
   {
      prob[i].resize(latency + 1);
      for (int j = 0;  j < latency + 1; j++)
      {
         prob[i][j] = 0.0;
      }
   }


   for (int i = 0; i < (int)(*nodes).size(); i++)
   {
      Node * curNode = &(*nodes)[i];
      for (int j = curNode->asapTime; j <= curNode->alapTime; j++)
      {
         if (curNode->type == MUL)
	 {
            prob[MUL_R][j] += curNode->prob;
	 }
	 else if ( (curNode->type == ADD) || (curNode->type == SUB) )
	 {
            prob[ADD_R][j] += curNode->prob;
	 }
	 else if ( (curNode->type == DIV) || (curNode->type == MOD) )
	 {
            prob[DIV_R][j] += curNode->prob;
	 }
	 else
	 {
            prob[ALU_R][j] += curNode->prob;
	 }
      }
   }
}

void Djikstra::ResetVisited(std::vector<Node> *nodes)
{
   for (int i = 0; i < (int)(*nodes).size(); i++)
   {
      (*nodes)[i].visited = false;
   }
}

tuple<int, double> Djikstra::ComputeForce(std::vector<Node>* nodes, Node* node, int time, bool first, bool successor)
{
   tuple<int, double> force(-1, 0.0);
   tuple<int, double> forceSum(-1, 10000);
   double probability;
   int resource;
   if (node->type == MUL) resource = MUL_R;
   else if ( (node->type == ADD) || (node->type == SUB) ) resource = ADD_R;
   else if ( (node->type == DIV) || (node->type == MOD) ) resource = DIV_R;
   else resource = ALU_R;

   node->visited = true;

   if ( ((time < node->asapTime) || (node->alapTime < time)) && !first)
   {
      tuple<int, double> temp(0, 0.0);
      return temp;
   }

   for (int time = node->asapTime; time <= node->alapTime; time++)
   {
      if (first)
      {
         ResetVisited(nodes);
      }
      for (int i = 0; i < (int)prob[resource].size(); i++)
      {
         if (!first)
	 {
            if (successor && (i <= time))
	    {
               probability = 0.0;
	    }
	    else if (!successor && (i >= time))
	    {
               probability = 0.0;
	    }
	    else if (time == i)
	    {
               probability = 1.0;
	    }
	    else 
	    {
               probability = 0.0;
	    }
	 }
	 else if (time == i)
	 {
            probability = 1.0;
	 }
	 else
	 {
            probability = 0.0;
	 }
         get<1>(force) += prob[resource][i] * (probability - node->prob);
      }
      
      for (int i = 0; i < (int)node->parentNames.size(); i++)
      {
         Node * parNode = FindNode(node->parentNames[i]);
	 if (!parNode->visited)
	 {
            get<1>(force) += get<1>(ComputeForce(nodes, parNode, time, false, false));
	 }
      }
      for (int i = 0; i < (int)node->neighborNames.size(); i++)
      {
         Node * neighborNode = FindNode(node->neighborNames[i]);
	 if (!neighborNode->visited)
	 {
            get<1>(force) += get<1>(ComputeForce(nodes, neighborNode, time, false, true));
	 }
      }
      if (get<1>(force) < get<1>(forceSum))
      {
         get<1>(forceSum) = get<1>(force);
	 if (first)
	 {
            get<0>(forceSum) = time;
	 }
      }

   }
   return forceSum;
}



