#pragma once
#include "Ablaze/Render/GraphicsContext.h"
#include "GLFW/glfw3.h"

namespace Ablaze {
	class OpenGLContext :public GraphicsContext {
	public:
		OpenGLContext(GLFWwindow* windowHandle);
		virtual void SwapBuffers() override;
		virtual void Init() override;
	private:
		GLFWwindow* m_WindowHandle;
	};
}