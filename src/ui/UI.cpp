#include "UI.h"
#include "core/Core.h"
#include "layer/LayerManager.h"
#include "Window.h"
#include "window/LayerWindow/LayerWindow.h"
#include "window/TestWindow/TestWindow.h"
#include "window/NodeViewer/NodeViewer.h"
#include "window/Inspector/Inspector.h"
#include "ui/window/NodeViewer/NodeView.h"

#include "util/Time.h"

#include <iostream>

UI::UI::UI(SDL_Window* main_window, SDL_Renderer* renderer, Core* core) :
    main_window(main_window),
    renderer(renderer),
    core(core),
    onLayerSelectedChange(new Util::Signal<Layer*>()),
    onEvent(new Util::Signal<SDL_Event*>())
{
    std::shared_ptr<Window> window = std::make_shared<TestWindow>(this);

    OpenWindow(window);

    window = std::make_shared<LayerWindow>(this);

    OpenWindow(window);

    window = std::make_shared<NodeViewer>(this);

    OpenWindow(window);

    window = std::make_shared<Inspector>(this);

    OpenWindow(window);
}

UI::UI::~UI()
{
    delete onLayerSelectedChange, onEvent;
}

Core* UI::UI::GetCore()
{
    return core;
}

// Updates all visible windows
void UI::UI::Update()
{
    double time = Time::GetUnixTimestamp();
    deltaTime = (float)(time - lastTime);

    for (std::shared_ptr<Window>& window : currentWindows)
    {
        window->Update();
    }

    lastTime = time;
}

// Renders all visible windows
void UI::UI::Render()
{
    for (std::shared_ptr<Window>& window : currentWindows)
    {
        window->Render();
    }
}

// Draws main menu bar
void UI::UI::DrawMenu()
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("Layer"))
        {
            if (ImGui::MenuItem("New layer"))
            {
                core->layerManager->CreateLayer();
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}

// Opens a window
void UI::UI::OpenWindow(std::shared_ptr<Window>& window)
{
    currentWindows.push_back(window);
}

// Closes an opened window;
void UI::UI::CloseWindow(std::shared_ptr<Window>& window)
{
    int index = GetWindowIndex(window);
    if (index >= 0)
    {
        currentWindows.erase(currentWindows.begin() + index);
    }
}

// Gets a opened window index
int UI::UI::GetWindowIndex(std::shared_ptr<Window>& window)
{
    for (int i = 0; i < currentWindows.size(); i++)
    {
        if (currentWindows[i] == window)
        {
            return i;
        }
    }
    return -1;
}

// Sets current selected layer;
void UI::UI::SetSelectedLayer(Layer* layer)
{
    selectedLayer = layer;
    onLayerSelectedChange->Emit(layer);
}

void UI::UI::HandleEvent(SDL_Event& event)
{
    onEvent->Emit(&event);
}