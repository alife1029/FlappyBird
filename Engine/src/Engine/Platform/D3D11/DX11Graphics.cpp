#include "engine_pch.h"
#include "DX11Graphics.h"
#include "Engine/App/Window.h"

#define GFX_THROW_FAILED(hrcall) if (FAILED(hr = (hrcall))) throw DX11Graphics::Exception(__LINE__, __FILE__, hr)
#define GFX_THROW_DEVICE_REMOVED(hr) throw DX11Graphics::DeviceRemovedException(__LINE__, __FILE__, hr)

namespace Engine
{
	DX11Graphics::DX11Graphics(Window* targetWindow) : Graphics(targetWindow)
	{
		// TODO: Think about multithreaded rendering

		// Describe the swap chain
		DXGI_SWAP_CHAIN_DESC sd = {};
		sd.BufferDesc.Width = targetWindow->GetWidth();
		sd.BufferDesc.Height = targetWindow->GetHeight();
		sd.BufferDesc.RefreshRate.Numerator = 60; // TODO: Make this mutable
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		sd.SampleDesc.Count = 1; // TODO: Implement MSAA
		sd.SampleDesc.Quality = 0; // Count - 1
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.BufferCount = 2;
		sd.OutputWindow = targetWindow->GetHWND();
		sd.Windowed = true; // TODO: Implement fullscreen
		sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		sd.Flags = NULL;
		
		// Device flags
		UINT deviceFlags = D3D11_CREATE_DEVICE_SINGLETHREADED;
#ifndef NDEBUG
		deviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		// Create HRESULT for checking errors
		HRESULT hr;
		
		// Create device and swap chain
		GFX_THROW_FAILED(D3D11CreateDeviceAndSwapChain(
			nullptr,						// Default adapter
			D3D_DRIVER_TYPE_HARDWARE,		// Hardware driver
			nullptr,						// No software device
			deviceFlags,					// Flags
			NULL, NULL,						// Default feature level array
			D3D11_SDK_VERSION,				// SDK Version
			&sd,
			&m_SwapChain,
			&m_Device,
			nullptr,
			&m_Context
		));

		if (FAILED(hr))
			throw InitializationError(__LINE__, __FILE__, "Failed to create D3D11 Device and Swap Chain!");

		// Get the address of the back buffer
		ID3D11Texture2D* backBuffer;
		GFX_THROW_FAILED(m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer));

		// Use back buffer to create render target
		GFX_THROW_FAILED(m_Device->CreateRenderTargetView(backBuffer, NULL, &m_RenderTargetView));
		m_Context->OMSetRenderTargets(1, &m_RenderTargetView, NULL); // TODO: Set depth-stencil buffer

		// Release back buffer COM pointer
		backBuffer->Release();

		// Set renderer api
		m_RendererAPI = Api::D3D11;
	}

	DX11Graphics::~DX11Graphics()
	{
		m_RenderTargetView->Release();
		m_SwapChain->Release();
		m_Context->Release();
		m_Device->Release();
	}

	void DX11Graphics::EndFrame()
	{
		HRESULT hr;
		
		if (FAILED(hr = m_SwapChain->Present(1u, 0u)))
		{
			if (hr == DXGI_ERROR_DEVICE_REMOVED) GFX_THROW_DEVICE_REMOVED(hr);
			else GFX_THROW_FAILED(hr);
		}
	}

	void DX11Graphics::ClearBuffer(float red, float green, float blue, float alpha) noexcept
	{
		const FLOAT color[4] = { red, green, blue, alpha };
		m_Context->ClearRenderTargetView(m_RenderTargetView, color);
	}

	void DX11Graphics::DrawIndexed(unsigned int count)
	{
		m_Context->DrawIndexed(count, 0u, 0u);
	}

	ID3D11Device* DX11Graphics::GetDevice() const noexcept
	{
		return m_Device;
	}

	ID3D11DeviceContext* DX11Graphics::GetContext() const noexcept
	{
		return m_Context;
	}

#pragma region Exceptions

	const char* DX11Graphics::Exception::GetType() const noexcept
	{
		return "Engine D3D11 Graphics Exception";
	}

	const char* DX11Graphics::DeviceRemovedException::GetType() const noexcept
	{
		return "Engine D3D11 Graphics Device Removed Exception";
	}

#pragma endregion
}
