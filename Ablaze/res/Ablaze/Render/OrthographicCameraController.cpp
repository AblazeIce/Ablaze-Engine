#include "AblazePch.h"
#include "OrthographicCameraController.h"
#include "Ablaze/Core/Input.h"
#include "Ablaze/Core/KeyCodes.h"
#include "Ablaze/Core/MouseButtonCodes.h"
namespace Ablaze {
	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
		:m_AspectRatio(aspectRatio),m_Camera(-aspectRatio*m_ZoomLevel,aspectRatio*m_ZoomLevel,-m_ZoomLevel,m_ZoomLevel),m_Rotation(rotation)
	{
	}
	void OrthographicCameraController::Update(Timestep& timestep)
	{
		ABLAZE_PROFILE_FUNCTION()

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
		ABLAZE_PROFILE_FUNCTION()

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));

	}
	//-----改变缩放级别------
	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		ABLAZE_PROFILE_FUNCTION()

		m_ZoomLevel -= e.GetYOffset()*0.5f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}
	//-----改变正交投影视口的宽高比-----
	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e)
	{
		ABLAZE_PROFILE_FUNCTION()

		m_AspectRatio = (float)e.GetWidth() /(float)e.GetHeight();
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}
}