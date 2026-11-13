#include <imgui.h>
#include <iostream>
#include <string>
#include <algorithm>
#include <memory>

#include "Inspector.h"
#include "ui/UI.h"
#include "ui/window/NodeViewer/NodeView.h"
#include "node/NodeProperty.h"
#include "imagegallery/Image.h"
#include "imagegallery/ImageGallery.h"
#include "ui/dialogs/SelectImageDialog.h"

UI::Inspector::Inspector(UI* ui) :
    Window(ui)
{
}

void UI::Inspector::Update()
{
}

void UI::Inspector::Render()
{
    if (currentSelectImageDialog)
    {
        currentSelectImageDialog->Render();
    }

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
                    std::shared_ptr<IntNodeProperty> intProperty = std::dynamic_pointer_cast<IntNodeProperty>(property);
                    int temp = intProperty->GetValue();

                    auto bounds = GetIntBounds(property);

                    int min = bounds->min;
                    int max = bounds->max;
                    ImGui::Text(intProperty->GetName().c_str());
                    ImGui::SliderInt(("##" + intProperty->GetName()).c_str(), &temp, min, max);

                    if (temp != intProperty->GetValue())
                    {
                        intProperty->SetValue(temp);
                    }
                    break;
                }
                case PropertyType::IMAGE: {
                    std::string text = "";

                    auto imageProperty = std::dynamic_pointer_cast<ImageNodeProperty>(property);

                    if (imageProperty->GetValue() != nullptr)
                    {
                        text = imageProperty->GetValue()->name;
                    }
                    else 
                    {
                        text = "Please select an image";
                    }

                    if (ImGui::Button(text.c_str()) && imageDialogOpened == false) {
                        currentSelectImageDialog = new SelectImageDialog(ui, ui->GetCore());
                        currentSelectImageDialog->Open();
                        selectImageDialogPropertyId = property->GetName();
                        imageDialogOpened = true;
                    };

                    if (imageDialogOpened && currentSelectImageDialog->IsOpen() == false && selectImageDialogPropertyId == property->GetName())
                    {
                        if (currentSelectImageDialog->cancelled == false && currentSelectImageDialog->imageIdSelected != -1)
                        {
                            std::shared_ptr<Image> image = ui->GetCore()->imageGallery->GetImage(currentSelectImageDialog->imageIdSelected);

                            imageProperty->SetValue(image);
                            imageProperty->OnValueChanged();
                        }

                        imageDialogOpened = false;
                        delete currentSelectImageDialog;
                    }
                    break;
                }
                default:
                    break;
            }
        }
    }

    ImGui::End();
}
