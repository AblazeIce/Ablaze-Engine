#include "AblazePch.h"
#include "Ice2D.h"


Ice2D::Ice2D()
	:Layer("Ice2D"), m_CameraController(1280.0f / 720.0f), m_Color({ 0.2f, 0.3f, 0.8f, 1.0f })
{}

void Ice2D::OnAttach()
{
	ABLAZE_PROFILE_FUNCTION()

		m_VA = Ablaze::VertexArray::Create();

	float vertices[4 * 5] = {
			-0.5f,-0.5f,0.0f,0.0f,0.0f,
			 0.5f,-0.5f,0.0f,1.0f,0.0f,
			 0.5f, 0.5f,0.0f,1.0f,1.0f,
			-0.5f, 0.5f,0.0f,0.0f,1.0f
	};
	std::shared_ptr<Ablaze::VertexBuffer> VBF = Ablaze::VertexBuffer::Create(vertices, sizeof(vertices));

	Ablaze::BufferLayout layout = {
		{Ablaze::ShaderDataType::Float3,"a_Position"},
		{Ablaze::ShaderDataType::Float2,"a_TexCoord"}
	};
	VBF->SetLayout(layout);

	m_VA->AddVertexBuffer(VBF);

	uint32_t indices[6] = { 0,1,2,0,2,3 };
	std::shared_ptr<Ablaze::IndexBuffer> VIB = Ablaze::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
	m_VA->SetIndexBuffer(VIB);

	auto m_Shader = m_ShaderLibrary.Load("Assets/Shaders/Texture.hlsl");
	m_Texture = Ablaze::Texture2D::Create("Assets/Textures/spongebob.png");
}

void Ice2D::OnDetach()
{
	ABLAZE_PROFILE_FUNCTION()

}

void Ice2D::OnUpdate(Ablaze::Timestep timestep)
{
	ABLAZE_PROFILE_FUNCTION()


		m_CameraController.Update(timestep);
	{
		ABLAZE_PROFILE_SCOPE("Renderer Prep")
			Ablaze::RenderCommand::SetClearColor({ 0,0,0,1 });
		Ablaze::RenderCommand::Clear();
	}

	/*Ablaze::Renderer::BeginScene(m_CameraController.GetCamera());
	m_Texture->Bind();
	std::dynamic_pointer_cast<Ablaze::OpenGLShader>(m_ShaderLibrary.Get("Texture"))->UploadUniformFloat4("m_Color", m_Color);
	Ablaze::Renderer::Submit(m_VA, m_ShaderLibrary.Get("Texture"), glm::mat4(1.0f));
	Ablaze::Renderer::EndScene();*/
	{
		ABLAZE_PROFILE_SCOPE("Renderer Draw");
		Ablaze::Renderer2D::BeginScene(m_CameraController.GetCamera());

		Ablaze::Renderer2D::EndScene();
	}
}

void Ice2D::OnImGuiRender()
{
	ABLAZE_PROFILE_FUNCTION()
		ImGui::Begin("Setting");
	ImGui::ColorEdit4("m_Color", glm::value_ptr(m_Color));

	ImGui::End();

}

void Ice2D::OnEvent(Ablaze::Event& event)
{
	m_CameraController.OnEvent(event);
}
