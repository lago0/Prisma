#include <imgui.h>
#include <iostream>
#include <string>
#include <algorithm>
#include <memory>

#include "Inspector.h"
#include "ui/UI.h"
#include "ui/window/NodeViewer/NodeView.h"
#include "node/NodeProperty.h"

UI::Inspector::Inspector(UI* ui) :
    Window(ui)
{
}

void UI::Inspector::Update()
{
}

void UI::Inspector::Render()
{
    ImGui::Begin("Inspector");

    Layer* selectedLayer = ui->selectedLayer;
    std::shared_ptr<NodeView> selectedNodeView = ui->selectedNodeView;

    if (selectedLayer && selectedNodeView && selectedNodeView->originalNode)
    {
        for (std::shared_ptr<NodeProperty> property : selectedNodeView->originalNode->properties)
        {
            if (property == nullptr)
            {
                continue;
            }

            switch (property->type)
            {
                // TODO: Add support for PropertyType::FLOAT and PropertyType::STRING
                case PropertyType::INT: {
                    int temp = *(int*)property->GetValue();

                    auto bounds = GetIntBounds(property);

                    int min = bounds->min;
                    int max = bounds->max;
                    ImGui::SliderInt(property->GetName().c_str(), &temp, 0, 200);

                    if (temp != *(int*)property->GetValue())
                    {
                        *(int*)property->value = temp;
                        property->OnValueChanged();
                    }
                }
            }
        }
    }

    ImGui::End();
}
