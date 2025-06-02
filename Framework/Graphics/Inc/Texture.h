#pragma once

namespace RedSnowEngine::Graphics
{
	class Texture
	{
	public:
        static void UnbindVS(uint32_t slot);

        Texture() = default;
		virtual ~Texture();

        Texture(const Texture&) = delete;
        Texture& operator=(const Texture&) = delete;

        Texture(Texture&& rhs) noexcept;
        Texture& operator=(Texture&& rhs) noexcept;

        virtual void Initialize(const std::filesystem::path& fileName);
        virtual void Terminate();

        void BindVS(uint32_t slot) const;
        void BindPS(uint32_t slot) const;

        void* GetRawData() const;

	protected:
        ID3D11ShaderResourceView* mShaderResourceView = nullptr;
	};
}