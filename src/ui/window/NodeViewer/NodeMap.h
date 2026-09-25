#pragma once

#include <string>
#include <vector>

#include "node/Node.h"
#include "node/NodeWorkspace.h"
#include "node/nodes/ImageIn.h"
#include "node/nodes/Grayscale.h"
#include "node/nodes/BoxBlur.h"
#include "node/nodes/ChromaKey.h"

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

class ChromaKeyNodeMap : public NodeMap
{
    public:
        ChromaKeyNodeMap() :
            NodeMap("Chroma Key")
        {
        }
        virtual Node* Create(NodeWorkspace* workspace)
        {
            Node* node = new ChromaKey();
            workspace->AddNode(node);
            return node;
        }
};

inline std::vector<std::shared_ptr<NodeMap>> mapNodes()
{
    std::vector<std::shared_ptr<NodeMap>> maps = {
        std::make_shared<MediaInNodeMap>(),
        std::make_shared<GrayscaleNodeMap>(),
        std::make_shared<BoxBlurNodeMap>(),
        std::make_shared<ChromaKeyNodeMap>()
    };

    return maps;
}