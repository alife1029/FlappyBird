#include "engine_pch.h"
#include "AudioSource.h"

#include <AL/al.h>

namespace Engine
{
	AudioSource::AudioSource()
	{
		alGenSources(1, &m_Source);
		alSourcef(m_Source, AL_PITCH, m_Pitch);
		alSourcef(m_Source, AL_GAIN, m_Gain);
		alSource3f(m_Source, AL_POSITION, m_Position.x, m_Position.y, m_Position.z);
		alSource3f(m_Source, AL_VELOCITY, m_Velocity.x, m_Velocity.y, m_Velocity.z);
		alSourcei(m_Source, AL_LOOPING, m_Loop);
		alSourcei(m_Source, AL_BUFFER, m_Buffer);
	}

	AudioSource::~AudioSource()
	{
		alDeleteSources(1, &m_Source);
	}

	void AudioSource::Play(const unsigned int bufferToPlay)
	{
		if (bufferToPlay != m_Buffer)
		{
			m_Buffer = bufferToPlay;
			alSourcei(m_Source, AL_BUFFER, m_Buffer);
		}

		alSourcePlay(m_Source);
	}
}
