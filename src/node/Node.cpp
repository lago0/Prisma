#include "Node.h"
#include "NodeInput.h"
#include "core/Core.h"

Node::Node(InOutType type) : 
    outType(type),
    isDirty(true)
{
}

Node::~Node()
{
    if (outputEnabled)
    {
        for (NodeInput* output : nodeOutputs)
        {
            DisconnectOutput(output);
        }
    }

    for (const auto& [key, input] : nodeInputs)
    {
        if (input->connected)
        {
            DisconnectInput(input);
        }
        delete input;
    }

    if (cachedBuffer)
    {
        delete cachedBuffer;
    }
}

void Node::ConnectOutput(NodeInput* nodeInput)
{
    if (!outputEnabled) return;

    if (nodeInput->Type() == this->outType)
    {
        Node* node = nodeInput->GetNode();

        nodeOutputs.push_back(nodeInput);
    
        nodeInput->connected = true;
        nodeInput->connectedNode = this;

        this->onNodeUpdated.Emit(NodeUpdateType::OUTPUT_CONNECTED);
        node->onNodeUpdated.Emit(NodeUpdateType::INPUT_CONNECTED);

        SetDirty();
    }
}

void Node::ConnectInput(NodeInput* nodeInput, Node* node)
{
    node->ConnectOutput(nodeInput);
}

void Node::DisconnectOutput(NodeInput* nodeInput)
{
    if (!outputEnabled) return;

    for (int i = 0; i < nodeOutputs.size(); i++)
    {
        if (nodeOutputs[i] == nodeInput)
        {
            NodeInput* input = nodeOutputs[i];
            nodeOutputs.erase(nodeOutputs.begin() + i);

            nodeInput->connected = false;
            nodeInput->connectedNode = nullptr;

            this->onNodeUpdated.Emit(NodeUpdateType::OUTPUT_DISCONNECTED);
            nodeInput->GetNode()->onNodeUpdated.Emit(NodeUpdateType::INPUT_DISCONNECTED);

            input->GetNode()->SetDirty();
            SetDirty();
        }
    }
}

void Node::DisconnectInput(NodeInput* nodeInput)
{
    if (nodeInput->ConnectedNode() != nullptr)
    {
        nodeInput->ConnectedNode()->DisconnectOutput(nodeInput);
        SetDirty();
    }
}

void* Node::GetOutput()
{
    return nullptr;
}

void* Node::GetViewportOutput()
{
    return nullptr;
}

void Node::SetDirty()
{
    isDirty = true;

    for (NodeInput* nodeInput : nodeOutputs)
    {
        nodeInput->GetNode()->SetDirty();
    }
}

void Node::NodeConnectedToWorkspace(NodeWorkspace* workspace)
{
    nodeWorkspace = workspace;
}

void Node::NodeDisconnectedFromWorkspace()
{
    nodeWorkspace = nullptr;
}