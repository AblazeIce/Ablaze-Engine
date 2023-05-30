#include "AblazePch.h"
#include "ImGuiLayer.h"
#include "imgui.h"
#include "Ablaze/Core/Application.h"

#include "GLFW/glfw3.h"
#include "glad/glad.h"

#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

//#include "Ablaze/Platform/OpenGL/imgui_impl_glfw.h"
//#include "Ablaze/Platform/OpenGL/imgui_impl_opengl3.h"

namespace Ablaze {
	ImGuiLayer::ImGuiLayer()
		:Layer("ImGuiLayer")
	{
	}
	ImGuiLayer::~ImGuiLayer()
	{
	}
	void ImGuiLayer::OnAttach()
	{
		ABLAZE_PROFILE_FUNCTION()

		//创建Imgui上下文
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGui::StyleColorsDark();
		//获得Imgui上下文和系统之间I/O的接口
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;// Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport / Platform Windows
															// 使得imgui窗口可以移出到GLFWwindow外
		
		ImGuiStyle& style = ImGui::GetStyle();
		//启用视口后，我们调整WindowRounding/WindowBg，使平台窗口看起来与常规窗口相同
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		/*Platform Binding:用GLFW的库，来监听例如按键输入，鼠标输入之类的事件，后面的
		true代表了这些输入要不要被callback回imgui*/
		Application& app = Application::Get();
		GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());
		ImGui_ImplGlfw_InitForOpenGL(window, true);

		//Renderer Binding:imgui想知道到底初始化什么GL
		ImGui_ImplOpenGL3_Init("#version 430");
	}
	void ImGuiLayer::OnDetach()
	{
		ABLAZE_PROFILE_FUNCTION()

		//销毁视窗，VAO，buffer，shader和shader program
		ImGui_ImplOpenGL3_Shutdown();
		//取消Callback绑定
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}
	void ImGuiLayer::Begin()
	{
		ABLAZE_PROFILE_FUNCTION()

		//创建VertexArray，Shader，Buffer，Attributes等一系列渲染相关的必要Device Object
		ImGui_ImplOpenGL3_NewFrame();
		//设定视窗大小，当前DeltaTime，更新鼠标位置，鼠标按键，鼠标光标，更新手柄事件等IO相关的行为。
		ImGui_ImplGlfw_NewFrame();
		//处理包括视窗，渲染帧，IO，FPS，DeltaTime，字体，拖曳，鼠标悬停，视窗及渲染数据读写，垃圾回收等
		ImGui::NewFrame();
	}
	void ImGuiLayer::End()
	{
		ABLAZE_PROFILE_FUNCTION()

		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();
		io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());

		//总合数据，提交给Renderer，设置DrawData，渲染鼠标光标，总合渲染的顶点数据和顶点Indices数据
		ImGui::Render();
		//把之前汇总的数据上传到Renderer，并且进行渲染
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}

}