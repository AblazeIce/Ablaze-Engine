#include "pch/AblazePch.h"
#include "Buffer.h"
#include "Renderer.h"
#include "Ablaze/Platform/OpenGL/OpenGLBuffer.h"

namespace Ablaze {
	VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (Renderer::GetRendererAPI())
		{
		case RendererAPI::None:
			ABLAZE_CORE_ASSERTS(false, "RendererAPI::None is currently not supported"); return nullptr;
		case RendererAPI::OpenGL:
			return new OpenGLVertexBuffer(vertices, size);
		}
		ABLAZE_CORE_ASSERTS(false, "Unknown RendererAPI!");
		return nullptr;
	}
	IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t size)
	{
		switch (Renderer::GetRendererAPI())
		{
		case RendererAPI::None:
			ABLAZE_CORE_ASSERTS(false, "RendererAPI::None is currently not supported"); return nullptr;
		case RendererAPI::OpenGL:
			return new OpenGLIndexBuffer(indices, size);
		}
		ABLAZE_CORE_ASSERTS(false, "Unknown RendererAPI!");
		return nullptr;
	}
}