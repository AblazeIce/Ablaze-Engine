#include "pch/AblazePch.h"
#include "VertexArray.h"
#include "Renderer.h"
#include "Ablaze/Platform/OpenGL/OpenGLVertexArray.h"
namespace Ablaze {
	VertexArray* VertexArray::Create()
	{
		switch (Renderer::GetRendererAPI())
		{
		case RendererAPI::API::None:
			ABLAZE_CORE_ASSERTS(false, "RendererAPI::None is currently not supported"); return nullptr;
		case RendererAPI::API::OpenGL:
			return new OpenGLVertexArray();
		}
		ABLAZE_CORE_ASSERTS(false, "Unknown RendererAPI!");
		return nullptr;
	}
}