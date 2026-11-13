#pragma once

#include <SDL3/SDL.h>
#include <vector>
#include <memory>
#include <imgui.h>

#include "core/Core.h"
#include "layer/Layer.h"
#include "util/signal/Signal.h"

namespace UI 
{
    class Window;
    class NodeView;
    class SelectImageDialog;

    class UI
    {
        public:
            UI(SDL_Window* window, SDL_Renderer* renderer, Core* core);
            ~UI();
            
            Core* GetCore();

            void Update();
            void Render();

            void DrawMenu();

            void OpenWindow(std::shared_ptr<Window>& window);
            void CloseWindow(std::shared_ptr<Window>& window);

            int GetWindowIndex(std::shared_ptr<Window>& window);

            void SetSelectedLayer(Layer* layer);

            void HandleEvent(SDL_Event& event);

            std::vector<std::shared_ptr<Window>> currentWindows;

            double lastTime;
            float deltaTime;

            SDL_Window* main_window;
            SDL_Renderer* renderer;

            Layer* selectedLayer = nullptr;

            Util::Signal<Layer*>* onLayerSelectedChange;
            Util::Signal<SDL_Event*>* onEvent;

            std::shared_ptr<NodeView> selectedNodeView = nullptr;

            SelectImageDialog* selectImageDialog = nullptr;

            ImFont* boldFont;
            ImFont* titleFont;

        private:
            Core* core;
    };
}