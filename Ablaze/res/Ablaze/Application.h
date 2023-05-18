#pragma once
#include "Core.h"
#include "Ablaze/Window.h"
#include "Event/ApplicationEvent.h"
#include "Layer/LayerStack.h"
#include "ImGui/ImGuiLayer.h"
#include "Ablaze/Render/Shader.h"
#include "Ablaze/Render/Buffer.h"
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
		bool OnWindowClose(WindowCloseEvent& e);
		LayerStack m_LayerStack;
		unsigned int m_VertexArray;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;
		std::unique_ptr<Shader> m_Shader;
	private:
		static Application* s_Instance;

	};
	Application* CreateApplication();
}
