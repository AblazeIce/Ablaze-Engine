#include "pch/AblazePch.h"
#include "Application.h"

namespace Ablaze
{
	Application::Application()
	{
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));
	}
	void Application::OnEvent(Event& e)
	{
		//Ϊ��������ÿ���¼�����һ���¼����
		EventDispatcher dispatcher(e);
		//����ص��¼�eΪWindowCloseEvent,�����OnWindowClose�����Խ���Run()����(�رմ���);
		//��ʹe.m_Handled=true(��ʾe�¼��ѱ�����)
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();) {
			(*--it)->OnEvent(e);
			if (e.GetEventIsHandled())//�¼��ѱ�����
				break;
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
	}

	void Application::PushOverLayer(Layer* layer)
	{
		m_LayerStack.PushOverlay(layer);
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_isRunning = false;
		return true;
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		//WindowResizeEvent e(1920, 1080);
		//ABLAZE_CORE_INFO(e);
		while (m_isRunning)
		{
			m_Window->OnUpdate();
			for (auto& layer : m_LayerStack) {
				layer->OnUpdate();
			}
		}
	}
}