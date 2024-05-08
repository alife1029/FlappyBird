#include "engine_pch.h"
#include "SoundBuffer.h"

#include <sndfile.h>
#include <cinttypes>
#include <AL/alext.h>

namespace Engine
{
	SoundBuffer* SoundBuffer::Get()
	{
		static SoundBuffer* buffer = new SoundBuffer();
		return buffer;
	}

	unsigned int SoundBuffer::AddSoundEffect(const char* fileName)
	{
		ALenum err, format;
		ALuint buffer;
		SNDFILE* sndFile;
		SF_INFO sfInfo;
		short* memBuf;
		sf_count_t numFrames;
		ALsizei numBytes;

		// Open the audio file and check that it is usable
		sndFile = sf_open(fileName, SFM_READ, &sfInfo);
		if (!sndFile)
		{
			// TODO: Throw custom error
			fprintf(stderr, "Could not open audip in %s: %s\n", fileName, sf_strerror(sndFile));
			return 0;
		}

		if (sfInfo.frames < 1 || sfInfo.frames >(sf_count_t)(INT_MAX / sizeof(short)) / sfInfo.channels)
		{
			// TODO: Throw custom error
			fprintf(stderr, "Bad sample count in: %s (%" PRId64 ")\n", fileName, sfInfo.frames);
			sf_close(sndFile);
			return 0;
		}

		// Get the sound format
		format = AL_NONE;
		if (sfInfo.channels == 1) format = AL_FORMAT_MONO16;
		else if (sfInfo.channels == 2) format = AL_FORMAT_STEREO16;
		else if (sfInfo.channels == 3)
		{
			if (sf_command(sndFile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
				format = AL_FORMAT_BFORMAT2D_16;
		}
		else if (sfInfo.channels == 4)
		{
			if (sf_command(sndFile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
				format = AL_FORMAT_BFORMAT3D_16;
		}

		if (!format)
		{
			// TODO: Throw custom exception
			fprintf(stderr, "Unsupported channel count: %d\n", sfInfo.channels);
			sf_close(sndFile);
			return 0;
		}

		// Decode the whole audio file to the buffer
		memBuf = static_cast<short*>(malloc((size_t)(sfInfo.frames * sfInfo.channels) * sizeof(short)));

		numFrames = sf_readf_short(sndFile, memBuf, sfInfo.frames);
		if (numFrames < 1)
		{
			free(memBuf);
			sf_close(sndFile);

			// TODO: Throw custom exception
			fprintf(stderr, "Failed to read samples in %s (%" PRId64 ")\n", fileName, numFrames);

			return 0;
		}

		numBytes = (ALsizei)(numFrames * sfInfo.channels) * (ALsizei)sizeof(short);

		// Create OpenAL buffer
		buffer = 0;
		alGenBuffers(1, &buffer);
		alBufferData(buffer, format, memBuf, numBytes, sfInfo.samplerate);

		free(memBuf);
		sf_close(sndFile);

		// Check if an error occured
		err = alGetError();
		if (err != AL_NO_ERROR)
		{
			// TODO: Throw custom error
			fprintf(stderr, "OpenAL Error: %s\n", alGetString(err));
			if (buffer && alIsBuffer(buffer))
				alDeleteBuffers(1, &buffer);
			return 0;
		}

		m_SoundEffectBuffers.push_back(buffer);

		return buffer;
	}
	
	bool SoundBuffer::RemoveSoundEffect(const unsigned int& buffer)
	{
		auto it = m_SoundEffectBuffers.begin();

		while (it != m_SoundEffectBuffers.end())
		{
			if (*it == buffer)
			{
				alDeleteBuffers(1, &*it);

				it = m_SoundEffectBuffers.erase(it);

				return true;
			}
			else
			{
				++it;
			}
		}

		return false;
	}
	
	SoundBuffer::SoundBuffer()
	{
		m_SoundEffectBuffers.clear();
	}
	
	SoundBuffer::~SoundBuffer()
	{
		alDeleteBuffers(static_cast<ALsizei>(m_SoundEffectBuffers.size()), m_SoundEffectBuffers.data());
		m_SoundEffectBuffers.clear();
	}
}
