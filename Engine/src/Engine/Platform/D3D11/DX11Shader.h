#pragma once

#include "Engine/Renderer/Shader.h"

namespace Engine
{
	class DX11Graphics;
	class DX11Shader : public Shader
	{
	public:
		DX11Shader(const std::string& vsFile, const std::string& psFile);
		~DX11Shader();

		void Bind() const noexcept override;

		ID3DBlob* GetVSByteCode() const noexcept;

	private:
		DX11Graphics* m_Gfx;
		ID3D11PixelShader* m_PixelShader;
		ID3D11VertexShader* m_VertexShader;
		ID3DBlob* m_ByteCodeBlob;
	};
}
