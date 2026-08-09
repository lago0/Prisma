#include "NodeInput.h"
#include "Node.h"

NodeInput::NodeInput(Node* parent, InOutType type) : 
    parent(parent),
    inType(type),
    connected(false),
    connectedNode(nullptr)
{
}

Node* NodeInput::GetNode()
{
    return parent;
}

Node* NodeInput::ConnectedNode()
{
    return connectedNode;
}

InOutType NodeInput::Type()
{
    return inType;
}