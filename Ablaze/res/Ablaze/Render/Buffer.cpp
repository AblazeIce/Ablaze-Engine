#include "AblazePch.h"
#include "Buffer.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Ablaze {
	std::shared_ptr<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (Renderer::GetRendererAPI())
		{
		case RendererAPI::API::None:
			ABLAZE_CORE_ASSERTS(false, "RendererAPI::None is currently not supported"); return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLVertexBuffer>(vertices, size);
		}
		ABLAZE_CORE_ASSERTS(false, "Unknown RendererAPI!");
		return nullptr;
	}
	std::shared_ptr<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t size)
	{
		switch (Renderer::GetRendererAPI())
		{
		case RendererAPI::API::None:
			ABLAZE_CORE_ASSERTS(false, "RendererAPI::None is currently not supported"); return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLIndexBuffer>(indices, size);
		}
		ABLAZE_CORE_ASSERTS(false, "Unknown RendererAPI!");
		return nullptr;
	}
}