#include "Ablaze.h"

#include "imgui.h"
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>
#include "Ablaze/Platform/OpenGL/OpenGLShader.h"
class ExampleLayer :public Ablaze::Layer
{
public:
	ExampleLayer()
		:Layer("Example"),m_Camera(-1.0f,1.0f, -0.7f, 0.7f),m_CameraPosition(0.0f)
	{
		m_VertexArray=Ablaze::VertexArray::Create();


		float vertices[4 * 5] = {
			-0.5f,-0.5f,0.0f,0.0f,0.0f,
			 0.5f,-0.5f,0.0f,1.0f,0.0f,
			 0.5f, 0.5f,0.0f,1.0f,1.0f,
			-0.5f, 0.5f,0.0f,0.0f,1.0f
		};
		m_VertexBuffer=Ablaze::VertexBuffer::Create(vertices, sizeof(vertices));

		Ablaze::BufferLayout layout = {
			{Ablaze::ShaderDataType::Float3,"a_Position"},
			{Ablaze::ShaderDataType::Float2, "a_TexCoord"}
		};
		m_VertexBuffer->SetLayout(layout);

		m_VertexArray->AddVertexBuffer(m_VertexBuffer);


		uint32_t indices[6] = { 0,1,2,0,2,3 };
		m_IndexBuffer=Ablaze::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));

		m_VertexArray->SetIndexBuffer(m_IndexBuffer);

		auto m_Shader=m_ShaderLibrary.Load("Assets/Shaders/Texture.hlsl");

		m_Texture=Ablaze::Texture2D::Create("Assets/Textures/1.png");
		m_spongebobTexture=Ablaze::Texture2D::Create("Assets/Textures/spongebob.png");
		std::dynamic_pointer_cast<Ablaze::OpenGLShader>(m_Shader)->UploadUniformInt("m_Texture", 0);
	}
	void OnUpdate(Ablaze::Timestep timestep) override
	{
		//ABLAZE_TRACE("Delta time:{0}s ({1}ms)", timestep.GetSeconds(), timestep.GetMilliseconds());
		if (Ablaze::Input::isKeyPressed(ABLAZE_KEY_LEFT))
			m_CameraPosition.x += m_CameraMoveSpeed*timestep;
		else if (Ablaze::Input::isKeyPressed(ABLAZE_KEY_RIGHT))
			m_CameraPosition.x -= m_CameraMoveSpeed * timestep;
		if (Ablaze::Input::isKeyPressed(ABLAZE_KEY_UP))
			m_CameraPosition.y -= m_CameraMoveSpeed * timestep;
		else if (Ablaze::Input::isKeyPressed(ABLAZE_KEY_DOWN))
			m_CameraPosition.y += m_CameraMoveSpeed * timestep;

		if (Ablaze::Input::isKeyPressed(ABLAZE_KEY_A))
			m_CameraRotation-= m_CameraRotationSpeed * timestep;
		else if (Ablaze::Input::isKeyPressed(ABLAZE_KEY_D))
			m_CameraRotation += m_CameraRotationSpeed * timestep;

		Ablaze::RenderCommand::SetClearColor({ 0,0,0,1 });
		Ablaze::RenderCommand::Clear();

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);
		Ablaze::Renderer::BeginScene(m_Camera);
		//std::dynamic_pointer_cast<Ablaze::OpenGLShader>(m_Shader)->UploadUniformFloat3("u_Color", m_TriangleColor);
		/*for (int i = 0; i < 10; ++i) {
			for (int j = 0; j < 10; ++j) {
				Ablaze::Renderer::Submit(m_VertexArray, m_Shader,glm::translate(glm::mat4(1.0f),{1.1f*i,1.1f*j,0.0f}));

			}
		}*/
		m_spongebobTexture->Bind();
		auto m_Shader = m_ShaderLibrary.Get("Texture");
		Ablaze::Renderer::Submit(m_VertexArray, m_Shader, glm::translate(glm::mat4(1.0f), glm::vec3(0.0f))*glm::scale(glm::mat4(1.0f),glm::vec3(1.5f)));
		m_Texture->Bind();
		Ablaze::Renderer::Submit(m_VertexArray, m_Shader, glm::translate(glm::mat4(1.0f), glm::vec3(0.0f))*glm::scale(glm::mat4(1.0f),glm::vec3(1.5f)));

		Ablaze::Renderer::EndScene();

	}
	void OnEvent(Ablaze::Event& event) override
	{
	}
	void OnImGuiRender() override
	{
		
	} 
private:
	std::shared_ptr<Ablaze::VertexArray> m_VertexArray;
	std::shared_ptr<Ablaze::VertexBuffer> m_VertexBuffer;
	std::shared_ptr<Ablaze::IndexBuffer> m_IndexBuffer;
	Ablaze::ShaderLibrary m_ShaderLibrary;
	std::shared_ptr<Ablaze::Texture2D> m_Texture,m_spongebobTexture;

	glm::vec3 m_CameraPosition;
	float m_CameraRotation = 0.0f;
	float m_CameraMoveSpeed = 10.0f;
	float m_CameraRotationSpeed = 100.0f;
	Ablaze::OrthographicCamera m_Camera;

};
class Ice :public Ablaze::Application
{
public:
	Ice() {
		PushLayer(new ExampleLayer());
		
	}
	~Ice() {}
};
Ablaze::Application* Ablaze::CreateApplication()
{
	return new Ice();
}