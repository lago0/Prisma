#pragma once

#include <string>
#include <vector>

#include "node/Node.h"
#include "node/NodeWorkspace.h"
#include "node/nodes/ImageIn.h"
#include "node/nodes/Grayscale.h"
#include "node/nodes/BoxBlur.h"

class NodeMap
{
    public:
        NodeMap(std::string name) :
            nodeName(name)
        {
        }
        std::string nodeName;
        virtual Node* Create(NodeWorkspace* workspace)
        {
            return nullptr;
        }
};

class MediaInNodeMap : public NodeMap
{
    public:
        MediaInNodeMap() :
            NodeMap("Media In")
        {
        }
        virtual Node* Create(NodeWorkspace* workspace)
        {
            Node* node = new ImageIn();
            workspace->AddNode(node);
            return node;
        }
};

class MediaOutNodeMap : public NodeMap
{
    public:
        MediaOutNodeMap() :
            NodeMap("Media Out")
        {
        }
        virtual Node* Create(NodeWorkspace* workspace)
        {
            Node* node = new ImageOut();
            workspace->AddNode(node);
            return node;
        }
};

class GrayscaleNodeMap : public NodeMap
{
    public:
        GrayscaleNodeMap() :
            NodeMap("Grayscale")
        {
        }
        virtual Node* Create(NodeWorkspace* workspace)
        {
            Node* node = new Grayscale();
            workspace->AddNode(node);
            return node;
        }
};

class BoxBlurNodeMap : public NodeMap
{
    public:
        BoxBlurNodeMap() :
            NodeMap("Box Blur")
        {
        }
        virtual Node* Create(NodeWorkspace* workspace)
        {
            Node* node = new BoxBlur();
            workspace->AddNode(node);
            return node;
        }
};

std::vector<NodeMap*> mapNodes()
{
    std::vector<NodeMap*> maps = {
        new MediaInNodeMap(),
        new MediaOutNodeMap(),
        new GrayscaleNodeMap(),
        new BoxBlurNodeMap()
    };

    return maps;
}