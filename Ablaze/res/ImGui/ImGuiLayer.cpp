#include "pch/AblazePch.h"
#include "ImGuiLayer.h"
#include "imgui.h"
#include "Ablaze/Application.h"

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
		//����Imgui������
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGui::StyleColorsDark();
		//���Imgui�����ĺ�ϵͳ֮��I/O�Ľӿ�
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;// Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport / Platform Windows
															// ʹ��imgui���ڿ����Ƴ���GLFWwindow��
		
		ImGuiStyle& style = ImGui::GetStyle();
		//�����ӿں����ǵ���WindowRounding/WindowBg��ʹƽ̨���ڿ������볣�洰����ͬ
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		/*Platform Binding:��GLFW�Ŀ⣬���������簴�����룬�������֮����¼��������
		true��������Щ����Ҫ��Ҫ��callback��imgui*/
		Application& app = Application::Get();
		GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());
		ImGui_ImplGlfw_InitForOpenGL(window, true);

		//Renderer Binding:imgui��֪�����׳�ʼ��ʲôGL
		ImGui_ImplOpenGL3_Init("#version 430");
	}
	void ImGuiLayer::OnDetach()
	{
		//�����Ӵ���VAO��buffer��shader��shader program
		ImGui_ImplOpenGL3_Shutdown();
		//ȡ��Callback��
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}
	void ImGuiLayer::OnImGuiRender()
	{
		static bool show = true;
		ImGui::ShowDemoWindow();
	}
	void ImGuiLayer::Begin()
	{
		//����VertexArray��Shader��Buffer��Attributes��һϵ����Ⱦ��صı�ҪDevice Object
		ImGui_ImplOpenGL3_NewFrame();
		//�趨�Ӵ���С����ǰDeltaTime���������λ�ã���갴��������꣬�����ֱ��¼���IO��ص���Ϊ��
		ImGui_ImplGlfw_NewFrame();
		//��������Ӵ�����Ⱦ֡��IO��FPS��DeltaTime�����壬��ҷ�������ͣ���Ӵ�����Ⱦ���ݶ�д���������յ�
		ImGui::NewFrame();
	}
	void ImGuiLayer::End()
	{
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();
		io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());

		//�ܺ����ݣ��ύ��Renderer������DrawData����Ⱦ����꣬�ܺ���Ⱦ�Ķ������ݺͶ���Indices����
		ImGui::Render();
		//��֮ǰ���ܵ������ϴ���Renderer�����ҽ�����Ⱦ
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}
	//void ImGuiLayer::OnEvent(Event& event)
	//{
	//	EventDispatcher dispatcher(event);

	//	dispatcher.Dispatch<MouseButtonPressedEvent>(BIND_EVENT_FN(ImGuiLayer::OnMouseButtonPressedEvent));
	//	dispatcher.Dispatch<MouseButtonReleasedEvent>(BIND_EVENT_FN(ImGuiLayer::OnMouseButtonReleasedEvent));
	//	dispatcher.Dispatch<MouseMovedEvent>(BIND_EVENT_FN(ImGuiLayer::OnMouseMovedEvent));
	//	dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_FN(ImGuiLayer::OnMouseScrolledEvent));
	//	dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(ImGuiLayer::OnKeyPressedEvent));
	//	dispatcher.Dispatch<KeyReleasedEvent>(BIND_EVENT_FN(ImGuiLayer::OnKeyReleasedEvent));
	//	dispatcher.Dispatch<KeyTypedEvent>(BIND_EVENT_FN(ImGuiLayer::OnKeyTypedEvent));
	//	dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(ImGuiLayer::OnWindowResizeEvent));
	//}
	//bool ImGuiLayer::OnMouseButtonPressedEvent(MouseButtonPressedEvent& e)
	//{
	//	ImGuiIO& io = ImGui::GetIO();
	//	io.MouseDown[e.GetMouseButton()] = true;
	//	//��������Ҳ�ܴ����������¼�
	//	return false;
	//}
	//bool ImGuiLayer::OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e)
	//{
	//	ImGuiIO& io = ImGui::GetIO();
	//	io.MouseDown[e.GetMouseButton()] = false;
	//	return false;
	//}
	//bool ImGuiLayer::OnMouseMovedEvent(MouseMovedEvent& e)
	//{
	//	ImGuiIO& io = ImGui::GetIO();
	//	io.MousePos = ImVec2(e.GetX(), e.GetY());
	//	return false;
	//}
	//bool ImGuiLayer::OnMouseScrolledEvent(MouseScrolledEvent& e)
	//{
	//	ImGuiIO& io = ImGui::GetIO();
	//	io.MouseWheelH += e.GetXOffset();
	//	io.MouseWheel += e.GetYOffset();
	//	return false;
	//}
	//bool ImGuiLayer::OnKeyPressedEvent(KeyPressedEvent& e)
	//{
	//	ImGuiIO& io = ImGui::GetIO();
	//	io.KeysDown[e.GetKeyCode()] = true;

	//	io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
	//	io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
	//	io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
	//	io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];
	//	return false;
	//}
	//bool ImGuiLayer::OnKeyReleasedEvent(KeyReleasedEvent& e)
	//{
	//	ImGuiIO& io = ImGui::GetIO();
	//	io.KeysDown[e.GetKeyCode()] = false;
	//	return false;
	//}
	//bool ImGuiLayer::OnKeyTypedEvent(KeyTypedEvent& e)
	//{
	//	ImGuiIO& io = ImGui::GetIO();
	//	int keycode = e.GetKeyCode();
	//	if (keycode > 0 && keycode < 0x10000)
	//	{
	//		io.AddInputCharacter((unsigned short)keycode);
	//	}
	//	
	//	return false;
	//}
	//bool ImGuiLayer::OnWindowResizeEvent(WindowResizeEvent& e)
	//{
	//	ImGuiIO& io = ImGui::GetIO();
	//	io.DisplaySize = ImVec2(e.GetWidth(), e.GetHeight());
	//	io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
	//	glViewport(0, 0, e.GetWidth(), e.GetHeight());

	//	return false;
	//}
}