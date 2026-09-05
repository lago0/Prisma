#include <imgui.h>
#include <iostream>
#include <string>
#include <algorithm>

#include "LayerWindow.h"
#include "ui/UI.h"
#include "LayerView.h"
#include "layer/Layer.h"
#include "layer/LayerManager.h"
#include "core/Core.h"

UI::LayerWindow::LayerWindow(UI* ui) :
    Window(ui)
{
    Core* core = ui->GetCore();
    layerManager = core->layerManager;

    layerManager->onLayerCreated->Connect([this](Layer* layer)
    {
        this->OnLayerCreatedCallback(layer);
    });

    layerManager->onLayerUpdated->Connect([this](Layer* layer, LayerUpdateType layerUpdateType)
    {
        std::cout << "Layer Updated: " << layer->GetIndex() << " " << layer->GetName() << " " << (int)layerUpdateType << std::endl;
        this->OnLayerUpdateCallback(layer, layerUpdateType);
    });

    layerManager->onLayerDeleted->Connect([this](Layer* layer)
    {
        std::cout << "Layer Deleted: " << layer->GetName() << std::endl;
        this->OnLayerDeletedCallback(layer);
    });

    std::cout << "Layers count: " << layerManager->layers.size() << std::endl;

    for (int i = 0; i < layerManager->layers.size(); i++)
    {
        std::cout << "Layer index loop: " << i << std::endl;
        OnLayerCreatedCallback(layerManager->layers.at(i));
    }

    layerManager->onLayerCreated->Connect([this](Layer* layer)
    {
        this->ui->SetSelectedLayer(layer);
    });
}

void UI::LayerWindow::Update()
{
}

void UI::LayerWindow::Render()
{
    int deleteIndex = -1;
    static int selectedIndex = -1;

    ImGui::Begin("Layer Window");

    if (ImGui::Button("Add Layer", ImVec2(ImGui::GetWindowSize()[0] - 10, 20)))
    {
        layerManager->CreateLayer();
    }

    ImGui::Separator();

    for (LayerView* layerView : layerViews)
    {
        if (layerView->layer == nullptr) continue;

        std::string text = std::to_string(layerView->layer->GetIndex()) + " " + layerView->layer->GetName();

        bool wasSelected = ui->selectedLayer == layerView->layer;
        
        if (wasSelected)
        {
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.45f, 0.9f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.1f, 0.55f, 1.0f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 0.35f, 0.75f, 1.0f));
        }

        if (ImGui::Button(text.c_str(), ImVec2(ImGui::GetWindowSize()[0] - 10, 50)))
        {
            ui->SetSelectedLayer(layerView->layer);
        }

        if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
        {
            selectedIndex = layerView->layer->GetIndex();
            //deleteIndex = layerView->layer->GetIndex();
        }

        if (wasSelected)
        {
            ImGui::PopStyleColor(3);
        }
    }

    if (selectedIndex > -1)
    {
        ImGui::OpenPopup("LayerPopup");
    }

    if (ImGui::BeginPopup("LayerPopup"))
    {
        if (ImGui::Selectable("Delete Layer"))
        {
            deleteIndex = selectedIndex;
            selectedIndex = -1;
        }
        
        if (ImGui::Selectable("Close"))
        {
            selectedIndex = -1;
        }

        ImGui::EndPopup();
    }

    if (deleteIndex > -1)
    {
        layerManager->DeleteLayer(deleteIndex);
    }
    
    ImGui::End();
}

void UI::LayerWindow::OnLayerCreatedCallback(Layer* layer)
{
    std::cout << "Layer Created: " << layer->GetIndex() << " " << layer->GetName() << std::endl;
    int index = layerManager->GetIndexByLayer(layer);

    if (index >= 0)
    {
        LayerView* layerView = new LayerView();
        layerView->layer = layer;
        
        if (layerViews.size() <= index)
        {
            layerViews.push_back(layerView);      
        }
        else
        {
            // TODO: Handle sync issues
            delete layerView;
        }
    }
}

void UI::LayerWindow::OnLayerUpdateCallback(Layer* layer, LayerUpdateType layerUpdateType)
{
    // Handle updates
}

void UI::LayerWindow::OnLayerDeletedCallback(Layer* layer)
{
    for (int i = 0; i < layerViews.size(); i++)
    {
        if (layerViews[i]->layer == layer)
        {
            layerViews.erase(layerViews.begin() + i);
        }
    }
}