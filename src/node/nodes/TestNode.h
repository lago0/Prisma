#pragma once

#include "node/Node.h"
#include "node/NodeInput.h"

class TestNode : public Node
{
    public:
        TestNode(Core* core) :
            Node(core, InOutType::BUFFER)
        {
            nodeInputs[0] = new NodeInput(this, InOutType::BUFFER);
        }

        virtual void* GetOutput() override
        {
            if (!nodeInputs[0]->connected) return nullptr;

            if (isDirty)
            {
                if (cachedBuffer != nullptr)
                {
                    delete cachedBuffer;
                }

                Buffer4* oldBuffer = static_cast<Buffer4*>(nodeInputs[0]->ConnectedNode()->GetOutput());
                if (oldBuffer == nullptr) return nullptr;
                Buffer4* newBuffer = new Buffer4(oldBuffer->width, oldBuffer->height);

                for (int i = 0; i < oldBuffer->size; i++) {
                    Color4 pixel = oldBuffer->PixelAt(i);
                    newBuffer->pixels[i] = Color4(
                        pixel.r,// * 1.2, 
                        pixel.g,// * 1.2, 
                        pixel.b,// * 1.2, 
                        pixel.a
                    );
                }

                cachedBuffer = newBuffer;
            }
            return static_cast<void*>(cachedBuffer);
        };

        virtual void SetDirty() override
        {
            Node::SetDirty();
            core->isViewportDirty = true;
        };
};