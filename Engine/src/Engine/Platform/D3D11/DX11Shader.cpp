#include "engine_pch.h"
#include "DX11Graphics.h"
#include "DX11Shader.h"
#include "Engine/App/AppManager.h"
#include "Engine/Utils/StringUtils.h"

namespace Engine
{
	DX11Shader::DX11Shader(const std::string& vsFile, const std::string& psFile) : Shader(vsFile, psFile)
	{
		m_Gfx = (DX11Graphics*)AppManager::GetRunningApplication()->GetWindow()->GetGfx();

		// TODO: Graphics error handling

		// Vertex shader
		D3DReadFileToBlob(ToWideString(vsFile).c_str(), &m_ByteCodeBlob);
		m_Gfx->GetDevice()->CreateVertexShader(
			m_ByteCodeBlob->GetBufferPointer(),
			m_ByteCodeBlob->GetBufferSize(),
			nullptr,
			&m_VertexShader
		);

		// Pixel shader
		ID3DBlob* psBlob;
		D3DReadFileToBlob(ToWideString(psFile).c_str(), &psBlob);
		m_Gfx->GetDevice()->CreatePixelShader(
			psBlob->GetBufferPointer(),
			psBlob->GetBufferSize(),
			nullptr,
			&m_PixelShader
		);
		psBlob->Release();
	}

	DX11Shader::~DX11Shader()
	{
		m_ByteCodeBlob->Release();
		m_VertexShader->Release();
		m_PixelShader->Release();
	}
	
	void DX11Shader::Bind() const noexcept
	{
		m_Gfx->GetContext()->VSSetShader(m_VertexShader, nullptr, 0);
		m_Gfx->GetContext()->PSSetShader(m_PixelShader, nullptr, 0);
	}

	ID3DBlob* DX11Shader::GetVSByteCode() const noexcept
	{
		return m_ByteCodeBlob;
	}
}
