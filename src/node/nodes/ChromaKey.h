#pragma once

#include <iostream>
#include <stdlib.h>
#include <opencv2/opencv.hpp>

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
                int weight = std::dynamic_pointer_cast<IntNodeProperty>(properties[0])->GetValue();

                Buffer4* newBuffer = new Buffer4(width, height);

                /*for (int i = 0; i < size; i++) {
                    Color4 color = lastBuffer->PixelAt(i);

                    bool lteIntensity = (color.g > 75);

                    bool isGreenTone = (color.g > (color.r + 30)) && (color.g > (color.b + 30)) && (color.r < 160);

                    if (isGreenTone)
                    {
                        color.a = 0;
                    }

                    newBuffer->pixels[i] = color;
                }*/
               
                cv::Mat lastBufferMat(lastBuffer->height, lastBuffer->width, CV_8UC4, lastBuffer->pixels.data());

                cv::UMat lastBufferUMat;
                lastBufferMat.copyTo(lastBufferUMat); 

                cv::UMat bgrForHsv;
                cv::cvtColor(lastBufferUMat, bgrForHsv, cv::COLOR_RGBA2BGR);

                cv::UMat hsv;
                cv::cvtColor(bgrForHsv, hsv, cv::COLOR_BGR2HSV);

                cv::UMat mask;
                cv::inRange(hsv, cv::Scalar(35, 40, 40), cv::Scalar(85, 255, 255), mask);

                cv::UMat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3)).getUMat(cv::ACCESS_READ);
                cv::morphologyEx(mask, mask, cv::MORPH_CLOSE, kernel);
                cv::morphologyEx(mask, mask, cv::MORPH_OPEN, kernel);

                cv::UMat mask_inv;
                cv::bitwise_not(mask, mask_inv);

                std::vector<cv::UMat> rgba_channels;
                cv::split(lastBufferUMat, rgba_channels);

                rgba_channels[3] = mask_inv; 

                cv::UMat rgba_gpu;
                cv::merge(rgba_channels, rgba_gpu);

                cv::Mat newBufferMat(newBuffer->height, newBuffer->width, CV_8UC4, newBuffer->pixels.data());

                rgba_gpu.copyTo(newBufferMat);

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