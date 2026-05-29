#pragma once

namespace RedSnowEngine::Graphics
{
    class UISprite;

    class UISpriteRender final
    {
    public:
        static void StaticInitialize();
        static void StaticTerminate();
        static UISpriteRender* Get();

        UISpriteRender() = default;
        ~UISpriteRender();

        void Initialize();
        void Terminate();

        void BeginRender();
        void EndRender();

        void Render(const UISprite& uiSprite);

    private:
        DirectX::CommonStates* mCommonStates = nullptr;
        DirectX::SpriteBatch* mSpriteBatch = nullptr;
    };
}