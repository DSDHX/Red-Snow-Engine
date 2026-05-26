#pragma once

namespace RedSnowEngine
{
    class GameObject;
    class GameWorld;
    class Component;

    namespace GameObjectFactory
    {
        void Make(const std::filesystem::path& templatePath, GameObject& gameObject, GameWorld& gameWorld);
        void OverrideDeserialize(const rapidjson::Value& value, GameObject& gameObject);
    }
}