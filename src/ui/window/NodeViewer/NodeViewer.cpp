#include <SDL3/SDL.h>
#include <imgui.h>
#include <cmath>
#include <math.h>

#include <iostream>

#include "NodeViewer.h"
#include "NodeView.h"
#include "layer/LayerManager.h"
#include "layer/Layer.h"
#include "node/Node.h"
#include "node/NodeWorkspace.h"
#include "ui/UI.h"
#include "node/nodes/ImageOut.h"
#include "NodeMap.h"
#include "RenderInfo.h"
#include "NodeViewerUtils.h"

#include "NodeViewerRenderer.h"
#include "NodeViewerControls.h"
#include "NodeViewerGraph.h"

/**
 * Please do not refactor
 * I already suffered much with this
 * 
 * These up there are util functions
 * Util functions usually have more arguments than they need so in the future if you need them, you don't have to break your head and change all the code
 */


UI::NodeViewer::NodeViewer(UI* ui) :
    Window(ui),
    nodeMaps(mapNodes()),
    renderer(std::make_unique<NodeViewerRenderer>(this)),
    controls(std::make_unique<NodeViewerControls>(this)),
    graph(std::make_unique<NodeViewerGraph>(this))
{
    ui->onEvent->Connect([&](SDL_Event* event)
    {
        this->OnEvent(event);
    });
}

UI::NodeViewer::~NodeViewer()
{
}

void UI::NodeViewer::Update()
{
    if (IsDragging(graph->renderInfo))
    {
        ImVec2 mousePos = ImGui::GetMousePos();

        if (dragging)
        {
            currentViewPosX = (draggingStartPosX - mousePos.x) / zoom + draggingPosOffsetX;
            currentViewPosY = (draggingStartPosY - mousePos.y) / zoom + draggingPosOffsetY;
        }
        else if (draggingNode)
        {
            ui->selectedNodeView->x = (mousePos.x - draggingStartPosX) / zoom + draggingPosOffsetX;
            ui->selectedNodeView->y = (mousePos.y - draggingStartPosY) / zoom + draggingPosOffsetY;
        }

        ImGui::SetMouseCursor(ImGuiMouseCursor_None);
    }
}

void UI::NodeViewer::Render()
{
    renderer->Render();
}

void UI::NodeViewer::OnEvent(SDL_Event* event)
{
    if (!renderer->isWindowFocused)
    {
        return;
    }

    controls->OnEvent(event);
}

void UI::NodeViewer::SetSelectedNodeView(std::shared_ptr<NodeView> view)
{
    ui->selectedNodeView = view;
}

std::shared_ptr<UI::NodeView> UI::NodeViewer::GetSelectedNodeView()
{
    return ui->selectedNodeView;
}