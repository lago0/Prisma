#pragma once

#include <imgui.h>
#include <memory>
#include <string>

#include "util/signal/Signal.h"

class Core;

/**
 * @class SelectImageDialog
 * @brief Class for select image source dialogs
 * @author lago0
 */

namespace UI
{
    class UI;
    
    class SelectImageDialog
    {
        public:
            SelectImageDialog(UI* ui, Core* core);
            ~SelectImageDialog();

            bool IsOpen();

            void Open();
            void Close();

            void Render();

            std::unique_ptr<Util::Signal<>> imageSelectedSignal; // Image selected
            std::unique_ptr<Util::Signal<>> cancelledSignal; // Cancelled

            std::unique_ptr<Util::Signal<>> optionSelectedSignal; // Cancelled or image selected

            bool cancelled = false;
            int imageIdSelected = -1;

            int dialogId;
            std::string dialogName;
        private:
            bool open = false;

            Core* core;
            UI* ui;
    };
}