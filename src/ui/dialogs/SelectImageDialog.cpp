#include <iostream>

#include "SelectImageDialog.h"
#include "core/Core.h"
#include "imagegallery/ImageGallery.h"
#include "imagegallery/Image.h"
#include "ui/UI.h"

UI::SelectImageDialog::SelectImageDialog(UI* ui, Core* core) :
    ui(ui),
    core(core),
    imageSelectedSignal(std::make_unique<Util::Signal<>>()),
    cancelledSignal(std::make_unique<Util::Signal<>>()),
    optionSelectedSignal(std::make_unique<Util::Signal<>>())
{
    dialogId = rand() % 1000;
    dialogName = "SelectImageDialog" + std::to_string(dialogId);
}

UI::SelectImageDialog::~SelectImageDialog()
{

}

bool UI::SelectImageDialog::IsOpen()
{
    return this->open;
}

void UI::SelectImageDialog::Open()
{
    this->open = true;
}

void UI::SelectImageDialog::Close()
{
    this->open = false;
}

void UI::SelectImageDialog::Render()
{
    if (!this->open)
    {
        return;
    }

    ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f));

    ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | 
                            ImGuiWindowFlags_NoBackground | 
                            ImGuiWindowFlags_NoMove | 
                            ImGuiWindowFlags_NoResize | 
                            ImGuiWindowFlags_NoSavedSettings |
                            ImGuiWindowFlags_NoInputs;

    ImGui::Begin(("Select an image##" + dialogName).c_str(), nullptr, ImGuiWindowFlags_NoTitleBar);
    
    ImGui::PushFont(ui->titleFont);

    ImVec2 textSize = ImGui::CalcTextSize("PLEASE SELECT AN IMAGE");
    ImVec2 availableSize = ImGui::GetContentRegionAvail();

    ImGui::SetCursorPosX((availableSize.x - textSize.x) * 0.5f);
    ImGui::Text("PLEASE SELECT AN IMAGE");
    ImGui::PopFont();

    ImGui::Separator();

    for (auto data : core->imageGallery->GetImages())
    {
        if (ImGui::Button((data.second->name + "##" + std::to_string(data.second->id)).c_str(), ImVec2(ImGui::GetWindowSize()[0] - 10, 60)))
        {
            this->imageIdSelected = data.second->id;
            this->imageSelectedSignal->Emit();
            this->optionSelectedSignal->Emit();

            this->Close();

            ImGui::End();
            return;
        }
    }

    if (ImGui::Button("Cancel", ImVec2(ImGui::GetWindowSize()[0] - 10, 60)))
    {
        this->cancelled = true;
        this->cancelledSignal->Emit();
        this->optionSelectedSignal->Emit();

        this->Close();

        ImGui::End();
        return;
    }

    ImGui::End();
    
}