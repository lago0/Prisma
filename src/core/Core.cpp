#include <iostream>
#include <string>

#include "Core.h"
#include "util/signal/Signal.h"

#include "node/Node.h"
#include "node/NodeInput.h"
#include "node/NodeWorkspace.h"
#include "node/nodes/CreatorNode.h"
#include "node/nodes/TestNode.h"
#include "node/enums/InOutType.h"

static Node* node;

Core::Core() :
    layerManager(new LayerManager())
{
    std::cout << "Initialized\n";
}

Core::~Core()
{
    delete layerManager;
}

void Core::Start()
{
    NodeWorkspace* workspace = new NodeWorkspace(this);

    Node* creatorNode = new CreatorNode(this);
    Node* testNode = new TestNode(this);

    workspace->AddNode(creatorNode);
    workspace->AddNode(testNode);

    creatorNode->ConnectOutput(testNode->nodeInputs[0]);

    node = testNode;
}

Buffer4* Core::GetOutput()
{
    return static_cast<Buffer4*>(node->GetOutput());
}