#pragma once

#include <unordered_map>
#include <vector>
#include <string>

#include "enums/InOutType.h"
#include "util/signal/Signal.h"
#include "enums/NodeUpdateType.h"
#include "util/Buffer.h"
#include "NodeWorkspace.h"

class NodeInput;
class Core;

class Node
{
    public:
        Node(InOutType type);
        ~Node();

        virtual void ConnectOutput(NodeInput* nodeInput);
        virtual void ConnectInput(NodeInput* nodeInput, Node* node);

        virtual void DisconnectOutput(NodeInput* nodeInput);
        virtual void DisconnectInput(NodeInput* nodeInput);

        virtual void* GetOutput();
        virtual void* GetViewportOutput();

        virtual void SetDirty();

        virtual void NodeConnectedToWorkspace(NodeWorkspace* nodeWorkspace);
        virtual void NodeDisconnectedFromWorkspace();

        Buffer4* cachedBuffer = nullptr;
        bool isDirty = true;

        std::unordered_map<int, NodeInput*> nodeInputs;
        std::vector<NodeInput*> nodeOutputs;
        bool outputEnabled = true;

        InOutType outType;
        Util::Signal<NodeUpdateType> onNodeUpdated;

        NodeWorkspace* nodeWorkspace;

        virtual std::string visualName() const
        {
            return "Node";
        }
};