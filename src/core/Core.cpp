#include <iostream>
#include <string>

#include "Core.h"
#include "util/signal/Signal.h"

#include "node/Node.h"
#include "node/NodeInput.h"
#include "node/NodeWorkspace.h"
#include "node/nodes/ImageIn.h"
#include "node/nodes/ImageOut.h"
#include "node/enums/InOutType.h"
#include "layer/LayerManager.h"
#include "compositor/Compositor.h"

static NodeWorkspace* nodeWorkspace;

Core::Core() :
    layerManager(new LayerManager(this)),
    compositor(new Compositor(this))
{
    std::cout << "Initialized\n";
}

Core::~Core()
{
    delete layerManager;
    delete compositor;
}

void Core::Start()
{
}

Buffer4* Core::GetOutput()
{
    //return static_cast<Buffer4*>(nodeWorkspace->GetOutput());
    return nullptr;
}

Compositor* Core::GetCompositor()
{
    return compositor;
}