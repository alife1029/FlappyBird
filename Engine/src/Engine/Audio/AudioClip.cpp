#include "engine_pch.h"
#include "AudioClip.h"

#include <sndfile.h>
#include <cinttypes>
#include <AL/alext.h>

namespace Engine
{
	AudioClip::AudioClip(const std::string& audioFile)
	{
		ALenum err;
		SNDFILE* sndFile;
		SF_INFO sfInfo;
		short* memBuf;
		sf_count_t numFrames;
		ALsizei numBytes;

		// Open the audio file and check that it is usable
		sndFile = sf_open(audioFile.c_str(), SFM_READ, &sfInfo);
		if (!sndFile)
		{
			// TODO: Throw custom error
			fprintf(stderr, "Could not open audip in %s: %s\n", audioFile.c_str(), sf_strerror(sndFile));
		}

		if (sfInfo.frames < 1 || sfInfo.frames >(sf_count_t)(INT_MAX / sizeof(short)) / sfInfo.channels)
		{
			// TODO: Throw custom error
			fprintf(stderr, "Bad sample count in: %s (%" PRId64 ")\n", audioFile.c_str(), sfInfo.frames);
			sf_close(sndFile);
		}

		// Get the sound format
		m_Format = AL_NONE;
		if (sfInfo.channels == 1) m_Format = AL_FORMAT_MONO16;
		else if (sfInfo.channels == 2) m_Format = AL_FORMAT_STEREO16;
		else if (sfInfo.channels == 3)
		{
			if (sf_command(sndFile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
				m_Format = AL_FORMAT_BFORMAT2D_16;
		}
		else if (sfInfo.channels == 4)
		{
			if (sf_command(sndFile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
				m_Format = AL_FORMAT_BFORMAT3D_16;
		}

		if (!m_Format)
		{
			// TODO: Throw custom exception
			fprintf(stderr, "Unsupported channel count: %d\n", sfInfo.channels);
			sf_close(sndFile);
		}

		// Decode the whole audio file to the buffer
		memBuf = (short*)(malloc((size_t)(sfInfo.frames * sfInfo.channels) * sizeof(short)));

		numFrames = sf_readf_short(sndFile, memBuf, sfInfo.frames);
		if (numFrames < 1)
		{
			free(memBuf);
			sf_close(sndFile);

			// TODO: Throw custom exception
			fprintf(stderr, "Failed to read samples in %s (%" PRId64 ")\n", audioFile.c_str(), numFrames);
		}

		numBytes = (ALsizei)(numFrames * sfInfo.channels) * (ALsizei)sizeof(short);

		// Create OpenAL buffer
		alGenBuffers(1, &m_Buffer);
		alBufferData(m_Buffer, m_Format, memBuf, numBytes, sfInfo.samplerate);

		m_SampleRate = sfInfo.samplerate;

		free(memBuf);
		sf_close(sndFile);

		// Check if an error occured
		err = alGetError();
		if (err != AL_NO_ERROR)
		{
			// TODO: Throw custom error
			fprintf(stderr, "OpenAL Error: %s\n", alGetString(err));
			if (m_Buffer && alIsBuffer(m_Buffer))
				alDeleteBuffers(1, &m_Buffer);
		}
	}

	AudioClip::~AudioClip()
	{
		alDeleteBuffers(1, &m_Buffer);
	}
	
	unsigned int AudioClip::GetBuffer() const noexcept
	{
		return m_Buffer;
	}
}
