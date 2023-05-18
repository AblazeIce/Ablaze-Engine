#pragma once
namespace Ablaze {
	enum class RendererAPI
	{
		None = 0, OpenGL = 1
	};
	class Renderer {
	public:
		inline static RendererAPI GetRendererAPI() { return m_RendererAPI; }
	private:
		static RendererAPI m_RendererAPI;
	};
}
