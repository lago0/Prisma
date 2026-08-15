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
                    125,
                    240,
                    104,
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
                    newBuffer->pixels[i] = Color4(
                        125,
                        240,
                        104,
                        255
                    );
                }

                cachedBuffer = newBuffer;
                isDirty = false;
            }
            return static_cast<void*>(cachedBuffer);
        };
};