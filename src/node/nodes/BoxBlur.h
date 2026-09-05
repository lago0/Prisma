#pragma once

#include <iostream>

#include "node/Node.h"
#include "node/NodeInput.h"

class BoxBlur : public Node
{
    public:
        BoxBlur() :
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
                }

                Buffer4* lastBuffer = static_cast<Buffer4*>(nodeInputs[0]->ConnectedNode()->GetViewportOutput());
                if (lastBuffer == nullptr) return nullptr;
                Buffer4* newBuffer = new Buffer4(lastBuffer->width, lastBuffer->height);

                for (int i = 0; i < lastBuffer->size; i++) {
                    int x = i % lastBuffer->width;
                    int y = i / lastBuffer->width;

                    int sumR = 0;
                    int sumG = 0;
                    int sumB = 0;

                    int count = 0;
                    
                    for (int newX = x - 1; newX <= x + 1; newX++)
                    {
                        for (int newY = y - 1; newY <= y + 1; newY++)
                        {
                            if (newX >= 0 && newX < lastBuffer->width && newY >= 0 && newY < lastBuffer->height)
                            {
                                Color4 pixel = lastBuffer->PixelAt(newX, newY);

                                sumR += pixel.r;
                                sumG += pixel.g;
                                sumB += pixel.b;

                                count++;
                            }
                        }
                    }
                    
                    newBuffer->pixels[i] = Color4(sumR / count, sumG / count, sumB / count, lastBuffer->PixelAt(i).a);
                }

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