#include "AblazePch.h"
#include "VertexArray.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"
namespace Ablaze {
	std::shared_ptr<VertexArray> VertexArray::Create()
	{
		switch (Renderer::GetRendererAPI())
		{
		case RendererAPI::API::None:
			ABLAZE_CORE_ASSERTS(false, "RendererAPI::None is currently not supported"); return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLVertexArray>();
		}
		ABLAZE_CORE_ASSERTS(false, "Unknown RendererAPI!");
		return nullptr;
	}
}