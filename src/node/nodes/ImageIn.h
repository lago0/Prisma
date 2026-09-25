#pragma once

#include "node/Node.h"
#include "node/NodeInput.h"

class ImageIn : public Node
{
    public:
        ImageIn() :
            Node(InOutType::BUFFER)
        {
        }

        virtual void* GetOutput() override
        {
            Buffer4* buffer = new Buffer4(1920, 1080);

            for (int i = 0; i < buffer->size; i++) {
                buffer->pixels[i] = Color4(
                    ((i % 1920) / 1920.0f) * 255,
                    ((i % 1920) / 1920.0f) * 123,
                    ((i % 1920) / 1920.0f) * 200,
                    255
                );
            }

            return static_cast<void*>(buffer);
        };

        virtual void* GetViewportOutput() override
        {
            if (isDirty)
            {
                if (cachedBuffer != nullptr)
                {
                    delete cachedBuffer;
                }

                Buffer4* newBuffer = new Buffer4(1920, 1080);

                for (int i = 0; i < newBuffer->size; i++) {
                    /*int x = i % newBuffer->width;
                    int y = i / newBuffer->width;

                    float fx = x / float(newBuffer->width - 1);
                    float fy = y / float(newBuffer->height - 1);

                    newBuffer->pixels[i] = Color4(
                        fx * 255,
                        fy * 255,
                        128,
                        255
                    );*/
                    int x = i % newBuffer->width;
                    int y = i / newBuffer->width;

                    newBuffer->pixels[i] = Color4(
                        x > 1000 ? 0 : 255,
                        255,
                        x > 1000 ? 0 : 255,
                        255
                    );
                }

                cachedBuffer = newBuffer;
                isDirty = false;
            }
            return static_cast<void*>(cachedBuffer);
        };

        virtual std::string visualName() const override
        {
            return "ImageIn";
        }
};