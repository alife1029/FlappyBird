#include "engine_pch.h"
#include "Font.h"
#include "Engine/Utils/EngineException.h"
#include "Engine/App/AppManager.h"
#include "Engine/Platform/GL/GLTexture2D.h"

namespace Engine
{
	static FT_Library ft;
	static bool ftInitialized = false;
	static unsigned int fontInstanceCount = 0;

	Font::Font(const std::string& path, unsigned int fontSize, Texture2D::Filter filterMode)
	{
		if (!ftInitialized)
		{
			if (FT_Init_FreeType(&ft))
			{
				throw InitializationError(__LINE__, __FILE__, "Failed to initialize FreeType library!");
			}

			ftInitialized = true;
		}

		Graphics* gfx = AppManager::GetRunningApplication()->GetWindow()->GetGfx();

		FT_Face fontFace;
		if (FT_New_Face(ft, path.c_str(), 0, &fontFace))
			throw ResourceNotFoundException(__LINE__, __FILE__, path);

		FT_Set_Pixel_Sizes(fontFace, 0, fontSize);
		
		if (gfx->GetAPI() == Graphics::Api::OPENGL) glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		m_FontSize = 0u;
		
		for (unsigned char c = 0; c < 255; c++)
		{
			if (FT_Load_Char(fontFace, c, FT_LOAD_RENDER))
				continue;

			// Generate texture
			Texture2D* texture = gfx->CreateTexture2D(
				(unsigned char*)fontFace->glyph->bitmap.buffer,
				fontFace->glyph->bitmap.width, fontFace->glyph->bitmap.rows,
				1, 1, filterMode);

			// Store character
			Character _c = {
				texture,
				{ fontFace->glyph->bitmap.width, fontFace->glyph->bitmap.rows },
				{ fontFace->glyph->bitmap_left, fontFace->glyph->bitmap_top },
				static_cast<uint32_t>(fontFace->glyph->advance.x)
			};

			m_Characters.insert(std::pair<char, Character>(c, _c));
			if (static_cast<unsigned int>(_c.GlyphSize.y) > m_FontSize)
				m_FontSize = _c.GlyphSize.y;
		}

		// Cleanup
		FT_Done_Face(fontFace);

		++fontInstanceCount;
	}

	Font::~Font()
	{
		// Delete character textures
		for (unsigned char i = 0; i < 255; i++)
		{
			if (m_Characters.count(i))
			{
				Character c = m_Characters[i];
				delete c.Texture;
			}
		}

		if (--fontInstanceCount == 0)
		{
			FT_Done_FreeType(ft);
			ftInitialized = false;
		}
	}

	Font::Character Font::GetChar(char c) noexcept
	{
		return m_Characters[c];
	}

	unsigned int Font::GetFontSize() const noexcept
	{
		return m_FontSize;
	}
}
