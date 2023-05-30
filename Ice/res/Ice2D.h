#pragma once
#include "Ablaze.h"

#include "imgui.h"
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>
#include "Platform/OpenGL/OpenGLShader.h"

class Ice2D :public Ablaze::Layer {
public:
	Ice2D();
	virtual ~Ice2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(Ablaze::Timestep timestep) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(Ablaze::Event& event) override;

private:
	std::shared_ptr<Ablaze::Texture2D> m_CheckerboardTexture;
	std::shared_ptr<Ablaze::Texture2D> m_spongebobTexture;
	Ablaze::OrthographicCameraController m_CameraController;

	glm::vec4 u_Color;

private:
	
};