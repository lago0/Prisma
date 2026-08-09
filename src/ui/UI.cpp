#include "UI.h"
#include "core/Core.h"
#include "Window.h"
#include "window/LayerWindow/LayerWindow.h"
#include "window/TestWindow/TestWindow.h"

#include "util/Time.h"

#include <iostream>

UI::UI::UI(SDL_Window* main_window, SDL_Renderer* renderer, Core* core) :
    main_window(main_window),
    renderer(renderer),
    core(core)
{
    std::shared_ptr<Window> window = std::make_shared<TestWindow>(this);

    OpenWindow(window);
}

Core* UI::UI::GetCore()
{
    return core;
}

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

void UI::UI::Render()
{
    for (std::shared_ptr<Window>& window : currentWindows)
    {
        window->Render();
    }
}

void UI::UI::OpenWindow(std::shared_ptr<Window>& window)
{
    currentWindows.push_back(window);
}

void UI::UI::CloseWindow(std::shared_ptr<Window>& window)
{
    int index = GetWindowIndex(window);
    currentWindows.erase(currentWindows.begin() + index);
}

int UI::UI::GetWindowIndex(std::shared_ptr<Window>& window)
{
    return 0;
}