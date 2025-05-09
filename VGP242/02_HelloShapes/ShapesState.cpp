#include "ShapesState.h"

using namespace RedSnowEngine;
using namespace RedSnowEngine::Graphics;
using namespace RedSnowEngine::Math;

void ShapeState::Initialize()
{
    CreateShape();

    ASSERT(!mVertices.empty(), "No vertices to create a shape");

    auto device = GraphicsSystem::Get()->GetDevice();
    SafeRelease(mVertexBuffer);

    D3D11_BUFFER_DESC bufferDesc{};
    bufferDesc.ByteWidth = static_cast<UINT>(mVertices.size() * sizeof(Vertex));
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bufferDesc.MiscFlags = 0;
    bufferDesc.StructureByteStride = 0;

    D3D11_SUBRESOURCE_DATA initDate = {};
    initDate.pSysMem = mVertices.data();

    HRESULT hr = device->CreateBuffer(&bufferDesc, &initDate, &mVertexBuffer);
    ASSERT(SUCCEEDED(hr), "Failed to create vertex buffer");
    //==============================================================

    std::filesystem::path shaderFilePath = L"../../Assets/Shaders/DoColor.fx";

    DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;
    ID3DBlob* shaderBlob = nullptr;
    ID3DBlob* errorBlob = nullptr;
    hr = D3DCompileFromFile(
        shaderFilePath.c_str(),
        nullptr,
        D3D_COMPILE_STANDARD_FILE_INCLUDE,
        "VS", "vs_5_0",
        shaderFlags, 0,
        &shaderBlob,
        &errorBlob);
    if (errorBlob != nullptr && errorBlob->GetBufferPointer() != nullptr)
    {
        LOG("%s", static_cast<const char*>(errorBlob->GetBufferPointer()));
    }
    ASSERT(SUCCEEDED(hr), "Failed to compile vertex shader");

    hr = device->CreateVertexShader(
        shaderBlob->GetBufferPointer(),
        shaderBlob->GetBufferSize(),
        nullptr,
        &mVertexShader);
    ASSERT(SUCCEEDED(hr), "Failed to create vertex shader");
    //==============================================================

    std::vector<D3D11_INPUT_ELEMENT_DESC> vertexLayout;
    vertexLayout.push_back({ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT});
    vertexLayout.push_back({ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT });

    hr = device->CreateInputLayout(
        vertexLayout.data(),
        static_cast<UINT>(vertexLayout.size()),
        shaderBlob->GetBufferPointer(),
        shaderBlob->GetBufferSize(),
        &mInputLayout);
    ASSERT(SUCCEEDED(hr), "Failed to create input layout");
    SafeRelease(shaderBlob);
    SafeRelease(errorBlob);
    //==============================================================

    hr = D3DCompileFromFile(
        shaderFilePath.c_str(),
        nullptr,
        D3D_COMPILE_STANDARD_FILE_INCLUDE,
        "PS", "ps_5_0",
        shaderFlags, 0,
        &shaderBlob,
        &errorBlob);
    if (errorBlob != nullptr && errorBlob->GetBufferPointer() != nullptr)
    {
        LOG("%s", static_cast<const char*>(errorBlob->GetBufferPointer()));
    }
    ASSERT(SUCCEEDED(hr), "Failed to compile pixel shader");

    hr = device->CreatePixelShader(
        shaderBlob->GetBufferPointer(),
        shaderBlob->GetBufferSize(),
        nullptr,
        &mPixelShader);
    ASSERT(SUCCEEDED(hr), "Failed to create pixel shader");
    SafeRelease(shaderBlob);
    SafeRelease(errorBlob);
}

void ShapeState::Terminate()
{
    mVertices.clear();
    SafeRelease(mPixelShader);
    SafeRelease(mInputLayout);
    SafeRelease(mVertexShader);
    SafeRelease(mVertexBuffer);
}

void ShapeState::Render()
{
    if (!mVertexBuffer || mVertices.empty())
    {
        return;
    }

    auto context = GraphicsSystem::Get()->GetContext();

    context->VSSetShader(mVertexShader, nullptr, 0);
    context->IASetInputLayout(mInputLayout);
    context->PSSetShader(mPixelShader, nullptr, 0);

    context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    context->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
    context->Draw(static_cast<UINT>(mVertices.size()), 0);
}

void ShapeState::RecreateVertexBuffer()
{
    if (mVertices.empty())
    {
        SafeRelease(mVertexBuffer);
        return;
    }

    auto device = GraphicsSystem::Get()->GetDevice();
    SafeRelease(mVertexBuffer);

    D3D11_BUFFER_DESC bufferDesc{};
    bufferDesc.ByteWidth = static_cast<UINT>(mVertices.size() * sizeof(Vertex));
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bufferDesc.CPUAccessFlags = 0;
    bufferDesc.MiscFlags = 0;
    bufferDesc.StructureByteStride = 0;

    D3D11_SUBRESOURCE_DATA initData{};
    initData.pSysMem = mVertices.data();

    HRESULT hr = device->CreateBuffer(&bufferDesc, &initData, &mVertexBuffer);
    ASSERT(SUCCEEDED(hr), "Failed to create vertex buffer");
}

void TriforceShapeState::CreateShape()
{
    mVertices.clear();

    float scale = 0.4f;
    float h_offset = 0.0f;
    float v_offset = 0.0f;

    //mVertices.push_back({ {h_offset + 0.0f * scale, v_offset + 1.0f * scale, 0.0f}, Colors::Gold });
    //mVertices.push_back({ {h_offset + 1.25f * scale, v_offset + 0.0f * scale, 1.0f}, Colors::Gold });
    //mVertices.push_back({ {h_offset + 0.5f * scale, v_offset + 0.0f * scale, 0.0f}, Colors::Gold });

    mVertices.push_back({ {h_offset - 0.5f * scale, v_offset + 0.0f * scale, 0.0f}, Colors::Goldenrod });
    mVertices.push_back({ {h_offset + 1.0f * scale, v_offset + 1.0f * scale, 0.0f}, Colors::Goldenrod });
    mVertices.push_back({ {h_offset + 0.0f * scale, v_offset - 1.0f * scale, 0.0f}, Colors::Goldenrod });

    mVertices.push_back({ {h_offset - 0.5f * scale, v_offset + 0.0f * scale, 0.0f}, Colors::DarkGoldenrod });
    mVertices.push_back({ {h_offset + 0.0f * scale, v_offset + 1.0f * scale, 0.0f}, Colors::DarkGoldenrod });
    mVertices.push_back({ {h_offset + 1.0f * scale, v_offset - 1.0f * scale, 0.0f}, Colors::DarkGoldenrod });
}

void TriforceShapeState::Update(float deltaTime)
{
    if (Input::InputSystem::Get()->IsKeyPressed(Input::KeyCode::SPACE))
    {
        MainApp().ChangeState("DiamondShapeState");
    }
}

void DiamondShapeState::CreateShape()
{
    mVertices.clear();

    float topY = 0.7f;
    float midY = 0.0f;
    float botY = -0.7f;
    float midX = 0.35f;
    float tipX = 0.0f;

    Color c1 = Colors::LightSkyBlue;
    Color c2 = Colors::DeepSkyBlue;
    Color c3 = Colors::CornflowerBlue;
    Color c4 = Colors::DodgerBlue;

    mVertices.push_back({ {tipX, topY, 0.0f}, c1 });
    mVertices.push_back({ {midX, midY, 0.0f}, c1 });
    mVertices.push_back({ {tipX, midY, 0.0f}, c1 });

    mVertices.push_back({ {tipX, topY, 0.0f}, c2 });
    mVertices.push_back({ {tipX, midY, 0.0f}, c2 });
    mVertices.push_back({ {-midX, midY, 0.0f}, c2 });

    mVertices.push_back({ {tipX, botY, 0.0f}, c3 });
    mVertices.push_back({ {tipX, midY, 0.0f}, c3 });
    mVertices.push_back({ {midX, midY, 0.0f}, c3 });

    mVertices.push_back({ {tipX, botY, 0.0f}, c4 });
    mVertices.push_back({ {-midX, midY, 0.0f}, c4 });
    mVertices.push_back({ {tipX, midY, 0.0f}, c4 });
}

void DiamondShapeState::Update(float deltaTime)
{
    if (Input::InputSystem::Get()->IsKeyPressed(Input::KeyCode::SPACE))
    {
        MainApp().ChangeState("HeartShapeState");
    }
}

void HeartShapeState::CreateShape()
{
    mVertices.clear();

    mVertices.push_back({ { 0.00f, -0.6f, 0.0f}, Colors::AliceBlue });
    mVertices.push_back({ {-0.35f, 0.0f, 0.0f}, Colors::AliceBlue });
    mVertices.push_back({ { 0.35f, 0.0f, 0.0f}, Colors::AliceBlue });

    mVertices.push_back({ {-0.35f, 0.0f, 0.0f}, Colors::Aqua });
    mVertices.push_back({ {-0.3f, 0.5f, 0.0f}, Colors::Aqua });
    mVertices.push_back({ { 0.0f, 0.5f, 0.0f}, Colors::Aqua });

    mVertices.push_back({ { 0.35f, 0.0f, 0.0f}, Colors::Aquamarine });
    mVertices.push_back({ { 0.0f, 0.5f, 0.0f}, Colors::Aquamarine });
    mVertices.push_back({ { 0.3f, 0.5f, 0.0f}, Colors::Aquamarine });
}

void HeartShapeState::Update(float deltaTime)
{
    if (Input::InputSystem::Get()->IsKeyPressed(Input::KeyCode::SPACE))
    {
        MainApp().ChangeState("TriforceShapeState");
    }
}