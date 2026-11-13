#include <imgui.h>
#include <iostream>
#include <string>
#include <algorithm>

#include "ImageGalleryWindow.h"
#include "ui/UI.h"
#include "core/Core.h"
#include "imagegallery/ImageGallery.h"
#include "imagegallery/Image.h"

UI::ImageGalleryWindow::ImageGalleryWindow(UI* ui) :
    Window(ui),
    core(ui->GetCore())
{
}

void UI::ImageGalleryWindow::Update()
{
}

void UI::ImageGalleryWindow::Render()
{
    ImGui::Begin("Image Gallery");
    
    ImageGallery* gallery = core->imageGallery;

    if (ImGui::Button("Import", ImVec2(ImGui::GetWindowSize()[0] - 10,60)))
    {
        ImGui::OpenPopup("ImageGalleryFileDialog");
    }

    if (gallery)
    {
        for (auto data : gallery->GetImages())
        {
            ImGui::Button((data.second->name + "##" + std::to_string(data.second->id)).c_str(), ImVec2(ImGui::GetWindowSize()[0] - 10,60));
        }
    }

    if (ImGui::BeginPopup("ImageGalleryFileDialog"))
    {
        static char text[255];
        
        ImGui::PushFont(ui->titleFont);

        ImVec2 textSize = ImGui::CalcTextSize("IMPORT IMAGE");
        ImVec2 availableSize = ImGui::GetContentRegionAvail();

        ImGui::SetCursorPosX((availableSize.x - textSize.x) * 0.5f);
        
        ImGui::Text("IMPORT IMAGE");
        ImGui::PopFont();

        ImGui::PushFont(ui->boldFont);
        ImGui::Text("File path: ");
        ImGui::PopFont();

        ImGui::SameLine();
        ImGui::InputText("##FileDialogText", text, 255);

        if (ImGui::Button("Import", ImVec2(availableSize.x, 30)))
        {
            std::string str_text = text;
            core->imageGallery->ImportImage(str_text, "test");
        }

        ImGui::EndPopup();
    }

    ImGui::End();
}
