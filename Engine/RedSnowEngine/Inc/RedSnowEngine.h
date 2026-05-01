#pragma once

#include "Common.h"

#include "App.h"
#include "AppState.h"
#include "SaveUtil.h"

#include "GameObject.h"
#include "GameObjectHandle.h"
#include "GameObjectFactory.h"
#include "GameWorld.h"

#include "TypeIds.h"
#include "Component.h"
#include "TransformComponent.h"
#include "CameraComponent.h"
#include "FPSCameraComponent.h"
#include "RenderObjectComponent.h"

#include "Service.h"
#include "CameraService.h"
#include "RenderService.h"

namespace RedSnowEngine
{
    App& MainApp();
}