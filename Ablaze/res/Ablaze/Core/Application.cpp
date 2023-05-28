#include "AblazePch.h"
#include "Application.h"
#include "GLFW/glfw3.h"
#include "Ablaze/Render/Renderer.h"
namespace Ablaze
{
	
	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		ABLAZE_CORE_ASSERTS(!s_Instance, "Application already exists");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));

		//启动混合
		Renderer::Init();

		m_ImGuiLayer =new ImGuiLayer();
		PushOverLayer(m_ImGuiLayer);

		
	}
	void Application::OnEvent(Event& e)
	{
		//为传进来的每个事件生成一个事件句柄
		EventDispatcher dispatcher(e);
		//如果回调事件e为WindowCloseEvent,则调用OnWindowClose函数以结束Run()函数(关闭窗口);
		//并使e.m_Handled=true(表示e事件已被处理)
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(Application::OnWindowResize));

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();) {
			(*--it)->OnEvent(e);
			if (e.GetEventIsHandled())//事件已被处理
				break;
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		//设置该层的上下文、初始化...
		layer->OnAttach();
	}

	void Application::PushOverLayer(Layer* layer)
	{
		m_LayerStack.PushOverlay(layer);
		//设置该层的上下文、初始化...
		layer->OnAttach();
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_isRunning = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		if (e.GetWidth() == 0 || e.GetHeight() == 0) {
			m_Minimized = true;
			return false;
		}
		m_Minimized = false;
		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());
		return false;
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		while (m_isRunning)
		{
			float time = (float)glfwGetTime();
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			if (!m_Minimized)
			{
				for (auto& layer : m_LayerStack) {
					layer->OnUpdate(timestep);
				}
				

			}
			m_ImGuiLayer->Begin();
			for (auto& layer : m_LayerStack) {
				layer->OnImGuiRender();
			}
			m_ImGuiLayer->End();
			m_Window->OnUpdate();
		}
	}
}