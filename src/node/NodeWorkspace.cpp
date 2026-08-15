#include "NodeWorkspace.h"
#include "Node.h"
#include "core/Core.h"
#include "nodes/ImageOut.h"
#include "nodes/ImageIn.h"
#include "compositor/Compositor.h"

NodeWorkspace::NodeWorkspace(Core* core) :
    core(core)
{
    this->imageOut = new ImageOut();
    this->AddNode(this->imageOut);

    ImageIn* imageIn = new ImageIn();
    this->AddNode(imageIn);
    
    imageIn->ConnectOutput(imageOut->nodeInputs.at(0));
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
        node->NodeConnectedToWorkspace(this);
    }
}

void NodeWorkspace::RemoveNode(Node* node)
{
    if (HasNode(node) && node != imageOut)
    {
        int index = GetNodeIndex(node);

        if (index >= 0) // There's a node
        {
            nodes.erase(nodes.begin() + index);
        }
        node->NodeDisconnectedFromWorkspace();
    }
}

Node* NodeWorkspace::GetOutputNode()
{
    return imageOut;
}

Buffer4* NodeWorkspace::GetViewportOutput()
{
    if (isDirty || cachedBuffer == nullptr)
    {
        cachedBuffer = static_cast<Buffer4*>(GetOutputNode()->GetViewportOutput());
    }

    return cachedBuffer;
}

void NodeWorkspace::SetDirty()
{
    isDirty = true;
    core->GetCompositor()->SetDirty();
}