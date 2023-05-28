#pragma once
#include "Ablaze/Core/Input.h"

namespace Ablaze{
	class ABLAZE_API WinInput:public Input {
	protected:
		virtual bool isKeyPressedImpl(int keycode) override;
		virtual bool isMouseButtonPressedImpl(int button) override;
		virtual std::pair<double, double> GetMousePositionImpl()override;
		virtual double GetMouseXImpl() override;
		virtual double GetMouseYImpl() override;
	};
}