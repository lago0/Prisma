#pragma once

namespace UI
{
    class UI;

    class Window
    {
        public:
            Window(UI* ui);

            virtual void Update();
            virtual void Render();

        protected:
            UI* ui;
    };
}