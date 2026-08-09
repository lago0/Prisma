#include "NodeWorkspace.h"
#include "Node.h"
#include "core/Core.h"

NodeWorkspace::NodeWorkspace(Core* core) :
    core(core)
{
}

NodeWorkspace::~NodeWorkspace()
{
    for (Node* node : nodes)
    {
        delete node;
    }
}

bool NodeWorkspace::HasNode(Node* node)
{
    for (Node* currentNode : nodes)
    {
        if (currentNode == node)
        {
            return true;
        }
    }

    return false;
}

int NodeWorkspace::GetNodeIndex(Node* node)
{
    for (int i = 0; i < nodes.size(); i++)
    {
        if (nodes[i] == node)
        {
            return i;
        }
    }

    return -1;
}

void NodeWorkspace::AddNode(Node* node)
{
    if (!HasNode(node))
    {
        nodes.push_back(node);
    }
}

void NodeWorkspace::RemoveNode(Node* node)
{
    if (HasNode(node))
    {
        int index = GetNodeIndex(node);

        if (index >= 0) // There's a node
        {
            nodes.erase(nodes.begin() + index);
        }
    }
}