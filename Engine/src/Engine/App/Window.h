#pragma once

#include "Engine/Utils/EngineException.h"
#include <Windows.h>

namespace Engine
{
	class Window
	{
		friend class Win32WindowClass;
	public:
		class Exception : public EngineException
		{
			using EngineException::EngineException;
		public:
			static std::string TranslateErrorCode(HRESULT hr) noexcept;
		};

		class HrException : public Exception
		{
		public:
			HrException(int line, const char* file, HRESULT hr) noexcept;
			virtual const char* what() const noexcept override;
			virtual const char* GetType() const noexcept override;
			HRESULT	GetErrorCode() const noexcept;
			std::string GetErrorDescription() const noexcept;
		private:
			HRESULT m_Hr;
		};

	public:
		Window(int width, int height, const std::string& title, bool fullScreen);
		~Window();

		void SetWidth(int width);
		void SetHeight(int height);
		void SetTitle(const std::string& title);
		void SetFullscreenState(bool fullScreen);
		void Show();
		void Hide();

		HWND GetHWND();

		void ProcessEvents();
		void CreateGraphicsContext();

	private:
		int m_Width, m_Height;
		std::string m_title;
		bool	m_IsFullscreen, 
				m_IsShown,
				m_HasFocus;
		HWND hWnd;

	private:
		static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
		static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
		LRESULT CALLBACK HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	};
}
