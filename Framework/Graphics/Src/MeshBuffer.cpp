#include "Precompiled.h"
#include "MeshBuffer.h"
#include "GraphicsSystem.h"

using namespace RedSnowEngine;
using namespace RedSnowEngine::Graphics;

void MeshBuffer::Initialize(const void* vertices, uint32_t vertexSize, uint32_t vertexCount)
{
    CreateVertexBuffer(vertices, vertexSize, vertexCount);
}

void MeshBuffer::Terminate()
{
    SafeRelease(mVertexBuffer);
}

void MeshBuffer::SetTopology(Topology topology)
{
    switch (topology)
    {
    case Topology::Points:
        mTopology = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
        break;
    case Topology::Lines:
        mTopology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
        break;
    case Topology::Triangles:
        mTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
        break;
    default:
        break;
    }
}

void MeshBuffer::Render() const
{
    auto context = GraphicsSystem::Get()->GetContext();
    UINT offset = 0;
    context->IASetVertexBuffers(0, 1, &mVertexBuffer, &mVertexSize, &offset);
    context->Draw(static_cast<UINT>(mVertexCount), 0);
}

void MeshBuffer::CreateVertexBuffer(const void* vertices, uint32_t vertexSize, uint32_t vertexCount)
{
    mVertexSize = vertexSize;
    mVertexCount = vertexCount;

    auto device = GraphicsSystem::Get()->GetDevice();

    D3D11_BUFFER_DESC bufferDesc{};
    bufferDesc.ByteWidth = vertexSize * vertexCount;
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bufferDesc.MiscFlags = 0;
    bufferDesc.StructureByteStride = 0;

    D3D11_SUBRESOURCE_DATA initDate = {};
    initDate.pSysMem = vertices;

    HRESULT hr = device->CreateBuffer(&bufferDesc, &initDate, &mVertexBuffer);
    ASSERT(SUCCEEDED(hr), "Failed to create vertex buffer");
}