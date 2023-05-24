#include "pch/AblazePch.h"
#include "Shader.h"
#include "Renderer.h"
#include "Ablaze/Platform/OpenGL/OpenGLShader.h"
namespace Ablaze {
	Shader* Shader::Create(const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		switch (Renderer::GetRendererAPI())
		{
		case RendererAPI::API::None:
			ABLAZE_CORE_ASSERTS(false, "RendererAPI::None is currently not supported"); return nullptr;
		case RendererAPI::API::OpenGL:
			return new OpenGLShader(vertexSrc,fragmentSrc);
		}
		ABLAZE_CORE_ASSERTS(false, "Unknown RendererAPI!");
		return nullptr;
	}
}