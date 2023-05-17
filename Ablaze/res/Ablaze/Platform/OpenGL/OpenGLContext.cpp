#include "pch/AblazePch.h"
#include "OpenGLContext.h"

#include "glad/glad.h"
namespace Ablaze {
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle) 
		:m_WindowHandle(windowHandle)
	{
		ABLAZE_CORE_ASSERTS(windowHandle,"Window handle is nullptr")
	}
	void OpenGLContext::SwapBuffers()
	{
		
		//将参数window关联的后缓存画面呈现给用户。通常是通过窗口的前后缓存的交换来完成的。
		glfwSwapBuffers(m_WindowHandle);
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		//glfwGetProcAddress本身是一个函数、会接收一个 c 字符串、然后返回函数的地址
		//那个GLADloadproc就是一个强制类型转换
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		ABLAZE_CORE_ASSERTS(status, "Failed to initialize Glad!")

	}
}
