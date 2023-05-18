#include "pch/AblazePch.h"
#include "Application.h"
#include "glad/glad.h"

namespace Ablaze
{
	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		ABLAZE_CORE_ASSERTS(!s_Instance, "Application already exists");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));

		m_ImGuiLayer =new ImGuiLayer();
		PushOverLayer(m_ImGuiLayer);

		glGenVertexArrays(1, &m_VertexArray);
		glBindVertexArray(m_VertexArray);

		
		float vertices[3 * 3] = {
			-0.5f,-0.5f,0.0f,
			 0.5f,-0.5f,0.0f,
			 0.0f, 0.5f,0.0f,
		};
		m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

		uint32_t indices[3] = { 0,1,2 };
		m_IndexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices)/sizeof(uint32_t)));

		std::string vertexSrc = R"(
			#version 330 core	

			layout(location=0) in vec3 a_Position;

			void main()
			{
				gl_Position=vec4(a_Position,1.0);
			}
		)";
		std::string pragmentSrc = R"(
			#version 330 core

			layout(location=0) out vec4 color;

			void main()
			{
				color=vec4(0.0,1.0,0.0,1);
			}
		)";
		m_Shader.reset(new Shader(vertexSrc,pragmentSrc));
	}
	void Application::OnEvent(Event& e)
	{
		//为传进来的每个事件生成一个事件句柄
		EventDispatcher dispatcher(e);
		//如果回调事件e为WindowCloseEvent,则调用OnWindowClose函数以结束Run()函数(关闭窗口);
		//并使e.m_Handled=true(表示e事件已被处理)
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));

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

	Application::~Application()
	{
	}

	void Application::Run()
	{
		while (m_isRunning)
		{
			//函数指定颜色缓冲区的清除值
			glClearColor(1, 0, 0.5, 1);
			//函数将缓冲区清除为预设值。
			glClear(GL_COLOR_BUFFER_BIT);

			m_Shader->Bind();
			glBindVertexArray(m_VertexArray);
			glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);

			for (auto& layer : m_LayerStack) {
				layer->OnUpdate();
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