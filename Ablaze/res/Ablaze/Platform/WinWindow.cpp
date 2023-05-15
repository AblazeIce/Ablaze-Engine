#include "pch/AblazePch.h"
#include "WinWindow.h"

#include "Ablaze/Event/ApplicationEvent.h"
#include "Ablaze/Event/KeyEvent.h"
#include "Ablaze/Event/MouseEvent.h"
#include "glad/glad.h"

namespace Ablaze {
	static bool s_GLFWInitialized = false;
	static void GLFWErrorCallback(int error, const char* description)
	{
		ABLAZE_CORE_ERROR("GLFW Error ({0}):{1}", error, description);
	}
	Window* Window::Create(const WindowProps& props) {
		return new WinWindow(props);
	}

	WinWindow::WinWindow(const WindowProps& props)
	{
		Init(props);
	}

	WinWindow::~WinWindow()
	{
		ShutDown();
	}

	void WinWindow::OnUpdate()
	{
		//���ܴ���Event
		glfwPollEvents();
		//������window�����ĺ󻺴滭����ָ��û���ͨ����ͨ�����ڵ�ǰ�󻺴�Ľ�������ɵġ�
		glfwSwapBuffers(m_Window);
	}

	void WinWindow::SetVSync(bool enabled)
	{
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		m_Data.VSync = enabled;
	}

	bool WinWindow::IsVSync() const
	{
		return m_Data.VSync;
	}

	void WinWindow::Init(const WindowProps& props)
	{
		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;

		ABLAZE_CORE_INFO("Creating Window {0},{1},{2}", props.Title, props.Width, props.Height);
		if (!s_GLFWInitialized) {
			int success = glfwInit();
			ABLAZE_CORE_ASSERTS(success, "Could not intialize GLFW!");
			glfwSetErrorCallback(GLFWErrorCallback);
			s_GLFWInitialized = true;
		}

		m_Window = glfwCreateWindow((int)m_Data.Width, (int)m_Data.Height, m_Data.Title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_Window);

		//glfwGetProcAddress������һ�������������һ�� c �ַ�����Ȼ�󷵻غ����ĵ�ַ
		//�Ǹ�GLADloadproc����һ��ǿ������ת��
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		ABLAZE_CORE_ASSERTS(status, "Failed to initialize Glad!")

			glfwSetWindowUserPointer(m_Window, &m_Data);//���ô��ڹ������û�����ָ�롣����GLFW�����洢�������κε����⴦���Ӧ��
		SetVSync(true);
#pragma region ����GlFW�ص�
		//��m_Window���ڴ�С�仯ʱ������ص��������ᴥ��
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
		{
			//��ȡ���ڹ������û�����ָ��
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.Width = width;
			data.Height = height;

			WindowResizeEvent event(data.Width, data.Height);

			//���øô��ڵ��¼��ص�����(�ô��ڵ��¼��ص������Ѿ��ڹ��캯����ָ��ΪApplication::OnEvent())
			data.EventCallback(event);
		}
		);

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			WindowCloseEvent event;
			data.EventCallback(event);
		});

		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			switch (action)
			{
			case GLFW_PRESS:
			{
				KeyPressedEvent event(key, 0);
				data.EventCallback(event);
				break;
			}
			case GLFW_RELEASE:
			{
				KeyReleasedEvent event(key);
				data.EventCallback(event);
				break;
			}
			case GLFW_REPEAT:
			{
				KeyPressedEvent event(key, 1);
				data.EventCallback(event);
				break;
			}
			default:
				break;
			}
		});
		glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			KeyTypedEvent event(keycode);

			data.EventCallback(event);
		});
		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
			case GLFW_PRESS:
			{
				MouseButtonPressedEvent event(button);
				data.EventCallback(event);
				break;
			}
			case GLFW_RELEASE:
			{
				MouseButtonReleasedEvent event(button);
				data.EventCallback(event);
				break;
			}
			default:
				break;
			}
		});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xoffset, double yoffset)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			MouseScrolledEvent event((float)xoffset, (float)yoffset);
			data.EventCallback(event);
		});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xpos, double ypos)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			MouseMovedEvent event((float)xpos, (float)ypos);
			data.EventCallback(event);
		});

#pragma endregion
	}

	void WinWindow::ShutDown()
	{
		glfwDestroyWindow(m_Window);
	}
}