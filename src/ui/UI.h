#pragma once

#include <SDL3/SDL.h>

#include <vector>
#include <memory>

class Core;

namespace UI 
{
    class Window;

    class UI
    {
        public:
            UI(SDL_Window* window, SDL_Renderer* renderer, Core* core);
            
            Core* GetCore();

            void Update();
            void Render();

            void OpenWindow(std::shared_ptr<Window>& window);
            void CloseWindow(std::shared_ptr<Window>& window);

            int GetWindowIndex(std::shared_ptr<Window>& window);

            std::vector<std::shared_ptr<Window>> currentWindows;

            double lastTime;
            float deltaTime;

            SDL_Window* main_window;
            SDL_Renderer* renderer;

        private:
            Core* core;
    };
}