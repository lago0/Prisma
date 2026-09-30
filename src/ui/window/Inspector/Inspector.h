#pragma once

#include <vector>

#include "ui/Window.h"

namespace UI
{
    class UI;
    
    class Inspector : public Window
    {
        public:
            Inspector(UI* ui);

            virtual void Update() override;
            virtual void Render() override;
    };
}