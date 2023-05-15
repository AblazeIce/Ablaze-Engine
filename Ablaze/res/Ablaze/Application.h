#pragma once
#include "Core.h"
#include "Ablaze/Window.h"
#include "Event/ApplicationEvent.h"
#include "Layer/LayerStack.h"

namespace Ablaze
{
	class ABLAZE_API Application
	{
	public:
		Application();
		virtual ~Application();
		void Run();

		void OnEvent(Event& e);
		void PushLayer(Layer* layer);
		void PushOverLayer(Layer* layer);
	private:
		std::unique_ptr<Window> m_Window;
		bool m_isRunning = true;
		bool OnWindowClose(WindowCloseEvent& e);
		LayerStack m_LayerStack;
	};
	Application* CreateApplication();
}
