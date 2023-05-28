#pragma once

//窗口接口
#include "AblazePch.h"
#include "Ablaze/Core/Core.h"
#include "Ablaze/Event/Event.h"

namespace Ablaze {
	struct WindowProps
	{
		std::string Title;
		unsigned int Width;
		unsigned int Height;
		WindowProps(const std::string& title = "Ablaze-Engine", unsigned int width = 1280, unsigned int height = 720)
			:Title(title), Width(width), Height(height) {}
	};
	class ABLAZE_API Window {
	public:
		using EventCallbackFn = std::function<void(Event&)>;
		virtual ~Window() {}
		virtual void OnUpdate() = 0;
		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void* GetNativeWindow() const = 0;

		//谁继承谁实例化返回一个
		static Window* Create(const WindowProps& props = WindowProps());
	};
}