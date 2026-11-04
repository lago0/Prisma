#pragma once

#include <iostream>
#include <opencv2/opencv.hpp>

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

                /*for (int i = 0; i < lastBuffer->size; i++) {
                    Color4 color = lastBuffer->PixelAt(i);
                    int component = (color.r + color.g + color.b) / 3;
                    color.r = component;
                    color.g = component;
                    color.b = component;
                    newBuffer->pixels[i] = color;
                }*/

                cv::Mat lastBufferMat(lastBuffer->height, lastBuffer->width, CV_8UC4, lastBuffer->pixels.data());
                cv::UMat lastBufferUMat;

                lastBufferMat.copyTo(lastBufferUMat);

                cv::UMat tempBuffer;
                cv::UMat newBufferUMat;

                cv::cvtColor(lastBufferUMat, tempBuffer, cv::COLOR_RGBA2GRAY);
                cv::cvtColor(tempBuffer, newBufferUMat, cv::COLOR_GRAY2RGBA);

                cv::Mat newBufferMat(newBuffer->height, newBuffer->width, CV_8UC4, newBuffer->pixels.data());

                newBufferUMat.copyTo(newBufferMat);

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