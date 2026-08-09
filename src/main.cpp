#include <bits/stdc++.h>
#include <cmath>

#include <SDL3/SDL.h>
#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_sdlrenderer3.h>

#include "core/Core.h"

#define USE_UI

#ifdef USE_UI
    #include "ui/UI.h"
#endif

#ifdef USE_UI
int InitUI(Core* core)
{
    if (!SDL_Init(SDL_INIT_VIDEO)) 
    {
        SDL_Log("Could not initialize SDL: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("Image Editor", 1920/2, 1080/2, SDL_WINDOW_RESIZABLE);
    if (window == nullptr) 
    {
        SDL_Log("Could not create SDL window: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);
    if (renderer == nullptr)
    {
        SDL_Log("Could not create SDL renderer: %s\n", SDL_GetError());
        return 1;
    }

    SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    SDL_ShowWindow(window);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGui::StyleColorsDark();

    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 5;
    style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
    style.FrameRounding = 5;
    style.FontScaleDpi = 1.1;
    style.WindowPadding = ImVec2(5,5);
    style.PopupRounding = 5;
    style.PopupBorderSize = 0;
    style.SelectableRounding = 5;

    ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer3_Init(renderer);

    UI::UI* ui = new UI::UI(window, renderer, core);

    bool running = true;

    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL3_ProcessEvent(&event);
            if (event.type == SDL_EVENT_QUIT) running = false;
            if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED && event.window.windowID == SDL_GetWindowID(window)) running = false;
        }

        // [If using SDL_MAIN_USE_CALLBACKS: all code below would likely be your SDL_AppIterate() function]
        if (SDL_GetWindowFlags(window) & SDL_WINDOW_MINIMIZED)
        {
            SDL_Delay(10);
            continue;
        }

        // Start the Dear ImGui frame
        ImGui_ImplSDLRenderer3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        ImGui::DockSpaceOverViewport();
        
        ui->Update();
        ui->Render();

        ImGui::Render();
        SDL_RenderClear(renderer);
        ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);
        SDL_RenderPresent(renderer);
    }

    delete ui;

    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
#endif

int main(int argc, char** argv)
{
    Core* core = new Core();
    std::thread core_thread(&Core::Start, core);
    
#ifdef USE_UI
    std::thread ui_thread(InitUI, core);
    ui_thread.join();
#else
    core_thread.join();
#endif

    delete core;

    return 0;
}