#pragma once

namespace RedSnowEngine::Graphics
{
	class Texture
	{
	public:
        static void UnbindVS(uint32_t slot);

        Texture() = default;
		~Texture();

        Texture(const Texture&) = delete;
        Texture& operator=(const Texture&) = delete;

        Texture(Texture&& rhs) noexcept;
        Texture& operator=(Texture&& rhs) noexcept;

        void Initialize(const std::filesystem::path& fileName);
        void Terminate();

        void BindVS(uint32_t slot) const;
        void BindPS(uint32_t slot) const;

        void* GetRawData() const;

	private:
        ID3D11ShaderResourceView* mShaderResourceView = nullptr;
	};
}