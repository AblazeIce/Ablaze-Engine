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
		//���øò�������ġ���ʼ��...
		layer->OnAttach();
	}

	void Application::PushOverLayer(Layer* layer)
	{
		m_LayerStack.PushOverlay(layer);
		//���øò�������ġ���ʼ��...
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
			//����ָ����ɫ�����������ֵ
			glClearColor(1, 0, 0.5, 1);
			//���������������ΪԤ��ֵ��
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