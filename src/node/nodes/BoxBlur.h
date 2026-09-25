#pragma once

#include <iostream>
#include <stdlib.h>

#include "node/Node.h"
#include "node/NodeInput.h"
#include "node/NodeInput.h"
#include "node/NodeProperty.h"

class BoxBlur : public Node
{
    public:
        BoxBlur() :
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

                Buffer4* tempBuffer = new Buffer4(width, height);
                Buffer4* newBuffer = new Buffer4(width, height);

                for (int y = 0; y < height; y++) {
                    int rowOffset = y * width;
                    for (int x = 0; x < width; x++) {
                        int sumR = 0, sumG = 0, sumB = 0, count = 0;
                        int startX = std::max(0, x - weight);
                        int endX = std::min(width - 1, x + weight);

                        for (int newX = startX; newX <= endX; newX++) {
                            Color4 pixel = lastBuffer->pixels[rowOffset + newX];
                            sumR += pixel.r; sumG += pixel.g; sumB += pixel.b;
                            count++;
                        }
                        tempBuffer->pixels[rowOffset + x] = Color4(sumR / count, sumG / count, sumB / count, lastBuffer->pixels[rowOffset + x].a);
                    }
                }

                for (int y = 0; y < height; y++) {
                    int rowOffset = y * width;
                    for (int x = 0; x < width; x++) {
                        int sumR = 0, sumG = 0, sumB = 0, count = 0;
                        int startY = std::max(0, y - weight);
                        int endY = std::min(height - 1, y + weight);

                        for (int newY = startY; newY <= endY; newY++) {
                            Color4 pixel = tempBuffer->pixels[newY * width + x];
                            sumR += pixel.r; sumG += pixel.g; sumB += pixel.b;
                            count++;
                        }
                        newBuffer->pixels[rowOffset + x] = Color4(sumR / count, sumG / count, sumB / count, tempBuffer->pixels[rowOffset + x].a);
                    }
                }

                delete tempBuffer;
                cachedBuffer = newBuffer;
                isDirty = false;
            }

            return static_cast<void*>(cachedBuffer);
        };

        virtual std::string visualName() const override
        {
            return "Box Blur";
        }
};