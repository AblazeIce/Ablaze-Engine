#include "AblazePch.h"
#include "RenderCommand.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Ablaze {
	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;
	
}