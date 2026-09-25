#pragma once

#include <iostream>

#include "node/Node.h"
#include "node/NodeInput.h"

class Grayscale : public Node
{
    public:
        Grayscale() :
            Node(InOutType::BUFFER)
        {
            nodeInputs[0] = new NodeInput(this, InOutType::BUFFER);
        }

        virtual void* GetOutput() override
        {
            Buffer4* buffer = new Buffer4(1920, 1080);

            for (int i = 0; i < buffer->size; i++) {
                buffer->pixels[i] = Color4(
                    (i / 1920) * 255,
                    (i / 1920) * 123,
                    (i / 1920) * 200,
                    255
                );
            }

            return static_cast<void*>(buffer);
        };

        virtual void* GetViewportOutput() override
        {
            if (!nodeInputs[0]->connected) return nullptr;

            if (isDirty)
            {
                if (cachedBuffer != nullptr)
                {
                    delete cachedBuffer;
                    cachedBuffer = nullptr;
                }

                Buffer4* lastBuffer = static_cast<Buffer4*>(nodeInputs[0]->ConnectedNode()->GetViewportOutput());
                if (lastBuffer == nullptr) return nullptr;
                Buffer4* newBuffer = new Buffer4(lastBuffer->width, lastBuffer->height);

                for (int i = 0; i < lastBuffer->size; i++) {
                    Color4 color = lastBuffer->PixelAt(i);
                    int component = (color.r + color.g + color.b) / 3;
                    color.r = component;
                    color.g = component;
                    color.b = component;
                    newBuffer->pixels[i] = color;
                }

                cachedBuffer = newBuffer;
                isDirty = false;
            }
            return static_cast<void*>(cachedBuffer);
        };

        virtual std::string visualName() const override
        {
            return "Grayscale";
        }
};