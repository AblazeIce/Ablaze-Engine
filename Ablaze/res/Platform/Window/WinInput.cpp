#include "AblazePch.h"
#include "WinInput.h"
#include "Ablaze/Core/Application.h"
#include "GLFW/glfw3.h"
namespace Ablaze{
	Input* WinInput::s_Instance = new WinInput();
	bool WinInput::isKeyPressedImpl(int keycode)
	{
		//静态类型转换，将void*转换为GLFWwindow*
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetKey(window, keycode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}
	bool WinInput::isMouseButtonPressedImpl(int button)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetMouseButton(window, button);

		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	std::pair<double, double> WinInput::GetMousePositionImpl()
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		return std::pair<double, double>(xpos, ypos);
	}

	double WinInput::GetMouseXImpl()
	{
		auto [x, y] = GetMousePositionImpl();
		return x;
	}

	double WinInput::GetMouseYImpl()
	{
		auto [x, y] = GetMousePositionImpl();
		return y;
	}
}