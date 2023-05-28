#pragma once
#include "Ablaze/Core/Core.h"
#include "Ablaze/Event/Event.h"
#include "Ablaze/Core/Timestep.h"

namespace Ablaze {
	class ABLAZE_API Layer {
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {}  
		virtual void OnDetach() {}
		virtual void OnUpdate(Timestep timestep) {}
		virtual void OnImGuiRender(){}
		virtual void OnEvent(Event& event) {}
	protected:
		std::string m_DebugName;
	};
}