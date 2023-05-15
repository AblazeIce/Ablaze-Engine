#pragma once
#include "Ablaze/Core.h"
#include "Ablaze/Event/Event.h"

namespace Ablaze {
	class ABLAZE_API Layer {
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate() {}
		virtual void OnEvent(Event& event) {}
	protected:
		std::string m_DebugName;
	};
}