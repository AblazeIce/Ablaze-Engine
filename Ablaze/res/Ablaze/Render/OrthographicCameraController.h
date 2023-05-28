#pragma once

#include "OrthographicCamera.h"
#include "Ablaze/Event/ApplicationEvent.h"
#include "Ablaze/Event/MouseEvent.h"
#include "Ablaze/Core/Timestep.h"

namespace Ablaze {
	class OrthographicCameraController {
	public:
		OrthographicCameraController(float aspectRatio,bool rotation=false);

		void Update(Timestep& timestep);
		void OnEvent(Event& e);
		OrthographicCamera& GetCamera() { return m_Camera; }
		const OrthographicCamera& GetCamera() const { return m_Camera; }
		inline float GetZoomLevel() { return m_ZoomLevel; }
		inline void SetZoomLevel(float level) { m_ZoomLevel = level; }
	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);
	private:
		//------����ӿڵĿ�߱�(x/y)-------
		float m_AspectRatio;
		//------���ż���-------
		float m_ZoomLevel = 1.0f;
		OrthographicCamera m_Camera;
		bool m_Rotation;
	private:
		glm::vec3 m_CameraPosition = {0.0f,0.0f,0.0f};
		float m_CameraRotation = 0.0f;
		float m_CameraTranslationSpeed = 1.0f;
		float m_CameraRotationSpeed = 180.0f;
	};
}