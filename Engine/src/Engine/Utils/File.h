#pragma once

#include <string>
#include <cstdint>
#include "EngineException.h"

namespace Engine
{
	class File
	{
	public:
		class CannotOpenFileException : public EngineException
		{
		public:
			CannotOpenFileException(int line, const char* file, const std::string& path);
			const char* what() const noexcept override;
			virtual const char* GetType() const noexcept;

			std::string GetFilePath() const noexcept;

		private:
			std::string m_FilePath;
		};

		class CannotWriteFileException : public EngineException
		{
		public:
			CannotWriteFileException(int line, const char* file, const std::string& path);
			const char* what() const noexcept override;
			virtual const char* GetType() const noexcept;

			std::string GetFilePath() const noexcept;

		private:
			std::string m_FilePath;
		};

	public:
		static std::string Read(const std::string& path);
		static uint8_t* ReadBinaryFile(const std::string & file);
		static void WriteBinaryFile(const std::string& path, const void* data, size_t dataLength);
	};
}
