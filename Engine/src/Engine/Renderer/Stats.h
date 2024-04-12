#pragma once

namespace Engine
{
	class RendererStats
	{
		friend class AppManager;
		friend class BatchRenderer;
		friend class TextRenderer;
	public:
		static size_t GetBatchCount();
		static size_t GetTriangleCount();
		static size_t GetVertexCount();
		
	private:
		static void Reset();

	private:
		static size_t s_BatchCount;
		static size_t s_TriangleCount;
		static size_t s_VertexCount;
	};
}
