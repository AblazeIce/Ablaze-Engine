#include "pch/AblazePch.h"
#include "RenderCommand.h"
#include "Ablaze/Platform/OpenGL/OpenGLRendererAPI.h"

namespace Ablaze {
	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;
	
}