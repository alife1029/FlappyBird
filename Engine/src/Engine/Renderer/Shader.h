#pragma once

#include "Engine/Utils/EngineException.h"

#include <string>
#include <cstdint>

#include <glm/glm.hpp>

namespace Engine
{
	class Shader
	{
	public:
		Shader(const std::string& vsFile, const std::string& fsFile);
		virtual ~Shader();
		virtual void Bind() const noexcept;
	};
}
