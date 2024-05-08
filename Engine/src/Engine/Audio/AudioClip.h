#pragma once

#include "Engine/Utils/EngineException.h"

#include <string>

namespace Engine
{
	class AudioClip
	{
	public:
		class BadSampleCountException : public EngineException
		{
		public:
			BadSampleCountException(int line, const char* file, const std::string& audioFile, int64_t sampleCount);
			const char* what() const noexcept override;
			const char* GetType() const noexcept;

			std::string GetAudioFile() const noexcept;
			int64_t GetSampleCount() const noexcept;

		private:
			std::string m_AudioFile;
			int64_t m_SampleCount;
		};

		class UnsupportedChannelCountException : public EngineException
		{
		public:
			UnsupportedChannelCountException(int line, const char* file, const std::string& audioFile, int64_t channelCount);
			const char* what() const noexcept override;
			const char* GetType() const noexcept;

			std::string GetAudioFile() const noexcept;
			int64_t GetChannelCount() const noexcept;

		private:
			std::string m_AudioFile;
			int64_t m_ChannelCount;
		};

		class OpenALException : public EngineException
		{
		public:
			OpenALException(int line, const char* file, const std::string& errorMessage);
			const char* what() const noexcept override;
			const char* GetType() const noexcept;

			std::string GetErrorMessage() const noexcept;
		
		private:
			std::string m_ErrorMessage;
		};

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
