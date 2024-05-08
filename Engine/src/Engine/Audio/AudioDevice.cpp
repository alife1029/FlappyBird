#include "engine_pch.h"
#include "AudioDevice.h"

#include <AL/al.h>

namespace Engine
{
	AudioDevice* AudioDevice::Get()
	{
		static AudioDevice* device = new AudioDevice();
		return device;
	}

	AudioDevice::AudioDevice()
	{
		m_ALCDevice = alcOpenDevice(nullptr);	// nullptr = Get default device
		if (!m_ALCDevice)
		{
			// TODO: Throw custom exception
			throw "Failed to get sound device!";
		}

		m_ALCContext = alcCreateContext(m_ALCDevice, nullptr);
		if (!m_ALCContext)
		{
			// TODO: Throw custom exception
			throw "Failed to set sound context";
		}

		if (!alcMakeContextCurrent(m_ALCContext))
		{
			// TODO: Throw custom exception
			throw "Failed to make context current";
		}

		const ALCchar* name = nullptr;
		if (alcIsExtensionPresent(m_ALCDevice, "ALC_ENUMERATE_ALL_EXT"))
			name = alcGetString(m_ALCDevice, ALC_ALL_DEVICES_SPECIFIER);
		if (!name || alcGetError(m_ALCDevice) != AL_NO_ERROR)
			name = alcGetString(m_ALCDevice, ALC_DEVICE_SPECIFIER);
	}

	AudioDevice::~AudioDevice()
	{
		if (!alcMakeContextCurrent(nullptr))
		{
			// TODO: Throw custom exception
			throw "Failed to set context to nullptr";
		}

		alcDestroyContext(m_ALCContext);

		if (m_ALCContext)
		{
			// TODO: Throw custom exception
			throw "Failed to unset during close";
		}

		if (!alcCloseDevice(m_ALCDevice))
		{
			// TODO: Throw custom exception
			throw "Failed to close audio device";
		}
	}
}
