#pragma once

#include <vector>
#include <stdint.h>
#include <SDL3/SDL.h>
#include <imgui.h>

#include "ui/Window.h"
#include "util/Buffer.h"
#include "ui/UI.h"
#include "core/Core.h"
#include "compositor/Compositor.h"

namespace UI
{
    
    class TestWindow : public Window
    {
        public:
            TestWindow(UI* ui) :
                Window(ui)
            {
                texture = SDL_CreateTexture(
                    ui->renderer,
                    SDL_PIXELFORMAT_ARGB8888,
                    SDL_TEXTUREACCESS_STREAMING,
                    1920,
                    1080
                );
            };

            ~TestWindow()
            {
                if (texture != nullptr)
                {
                    SDL_DestroyTexture(texture);
                }
            }

            virtual void Update() override
            {
                Buffer4* buffer = ui->GetCore()->GetCompositor()->GetComposedViewportBuffer();

                if (buffer != nullptr)
                {
                    real_buffer = buffer->ToUint32Buffer();

                    SDL_Rect rect;
                    rect.x = 0;
                    rect.y = 0;
                    rect.w = buffer->width;
                    rect.h = buffer->height;
                    
                    SDL_UpdateTexture(texture, &rect, real_buffer.data(), 4*buffer->width);
                }
            };

            virtual void Render() override
            {
                ImTextureID texture_id = (ImTextureID)(intptr_t)texture;

                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

                ImGui::Begin("Viewport");

                ImVec2 windowSize = ImGui::GetWindowSize();
                ImVec2 size = ImVec2(windowSize.y * ((float)texture->w / (float)texture->h), windowSize.y); 

                ImGui::SetCursorPosX(windowSize.x / 2 - size.x / 2);
                
                ImGui::Image(texture_id, size);

                ImGui::End();

                ImGui::PopStyleVar();
            };

            std::vector<uint32_t> real_buffer;
            SDL_Texture* texture;
    };
}