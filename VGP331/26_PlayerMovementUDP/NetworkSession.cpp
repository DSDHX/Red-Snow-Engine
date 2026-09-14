#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <winsock2.h>
#include <ws2tcpip.h>
#include "NetworkSession.h"
#include <cmath>
#include <cstring>

#pragma comment(lib, "ws2_32.lib")

namespace
{
    constexpr uint32_t Magic = 0x52534D5A; // RSMZ
    constexpr uint32_t Version = 1;
    constexpr uint32_t InputPacket = 1;
    constexpr uint32_t SnapshotPacket = 2;
    struct Packet
    {
        uint32_t magic = Magic, version = Version, type = 0, sequence = 0;
        uint32_t session = 0, buttons = 0;
        NetworkSession::Positions positions{};
    };
    static_assert(sizeof(Packet) == 48, "UDP packet layout changed");

    sockaddr_in LocalAddress(uint16_t networkPort)
    {
        sockaddr_in address{};
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
        address.sin_port = networkPort;
        return address;
    }
    bool IsNewer(uint32_t candidate, uint32_t previous)
    {
        const uint32_t difference = candidate - previous;
        return difference != 0 && difference < 0x80000000u;
    }
}

NetworkSession::~NetworkSession() { Terminate(); }

bool NetworkSession::Initialize(Role requested)
{
    Terminate();
    mError.clear();
    mHasSnapshot = mHasSequence = mConnected = false;
    mSendSequence = mReceiveSequence = mReceivedCount = mRemoteButtons = 0;
    mPeerAge = mInputAge = 0;
    mPeerPort = 0;
    WSADATA data{};
    const int startup = WSAStartup(MAKEWORD(2, 2), &data);
    if (startup != 0)
    {
        mError = "WSAStartup failed: " + std::to_string(startup);
        return false;
    }
    mStarted = true;
    mSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    auto fail = [this](const char* operation)
    {
        mError = std::string(operation) + ": " + std::to_string(WSAGetLastError());
        Terminate();
        return false;
    };
    if (mSocket == InvalidSocket) return fail("socket failed");
    BOOL exclusive = TRUE;
    if (setsockopt(mSocket, SOL_SOCKET, SO_EXCLUSIVEADDRUSE,
        reinterpret_cast<const char*>(&exclusive), sizeof(exclusive)) == SOCKET_ERROR)
        return fail("setsockopt failed");

    mRole = requested;
    if (requested != Role::Client)
    {
        auto address = LocalAddress(htons(Port));
        if (bind(mSocket, reinterpret_cast<sockaddr*>(&address), sizeof(address)) == 0)
            mRole = Role::Server;
        else
        {
            const int error = WSAGetLastError();
            if (requested == Role::Server || (error != WSAEADDRINUSE && error != WSAEACCES))
                return fail("Cannot start server on 127.0.0.1:27026");
            mRole = Role::Client;
        }
    }
    if (mRole == Role::Client)
    {
        auto address = LocalAddress(0);
        if (bind(mSocket, reinterpret_cast<sockaddr*>(&address), sizeof(address)) != 0)
            return fail("Cannot bind client");
        mPeerPort = htons(Port);
    }
    u_long nonblocking = 1;
    if (ioctlsocket(mSocket, FIONBIO, &nonblocking) != 0)
        return fail("Cannot enable nonblocking UDP");
    mSession = GetCurrentProcessId() ^ static_cast<uint32_t>(GetTickCount64());
    if (mSession == 0) mSession = 1;
    return true;
}

void NetworkSession::Terminate()
{
    if (mSocket != InvalidSocket) closesocket(mSocket);
    mSocket = InvalidSocket;
    if (mStarted) WSACleanup();
    mStarted = mConnected = false;
}

void NetworkSession::Receive(float deltaTime)
{
    if (!IsOpen()) return;
    mPeerAge += deltaTime;
    mInputAge += deltaTime;
    if (mPeerAge > 2.0f)
    {
        mConnected = mHasSequence = false;
        mRemoteButtons = 0;
        if (IsServer()) mPeerPort = 0;
    }

    for (int i = 0; i < 64; ++i)
    {
        char bytes[256];
        sockaddr_in from{};
        int length = sizeof(from);
        const int received = recvfrom(mSocket, bytes, sizeof(bytes), 0,
            reinterpret_cast<sockaddr*>(&from), &length);
        if (received == SOCKET_ERROR)
        {
            const int error = WSAGetLastError();
            if (error == WSAEWOULDBLOCK) break;
            if (error == WSAECONNRESET || error == WSAEMSGSIZE) continue;
            mError = "recvfrom failed: " + std::to_string(error);
            break;
        }
        if (received != sizeof(Packet) || from.sin_addr.s_addr != htonl(INADDR_LOOPBACK)) continue;
        Packet packet;
        std::memcpy(&packet, bytes, sizeof(packet));
        if (packet.magic != Magic || packet.version != Version || packet.session == 0) continue;
        if (IsServer())
        {
            if (packet.type != InputPacket || (packet.buttons & ~15u) != 0) continue;
            if (mPeerPort != 0 && (mPeerPort != from.sin_port || packet.session != mSession)) continue;
        }
        else
        {
            if (packet.type != SnapshotPacket || from.sin_port != mPeerPort || packet.session != mSession) continue;
            bool valid = true;
            for (const auto& p : packet.positions)
                valid &= std::isfinite(p.x) && std::isfinite(p.y) && std::isfinite(p.z)
                    && std::abs(p.x) < 100 && std::abs(p.y) < 100 && std::abs(p.z) < 100;
            if (!valid) continue;
        }
        if (mHasSequence && !IsNewer(packet.sequence, mReceiveSequence)) continue;
        if (IsServer())
        {
            mPeerPort = from.sin_port;
            mSession = packet.session;
            mRemoteButtons = packet.buttons;
            mInputAge = 0;
        }
        else
        {
            mPositions = packet.positions;
            mHasSnapshot = true;
        }
        mReceiveSequence = packet.sequence;
        mHasSequence = mConnected = true;
        mPeerAge = 0;
        ++mReceivedCount;
    }
}

void NetworkSession::Send(uint32_t type, uint32_t buttons, const Positions& positions)
{
    if (!IsOpen() || mPeerPort == 0) return;
    Packet packet;
    packet.type = type;
    packet.sequence = ++mSendSequence;
    packet.session = mSession;
    packet.buttons = buttons;
    packet.positions = positions;
    const auto address = LocalAddress(mPeerPort);
    if (sendto(mSocket, reinterpret_cast<const char*>(&packet), sizeof(packet), 0,
        reinterpret_cast<const sockaddr*>(&address), sizeof(address)) == SOCKET_ERROR)
    {
        const int error = WSAGetLastError();
        if (error != WSAEWOULDBLOCK && error != WSAECONNRESET)
            mError = "sendto failed: " + std::to_string(error);
    }
}

void NetworkSession::SendInput(uint32_t buttons)
{
    if (!IsServer()) Send(InputPacket, buttons, {});
}

void NetworkSession::SendSnapshot(const Positions& positions)
{
    if (IsServer()) Send(SnapshotPacket, 0, positions);
}