#include "Viewport.h"

#include <glad/glad.h>

Viewport::Viewport(int width, int height)
	:
	m_X(0),
	m_Y(0),
	m_Width(width),
	m_Height(height) 
{ }

Viewport::Viewport(int x, int y, int width, int height)
	:
	m_X(0),
	m_Y(0),
	m_Width(width),
	m_Height(height)
{ }

void Viewport::Apply()
{
	glViewport(m_X, m_Y, m_Width, m_Height);
}
