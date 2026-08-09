#pragma once

#include "node/Node.h"
#include "node/NodeInput.h"

class CreatorNode : public Node
{
    public:
        CreatorNode(Core* core) :
            Node(core, InOutType::BUFFER)
        {
        }

        virtual void* GetOutput() override
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
                        255,
                        255,
                        255,
                        255
                    );
                }

                cachedBuffer = newBuffer;
            }
            return static_cast<void*>(cachedBuffer);
        };
};