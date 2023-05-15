#pragma once
#include "Ablaze/Input.h"

namespace Ablaze{
	class ABLAZE_API WinInput:public Input {
	protected:
		virtual bool isKeyPressedImpl(int keycode) override;
		virtual bool isMouseButtonPressedImpl(int button) override;
		virtual std::pair<float, float> GetMousePositionImpl()override;
		virtual float GetMouseXImpl() override;
		virtual float GetMouseYImpl() override;
	};
}