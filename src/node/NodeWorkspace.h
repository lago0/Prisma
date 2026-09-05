#pragma once

#include <util/Buffer.h>
#include <util/signal/Signal.h>

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
    
        Node* GetOutputNode();

        Buffer4* GetViewportOutput();

        void SetDirty();
        
        bool isDirty = true;
        Buffer4* cachedBuffer = nullptr;

        std::vector<Node*> nodes;
        Node* imageOut;

        Core* core;

        Util::Signal<NodeWorkspace*, Node*>* onNodeCreated;
        Util::Signal<NodeWorkspace*, Node*>* onNodeDeleted;
};