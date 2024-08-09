#include "engine_pch.h"
#include "DXGIInfoManager.h"
#include "Engine/Utils/EngineException.h"
#include <dxgidebug.h>
#include <memory>

#define GFX_THROW_NOINFO(hrcall) if (FAILED(hr=(hrcall))) throw HrException(__LINE__, __FILE__, hr)

namespace Engine
{
	DXGIInfoManager::DXGIInfoManager()
	{
		// Define function signature of DXGIGetDebugInterface
		typedef HRESULT(WINAPI* DXGIGetDebugInterface)(REFIID, void**);

		// Load the DLL that contains the function DXGIGetDebugInterface
		const HMODULE hModDxgiDebug = LoadLibraryEx("dxgidebug.dll", nullptr, LOAD_LIBRARY_SEARCH_DEFAULT_DIRS);
		if (hModDxgiDebug == nullptr) throw HrException(__LINE__, __FILE__, GetLastError());

		// Get address of DXGIGetDebugInterface in DLL
		const auto DxgiGetDebugInterface = reinterpret_cast<DXGIGetDebugInterface>(
			reinterpret_cast<void*>(GetProcAddress(hModDxgiDebug, "DXGIGetDebugInterface"))
		);
		if (DxgiGetDebugInterface == nullptr) throw HrException(__LINE__, __FILE__, GetLastError());

		HRESULT hr;
		GFX_THROW_NOINFO(DxgiGetDebugInterface(__uuidof(IDXGIInfoQueue), reinterpret_cast<void**>(&m_DxgiInfoQueue)));
	}

	DXGIInfoManager::~DXGIInfoManager()
	{
		if (m_DxgiInfoQueue != nullptr) m_DxgiInfoQueue->Release();
	}

	void DXGIInfoManager::Set() noexcept
	{
		// Set the index (next) so that the next all to GetMessages() will only get errors generated after this call
		m_Next = m_DxgiInfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);
	}

	std::vector<std::string> DXGIInfoManager::GetMessages() const
	{
		std::vector<std::string> messages;
		const auto end = m_DxgiInfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);
		for (auto i = m_Next; i < end; i++)
		{
			HRESULT hr;
			SIZE_T messageLength;

			// Get the size of message in bytes
			GFX_THROW_NOINFO(m_DxgiInfoQueue->GetMessageA(DXGI_DEBUG_ALL, i, nullptr, &messageLength));

			// Allocate memory for the message
			auto bytes = std::make_unique<byte[]>(messageLength);
			auto message = reinterpret_cast<DXGI_INFO_QUEUE_MESSAGE*>(bytes.get());

			// Get the message and push its description into the vector
			GFX_THROW_NOINFO(m_DxgiInfoQueue->GetMessageA(DXGI_DEBUG_ALL, i, message, &messageLength));
			messages.emplace_back(message->pDescription);
		}

		return messages;
	}
}
