#pragma once

#include <vector>

#include "ui/Window.h"

namespace UI
{
    class UI;
    class SelectImageDialog;
    
    class Inspector : public Window
    {
        public:
            Inspector(UI* ui);

            virtual void Update() override;
            virtual void Render() override;

            bool imageDialogOpened = false;
            SelectImageDialog* currentSelectImageDialog;
            std::string selectImageDialogPropertyId = "";
    };
}