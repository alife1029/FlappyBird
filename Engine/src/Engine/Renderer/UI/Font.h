#pragma once

#include <map>
#include <cstdint>
#include <string>
#include <glm/vec2.hpp>

#include "../Texture2D.h"

namespace Engine
{
	class Font
	{
	public:
		struct Character
		{
			Texture2D*	Texture;
			glm::ivec2	GlyphSize;
			glm::ivec2	Bearing;	// Offset from the baseline to left-top of the glyph
			uint32_t	Advance;	// Horizontal offset to advance	to next glyph
		};

	public:
		Font(const std::string& path, unsigned int fontSize = 64, Texture2D::Filter filterMode = Texture2D::Filter::Bilinear);
		~Font();

		Character GetChar(char c) noexcept;
		unsigned int GetFontSize() const noexcept;

	private:
		std::map<char, Character> m_Characters;
		unsigned int m_FontSize;
	};
}
