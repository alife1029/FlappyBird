#pragma once

#include <string>

namespace Engine
{
	class AudioClip
	{
	public:
		AudioClip(const std::string& audioFile);
		~AudioClip();

		unsigned int GetBuffer() const noexcept;

	private:
		unsigned int m_Buffer;
		int m_SampleRate;
		int m_Format;
	};
}
