#pragma once

#include <imgui.h>

#include "NodeViewer.h"

namespace UI
{
    struct RenderInfo
    {
        ImVec2 winPos;
        ImVec2 winSize;
        ImDrawList* drawList;
        NodeViewer* window;
    };
}