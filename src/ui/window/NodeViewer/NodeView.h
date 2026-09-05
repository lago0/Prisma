#pragma once

#include "node/Node.h"

class NodeView
{
    public:
        float x, y;
        Node* originalNode;

        ImVec2 realStartPos;
        ImVec2 realEndPos;
};