#pragma once

#include "Component.h"

namespace RedSnowEngine
{
    class UIComponent : public Component
    {
    public:
        virtual void Render() = 0;
    };
}