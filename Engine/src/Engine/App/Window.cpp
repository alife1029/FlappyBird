#include "engine_pch.h"

#include "Window.h"
#include "Engine/App/AppManager.h"

#define	ENGINE_WND_EXCEPT(hr) Engine::Window::HrException(__LINE__, __FILE__, hr)
#define ENGINE_WND_LASTEXCEPT() Engine::Window::HrException(__LINE__, __FILE__, GetLastError())

namespace Engine
{
#pragma region Win32WindowClass
	class Win32WindowClass
	{
	public:
		static LPCSTR GetName() noexcept
		{
			return s_WndClassName;
		}
		static HINSTANCE GetInstance() noexcept
		{
			return s_WndClass.m_hInst;
		}

	private:
		Win32WindowClass() noexcept
			:
			m_hInst(GetModuleHandle(nullptr))
		{
			WNDCLASSEX wc = { 0 };
			wc.cbSize = sizeof(wc);
			wc.style = CS_OWNDC;
			wc.lpfnWndProc = Window::HandleMsgSetup;
			wc.cbClsExtra = 0;
			wc.cbWndExtra = 0;
			wc.hInstance = GetInstance();
			wc.hIcon = LoadIcon(GetInstance(), MAKEINTRESOURCE(101));
			wc.hIconSm = LoadIcon(GetInstance(), MAKEINTRESOURCE(IDI_APPLICATION));
			wc.hCursor = nullptr;
			wc.hbrBackground = nullptr;
			wc.lpszMenuName = nullptr;
			wc.lpszClassName = GetName();

			RegisterClassEx(&wc);
		}

		~Win32WindowClass()
		{
			UnregisterClass(GetName(), GetInstance());
		}

	private:
		static constexpr LPCSTR s_WndClassName = "__AliEfeWindowClass__";
		static Win32WindowClass s_WndClass;
		HINSTANCE m_hInst;
	};

	Win32WindowClass Win32WindowClass::s_WndClass = Win32WindowClass();
#pragma endregion

#pragma region Window
	Window::Window(int width, int height, const std::string& title, bool fullScreen)
		:
		m_Width(width), m_Height(height), m_title(title), m_IsFullscreen(fullScreen), m_Graphics(nullptr), m_HasFocus(false), m_IsShown(false)
	{
		constexpr DWORD windowStyle = WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SIZEBOX | WS_SYSMENU | CS_OWNDC;

		// Calculate window size based on desired client region size
		RECT wr;
		wr.left = 100;
		wr.right = width + wr.left;
		wr.top = 100;
		wr.bottom = height + wr.top;

		if (AdjustWindowRect(&wr, windowStyle, FALSE) == 0)
		{
			throw ENGINE_WND_LASTEXCEPT();
		}

		// Window creation
		hWnd = CreateWindow(
			Win32WindowClass::GetName(),
			title.c_str(),
			windowStyle,
			CW_USEDEFAULT, CW_USEDEFAULT,
			width, height,
			nullptr, nullptr,
			Win32WindowClass::GetInstance(),
			this
		);

		if (hWnd == nullptr)
		{
			throw ENGINE_WND_LASTEXCEPT();
		}
	}

	Window::~Window()
	{
		DestroyWindow(hWnd);
	}

	void Window::SetWidth(int width)
	{
		// TODO: Implement this
	}
	void Window::SetHeight(int height)
	{
		// TODO: Implement this
	}
	void Window::SetTitle(const std::string& title)
	{
		if (SetWindowText(hWnd, title.c_str()) == 0)
		{
			throw ENGINE_WND_LASTEXCEPT();
		}
	}
	void Window::SetFullscreenState(bool fullScreen)
	{
		// TODO: Implement this
	}
	void Window::Show()
	{
		ShowWindow(hWnd, SW_SHOWDEFAULT);
		m_IsShown = true;
	}
	void Window::Hide() 
	{
		if (ShowWindow(hWnd, SW_HIDE) == 0) 
		{
			throw ENGINE_WND_LASTEXCEPT();
		}
		m_IsShown = false;
	}
	int Window::GetWidth() const noexcept
	{
		return m_Width;
	}
	int Window::GetHeight() const noexcept
	{
		return m_Height;
	}
	std::string Window::GetTitle() const noexcept
	{
		return m_title;
	}
	OpenGLGraphics* Window::GetGfx() const noexcept
	{
		return m_Graphics;
	}
	HWND Window::GetHWND() const noexcept
	{
		return hWnd;
	}

	void Window::ProcessEvents()
	{
		m_Keyboard.Flush();

		MSG msg;

		while (PeekMessage(&msg, hWnd, NULL, NULL, PM_REMOVE) > 0)
		{
			if (msg.message == WM_QUIT)
			{
				AppManager::QuitApplication(static_cast<int>(msg.wParam));
			}

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	void Window::CreateGraphicsContext()
	{
		m_Graphics = new OpenGLGraphics(this);
	}

	LRESULT Window::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
	{
		if (msg == WM_NCCREATE)
		{
			const CREATESTRUCT* const pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
			Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);
			SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
			SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(Window::HandleMsgThunk));
			return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
		}

		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	LRESULT Window::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
	{
		Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
		return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
	}

	LRESULT Window::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
	{
		switch (msg)
		{
		case WM_CLOSE:	// Window closed
			m_IsShown = false;
			AppManager::QuitApplication(0);
			break;

		case WM_SETFOCUS:	// Focus gained
			m_HasFocus = true;
			break;

		case WM_KILLFOCUS:	// Focus lost
			m_HasFocus = false;
			break;

		case WM_SIZE:	// Window resize
			RECT wndRect;
			GetClientRect(hWnd, &wndRect);
			m_Width = wndRect.right - wndRect.left;
			m_Height = wndRect.bottom - wndRect.top;
			break;


		/************* KEYBOARD MESSAGES *************/
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
			if (!(lParam & 0x40000000) || m_Keyboard.IsAutorepeatEnabled())
				m_Keyboard.OnKeyPress(static_cast<unsigned char>(wParam));
			break;

		case WM_KEYUP:
		case WM_SYSKEYUP:
			m_Keyboard.OnKeyRelease(static_cast<unsigned char>(wParam));
			break;

		case WM_CHAR:
			m_Keyboard.OnChar(static_cast<char>(wParam));
			break;
		/*********** END KEYBOARD MESSAGES ***********/

		default:
			break;
		}

		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
#pragma endregion

#pragma region Window Exceptions

	std::string Window::Exception::TranslateErrorCode(HRESULT hr) noexcept
	{
		char* pMsgBuf = nullptr;
		DWORD msgLen = FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			nullptr, hr, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), reinterpret_cast<LPSTR>(&pMsgBuf), 0, nullptr
		);

		if (msgLen == 0) return "Undefined Error Code";

		std::string errorString = pMsgBuf;
		LocalFree(pMsgBuf);
		return errorString;
	}

	Window::HrException::HrException(int line, const char* file, HRESULT hr) noexcept
		:
		Exception(line, file), m_Hr(hr)
	{
	}

	const char* Window::HrException::what() const noexcept
	{
		std::ostringstream oss;
		oss << GetType() << std::endl
			<< "[Error Code] " << GetErrorCode() << std::endl
			<< "[Description] " << GetErrorDescription() << std::endl
			<< GetOriginString();
		m_WhatBuffer = oss.str();
		return m_WhatBuffer.c_str();
 	}

	const char* Window::HrException::GetType() const noexcept
	{
		return "Win32 Window Exception";
	}

	HRESULT Window::HrException::GetErrorCode() const noexcept
	{
		return m_Hr;
	}

	std::string Window::HrException::GetErrorDescription() const noexcept
	{
		return TranslateErrorCode(m_Hr);
	}

#pragma endregion
}
