#pragma once
#include "Layer/Layer.h"
#include "Ablaze/Event/ApplicationEvent.h"
#include "Ablaze/Event/KeyEvent.h"
#include "Ablaze/Event/MouseEvent.h"
namespace Ablaze {
	class ABLAZE_API ImGuiLayer:public Layer 
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate() override;
		virtual void OnImGuiRender() override;


		void Begin();
		void End();
	private:
		float m_Time = 0.0f;

		
	};
}

