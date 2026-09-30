#pragma once

#include <iostream>
#include <stdlib.h>

#include "node/Node.h"
#include "node/NodeInput.h"
#include "node/NodeInput.h"
#include "node/NodeProperty.h"

class ChromaKey : public Node
{
    public:
        ChromaKey() :
            Node(InOutType::BUFFER)
        {
            nodeInputs[0] = new NodeInput(this, InOutType::BUFFER);

            properties.resize(2);

            properties[0] = CreateIntNodeProperty(this, "Weight", 1, 0, 100);
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
                if (cachedBuffer != nullptr) {
                    delete cachedBuffer;
                    cachedBuffer = nullptr;
                }

                Buffer4* lastBuffer = static_cast<Buffer4*>(nodeInputs[0]->ConnectedNode()->GetViewportOutput());
                if (lastBuffer == nullptr) return nullptr;
                
                int width = lastBuffer->width;
                int height = lastBuffer->height;
                int size = lastBuffer->size;
                int weight = *(int*)properties[0]->GetValue();

                Buffer4* newBuffer = new Buffer4(width, height);

                for (int i = 0; i < size; i++) {
                    Color4 color = lastBuffer->PixelAt(i);

                    bool lteIntensity = (color.g > 75);

                    bool isGreenTone = (color.g > (color.r + 30)) && (color.g > (color.b + 30)) && (color.r < 160);

                    if (isGreenTone)
                    {
                        color.a = 0;
                    }

                    newBuffer->pixels[i] = color;
                }

                cachedBuffer = newBuffer;
                isDirty = false;
            }

            return static_cast<void*>(cachedBuffer);
        };

        virtual std::string visualName() const override
        {
            return "Chroma Key";
        }
};