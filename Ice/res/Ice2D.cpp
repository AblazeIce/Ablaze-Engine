#include "AblazePch.h"
#include "Ice2D.h"


Ice2D::Ice2D()
	:Layer("Ice2D"), m_CameraController(1280.0f / 720.0f,true), u_Color(1.0f)
{}

void Ice2D::OnAttach()
{
	ABLAZE_PROFILE_FUNCTION()

	m_CheckerboardTexture = Ablaze::Texture2D::Create("assets/textures/Checkerboard.png");
	m_spongebobTexture = Ablaze::Texture2D::Create("assets/textures/spongebob.png");
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

	{
		ABLAZE_PROFILE_SCOPE("Renderer Draw");
		Ablaze::Renderer2D::BeginScene(m_CameraController.GetCamera());
		/*Ablaze::Renderer2D::DrawRotatedQuad({ -1,0 }, { 1.0f,1.0f }, glm::radians(-45.0f),{ 1,0,0,1 });
		Ablaze::Renderer2D::DrawRotatedQuad({ 0,2 }, { 1.0f,1.0f }, glm::radians(-45.0f), m_spongebobTexture);
		Ablaze::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 10.0f, 10.0f }, m_CheckerboardTexture, 10.0f,u_Color);*/
		Ablaze::Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
		Ablaze::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });
		Ablaze::Renderer2D::EndScene();
	}
}

void Ice2D::OnImGuiRender()
{
	ABLAZE_PROFILE_FUNCTION()
		ImGui::Begin("Setting");
	ImGui::ColorEdit4("u_Color", glm::value_ptr(u_Color));

	ImGui::End();

}

void Ice2D::OnEvent(Ablaze::Event& event)
{
	m_CameraController.OnEvent(event);
}
