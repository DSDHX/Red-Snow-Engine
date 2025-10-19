#include "Precompiled.h"
#include "RenderObject.h"

using namespace RedSnowEngine;
using namespace RedSnowEngine::Graphics;

void RenderObject::Terminate()
{
    meshBuffer.Terminate();
}