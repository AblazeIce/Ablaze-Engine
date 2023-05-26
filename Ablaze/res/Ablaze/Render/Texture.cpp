#include "pch/AblazePch.h"
#include "Texture.h"
#include "Ablaze/Render/Renderer.h"
#include "Ablaze/Platform/OpenGL/OpenGLTexture.h"
namespace Ablaze {
	Texture2D* Texture2D::Create(const std::string& path)
	{
		switch (Renderer::GetRendererAPI())
		{
		case RendererAPI::API::None:
			ABLAZE_CORE_ASSERTS(false, "RendererAPI::None is currently not supported"); return nullptr;
		case RendererAPI::API::OpenGL:
			return  new OpenGLTexture2D(path);
		}
		ABLAZE_CORE_ASSERTS(false, "Unknown RendererAPI!");
		return nullptr;
	}
}