#pragma once

namespace Engine
{
	class Window;

	class Graphics
	{
	public:
		enum class Api { D3D11, OPENGL, NONE };
	public:
		Graphics(Window* targetWindow);
		Graphics(const Graphics&) = delete;
		virtual Graphics& operator=(const Graphics&) = delete;
		virtual ~Graphics();

		virtual void EndFrame();
		virtual void ClearBuffer(float red, float green, float blue, float alpha) noexcept;
		virtual void DrawIndexed(unsigned int count);

		Api GetAPI();

	protected:
		Window* m_TargetWindow;
		Api m_RendererAPI;
	};
}
