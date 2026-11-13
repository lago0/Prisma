#pragma once

#include <iostream>
#include <stdlib.h>
#include <opencv2/opencv.hpp>

#include "node/Node.h"
#include "node/NodeInput.h"
#include "node/NodeInput.h"
#include "node/NodeProperty.h"

class GaussianBlur : public Node
{
    public:
        GaussianBlur() :
            Node(InOutType::BUFFER)
        {
            nodeInputs[0] = new NodeInput(this, InOutType::BUFFER);

            properties.resize(2);

            properties[0] = CreateIntNodeProperty(this, "Weight", 10, 1, 100);
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
                
                int weight = std::dynamic_pointer_cast<IntNodeProperty>(properties[0])->GetValue();

                Buffer4* newBuffer = new Buffer4(width, height);

                cv::Mat originBuffer(lastBuffer->height, lastBuffer->width, CV_8UC4, lastBuffer->pixels.data());

                cv::UMat originBufferU = originBuffer.getUMat(cv::ACCESS_READ);
                cv::UMat blurredBufferU;

                if (weight % 2 == 0) {
                    weight++;
                }

                cv::GaussianBlur(originBufferU, blurredBufferU, cv::Size(weight, weight), 0);

                cv::Mat blurredBuffer(lastBuffer->height, lastBuffer->width, CV_8UC4, newBuffer->pixels.data());

                blurredBufferU.copyTo(blurredBuffer);

                cachedBuffer = newBuffer;
                isDirty = false;
            }

            return static_cast<void*>(cachedBuffer);
        };

        virtual std::string visualName() const override
        {
            return "Gaussian Blur";
        }
};