#pragma once
#include "Ablaze/Core/Core.h"
#include <utility>

namespace Ablaze {
	class ABLAZE_API Input {
	public:
		inline static bool isKeyPressed(int keycode) { return s_Instance->isKeyPressedImpl(keycode); }
		inline static bool isMouseButtonPressed(int button) { return s_Instance->isMouseButtonPressedImpl(button); }
		inline static std::pair<double, double> GetMousePosition() { return s_Instance->GetMousePositionImpl(); }
		inline static double GetMouseX() { return s_Instance->GetMouseXImpl(); }
		inline static double GetMouseY() { return s_Instance->GetMouseYImpl(); }
	protected:
		virtual bool isKeyPressedImpl(int keycode) = 0;
		virtual bool isMouseButtonPressedImpl(int button) = 0;
		virtual std::pair<double, double> GetMousePositionImpl() = 0;
		virtual double GetMouseXImpl() = 0;
		virtual double GetMouseYImpl() = 0;
	private:
		static Input* s_Instance;
	};
}