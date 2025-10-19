#include "Precompiled.h"
#include "RenderObject.h"

using namespace RedSnowEngine;
using namespace RedSnowEngine::Graphics;

void RenderObject::Terminate()
{
    meshBuffer.Terminate();
    TextureManager* tm = TextureManager::Get();
    tm->RelaseTexture(diffuseMapId);
    tm->RelaseTexture(specMapId);
    tm->RelaseTexture(normalMapId);
    tm->RelaseTexture(bumpMapId);
}