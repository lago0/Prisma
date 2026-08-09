#pragma once

#include <vector>

class Node;
class Core;

class NodeWorkspace
{
    public:
        NodeWorkspace(Core* core);
        ~NodeWorkspace();

        bool HasNode(Node* node);
        int GetNodeIndex(Node* node);

        void AddNode(Node* node);
        void RemoveNode(Node* node);
        
        Core* core;

        std::vector<Node*> nodes;
};