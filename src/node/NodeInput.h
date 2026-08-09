#pragma once

#include "enums/InOutType.h"

class Node;

class NodeInput
{
    public:
        NodeInput(Node* parent, InOutType type);

        virtual Node* GetNode();
        virtual Node* ConnectedNode();

        virtual InOutType Type();

        Node* parent;

        bool connected;
        Node* connectedNode;

        InOutType inType;
};