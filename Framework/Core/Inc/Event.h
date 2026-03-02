#pragma once

namespace RedSnowEngine::Core
{
    using EventTypeId = std::size_t;

    class Event
    {
    public:
        Event() = default;
        virtual ~Event() = default;

        virtual EventTypeId GetTypeId() const = 0;
    };
}

#define SET_EVENT_TYPE_ID(id) \
    static RedSnowEngine::Core::EventTypeId StaticGetTypeId() { return static_cast<RedSnowEngine::Core::EventTypeId>(id); } \
    RedSnowEngine::Core::EventTypeId GetTypeId() const override { return StaticGetTypeId(); }