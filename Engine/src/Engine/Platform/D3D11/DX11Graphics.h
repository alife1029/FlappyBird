#pragma once

#include "Engine/Renderer/Graphics.h"
#include "Engine/Utils/EngineException.h"
#include "DXGIInfoManager.h"

#include <Windows.h>
#include <d3d11.h>

namespace Engine
{
	class Window;

	class DX11Graphics : public Graphics
	{
	public:
		class Exception : public HrException
		{
		public:
			Exception(int line, const char* file, HRESULT hr, std::vector<std::string> infoList = {});
			virtual const char* what() const noexcept override;
			virtual const char* GetType() const noexcept override;
			std::string GetErrorInfo() const noexcept;
		private:
			std::string m_Info;
		};

		class DeviceRemovedException : public Exception
		{
			using Exception::Exception;
		public:
			virtual const char* GetType() const noexcept override;
		private:
			std::string reason;
		};

	public:
		DX11Graphics(Window* targetWindow);
		DX11Graphics(const DX11Graphics&) = delete;
		DX11Graphics& operator=(const DX11Graphics&) = delete;
		~DX11Graphics();

		void EndFrame() override;
		void ClearBuffer(float red, float green, float blue, float alpha) noexcept override;
		void DrawIndexed(unsigned int count) override;

		ID3D11Device* GetDevice() const noexcept;
		ID3D11DeviceContext* GetContext() const noexcept;

	private:
		ID3D11Device* m_Device;
		ID3D11DeviceContext* m_Context;
		IDXGISwapChain* m_SwapChain;
		ID3D11RenderTargetView* m_RenderTargetView;

#ifndef NDEBUG
		DXGIInfoManager m_InfoManager;
#endif
	};
}
