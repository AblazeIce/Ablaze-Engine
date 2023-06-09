#pragma once
#include "Ablaze/Core/Window.h"
#include "GLFW/glfw3.h"
#include "Ablaze/Render/GraphicsContext.h"
namespace Ablaze {
	class WinWindow :public Window {
	public:
		WinWindow(const WindowProps& props);
		virtual ~WinWindow();

		void OnUpdate() override;

		inline unsigned int GetWidth() const override { return m_Data.Width; }
		inline unsigned int GetHeight() const override { return m_Data.Height; }

		inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
		void SetVSync(bool enabled) override;
		bool IsVSync() const override;

		inline void* GetNativeWindow() const override { return m_Window; }
	private:
		virtual void Init(const WindowProps& props);
		virtual void ShutDown();

	private:
		GLFWwindow* m_Window;

		GraphicsContext* m_OpenGLContext;
		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;
			bool VSync;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
	};
}