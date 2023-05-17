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
		
		//������window�����ĺ󻺴滭����ָ��û���ͨ����ͨ�����ڵ�ǰ�󻺴�Ľ�������ɵġ�
		glfwSwapBuffers(m_WindowHandle);
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		//glfwGetProcAddress������һ�������������һ�� c �ַ�����Ȼ�󷵻غ����ĵ�ַ
		//�Ǹ�GLADloadproc����һ��ǿ������ת��
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		ABLAZE_CORE_ASSERTS(status, "Failed to initialize Glad!")

	}
}
