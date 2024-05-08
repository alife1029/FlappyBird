#pragma once

namespace Engine
{
	class SoundBuffer
	{
	public:
		static SoundBuffer* Get();

		unsigned int AddSoundEffect(const char* fileName);
		bool RemoveSoundEffect(const unsigned int& buffer);

	private:
		SoundBuffer();
		~SoundBuffer();

		std::vector<unsigned int> m_SoundEffectBuffers;
	};
}
