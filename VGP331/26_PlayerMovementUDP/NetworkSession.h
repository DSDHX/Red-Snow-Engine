#pragma once

#include <array>
#include <cstdint>
#include <string>

class NetworkSession final
{
public:
    enum class Role { Automatic, Server, Client };
    enum Button : uint32_t { Forward = 1, Back = 2, Left = 4, Right = 8 };
    static constexpr uint16_t Port = 27026;
    static constexpr float SendInterval = 1.0f / 30.0f;
    struct Position { float x = 0, y = 0, z = 0; };
    using Positions = std::array<Position, 2>;

    ~NetworkSession();
    bool Initialize(Role requested = Role::Automatic);
    void Terminate();
    void Receive(float deltaTime);
    void SendInput(uint32_t buttons);
    void SendSnapshot(const Positions& positions);
    bool IsServer() const { return mRole == Role::Server; }
    bool IsOpen() const { return mStarted && mSocket != InvalidSocket; }
    bool IsConnected() const { return mConnected; }
    bool HasSnapshot() const { return mHasSnapshot; }
    uint32_t RemoteButtons() const { return mInputAge < 0.25f ? mRemoteButtons : 0; }
    const Positions& GetPositions() const { return mPositions; }
    const std::string& Error() const { return mError; }
    uint32_t ReceivedCount() const { return mReceivedCount; }

private:
    static constexpr uintptr_t InvalidSocket = ~uintptr_t(0);
    void Send(uint32_t type, uint32_t buttons, const Positions& positions);
    uintptr_t mSocket = InvalidSocket;
    Role mRole = Role::Automatic;
    bool mStarted = false;
    bool mConnected = false;
    bool mHasSequence = false;
    bool mHasSnapshot = false;
    uint16_t mPeerPort = 0;
    uint32_t mSession = 0;
    uint32_t mSendSequence = 0;
    uint32_t mReceiveSequence = 0;
    uint32_t mRemoteButtons = 0;
    uint32_t mReceivedCount = 0;
    float mPeerAge = 0;
    float mInputAge = 0;
    Positions mPositions{};
    std::string mError;
};