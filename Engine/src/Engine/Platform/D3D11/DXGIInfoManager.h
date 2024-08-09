#pragma once

#include <vector>
#include <string>
#include <dxgi.h>

struct IDXGIInfoQueue;

namespace Engine
{
	class DXGIInfoManager
	{
	public:
		DXGIInfoManager();
		~DXGIInfoManager();
		DXGIInfoManager(const DXGIInfoManager&) = delete;
		DXGIInfoManager& operator=(const DXGIInfoManager&) = delete;
		void Set() noexcept;
		std::vector<std::string> GetMessages() const;
	private:
		unsigned long long m_Next = 0u;
		IDXGIInfoQueue* m_DxgiInfoQueue = nullptr;
	};
}
