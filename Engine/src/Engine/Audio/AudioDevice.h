#pragma once

#include <AL/alc.h>

namespace Engine
{
	class AudioDevice
	{
	public:
		static AudioDevice* Get();

	private:
		AudioDevice();
		~AudioDevice();

		ALCdevice* m_ALCDevice;
		ALCcontext* m_ALCContext;
	};
}

