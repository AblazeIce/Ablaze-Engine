#include "pch/AblazePch.h"
#include "OrthographicCameraController.h"
#include "Input.h"
#include "Codes/KeyCodes.h"
#include "Codes/MouseButtonCodes.h"
namespace Ablaze {
	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
		:m_AspectRatio(aspectRatio),m_Camera(-aspectRatio*m_ZoomLevel,aspectRatio*m_ZoomLevel,-m_ZoomLevel,m_ZoomLevel),m_Rotation(rotation)
	{
	}
	void OrthographicCameraController::Update(Timestep& timestep)
	{
		if (Input::isKeyPressed(ABLAZE_KEY_A))
			m_CameraPosition.x += m_CameraTranslationSpeed * timestep;
		else if (Input::isKeyPressed(ABLAZE_KEY_D))
			m_CameraPosition.x -= m_CameraTranslationSpeed * timestep;
		if (Input::isKeyPressed(ABLAZE_KEY_W))
			m_CameraPosition.y -= m_CameraTranslationSpeed * timestep;
		else if (Input::isKeyPressed(ABLAZE_KEY_S))
			m_CameraPosition.y += m_CameraTranslationSpeed * timestep;
		if (m_Rotation) {
			if (Ablaze::Input::isKeyPressed(ABLAZE_KEY_Q))
				m_CameraRotation -= m_CameraRotationSpeed * timestep;
			else if (Ablaze::Input::isKeyPressed(ABLAZE_KEY_E))
				m_CameraRotation += m_CameraRotationSpeed * timestep;
			m_Camera.SetRotation(m_CameraRotation);
		}
		m_Camera.SetPosition(m_CameraPosition);
		m_CameraTranslationSpeed = m_ZoomLevel;
	}
	void OrthographicCameraController::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));

	}
	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		m_ZoomLevel -= e.GetYOffset()*0.5f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}
	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e)
	{
		m_AspectRatio = (float)e.GetWidth() /(float)e.GetHeight();
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}
}