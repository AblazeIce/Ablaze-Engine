#pragma once
#include "Core.h"
#include "Ablaze/Core/Window.h"
#include "Ablaze/Event/ApplicationEvent.h"
#include "Ablaze/Core/LayerStack.h"
#include "Ablaze/ImGui/ImGuiLayer.h"

#include "Ablaze/Core/Timestep.h"



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
		inline Window& GetWindow() { return *m_Window; }
		inline static Application& Get() { return *s_Instance; }
	private:
		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_isRunning = true;
		bool m_Minimized = false;
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
		LayerStack m_LayerStack;
		
	private:
		Timestep m_Timestep;
		float m_LastFrameTime = 0.0f;
	private:
		static Application* s_Instance;

	};
	Application* CreateApplication();
}
